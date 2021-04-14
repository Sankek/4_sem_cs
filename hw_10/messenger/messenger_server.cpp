#define BOOST_DATE_TIME_NO_LIB
#define BOOST_REGEX_NO_LIB

#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <mutex>
#include <future>
#include <atomic>
#include <utility>


#include <boost/asio.hpp>


std::string read_data_until(boost::asio::ip::tcp::socket & socket)
{
  boost::asio::streambuf buffer;

  boost::asio::read_until(socket, buffer, char(4));

  std::string message;

  // Because buffer 'buf' may contain some other data
  // after '\n' symbol, we have to parse the buffer and
  // extract only symbols before the delimiter.
  std::istream input_stream(&buffer);
  std::getline(input_stream, message, char(4));

  return message;
}

void write_data(boost::asio::ip::tcp::socket & socket, const std::string &data)
{
  boost::asio::write(socket, boost::asio::buffer(data));
}


class User{
 public:
  explicit User(boost::asio::io_service &io_service, size_t id) : socket_(io_service),
                                                                  id_(id){};

  auto& socket(){
   return socket_;
  }

  void set_id(size_t id){ id_ = id; }
  void set_name(const std::string &name){ name_ = name; }
  void set_ip(const std::string &ip) { ip_ = ip; }

  auto get_id() const{ return id_; }
  auto get_name() const{ return name_; }
  auto get_ip() const{ return socket_.remote_endpoint().address().to_string(); }

  bool operator==(const User &u){ return id_ == u.id_; }

 private:
  size_t id_;
  std::string name_ = "NONAME";
  boost::asio::ip::tcp::socket socket_;
  std::string ip_ = "0.0.0.0";
};


class Session{
 public:

  explicit Session(boost::asio::ip::tcp::endpoint endpoint) : endpoint_(std::move(endpoint)){
    acceptor_.bind(endpoint_);
  };

  void start(){
    is_session_ended_ = false;
    listener_future_ = std::async(std::launch::async, &Session::Listener, this);
    writer_future_ = std::async(std::launch::async, &Session::Writer, this);

    while(!is_session_ended_){
      acceptor_.listen(max_listen_connections_);
      User u{io_service_, unique_ids_++};
      connect(u);
    }

    stop();
  }

  void Listener(){
    while(!is_session_ended_){
      std::this_thread::yield();
      std::unique_lock users_lock(users_m_);
      for (auto &user : users_){
        if (user.socket().available()){
          std::string msg;

          try{
            msg = read_data_until(user.socket());
          } catch (const std::exception &err){
            {
              std::scoped_lock io_lock(io_m_);
              std::cout << "Couldn't read user data: " << err.what() << '\n';
            }
            disconnect(user);
            break;
          }

          if (msg == "/shutdown"){
            is_session_ended_ = true;
            acceptor_.cancel();
            break;
          }

          std::scoped_lock message_queue_lock(message_queue_m_);
          message_queue_.push(std::make_pair(std::make_pair(
              user.get_id(),
              user.get_name()),
                                             msg));
          writer_condition_.notify_one();
        }
      }
    }
  }

  void Writer(){
    while(!is_session_ended_){
      std::string msg;
      size_t id;
      std::string name;

      {
        std::unique_lock lock(message_queue_m_);
        writer_condition_.wait(lock, [&]() { return !message_queue_.empty() || is_session_ended_  ; });
        if (is_session_ended_){
          return;
        }

        auto [id_name, msg_] = message_queue_.back();
        auto [id_, name_] = id_name;
        message_queue_.pop();
        msg = std::move(msg_);
        id = std::move(id_);
        name = std::move(name_);
      }
      {
        std::scoped_lock io_lock(io_m_);
        std::cout << id << ", " << name << ", " << msg << '\n';
      }

      std::scoped_lock users_lock(users_m_);
      for (auto &user : users_) {
        if (user.get_id() != id) {
          try {
            write_data(user.socket(), name + ": " + msg + char(4));
          } catch (const boost::system::system_error &er) {
            disconnect(user);
            break;
          }
        }
      }
    }

  }

  void stop(){
    is_session_ended_ = true;
    listener_future_.get();
    writer_condition_.notify_one();
    writer_future_.get();
    while (!users_.empty()){
      disconnect(users_[0]);
    }
  }

  void connect(User &u){
    try{
      acceptor_.accept(u.socket());
    } catch (const std::exception &err){
      if (is_session_ended_){
        return;
      }
      std::cout << "Error accepting user: " << err.what() << '\n';
    }
    u.set_ip(u.socket().remote_endpoint().address().to_string());

    {
      std::scoped_lock io_lock(io_m_);
      std::cout << u.get_ip() << " connected.\n";
    }

    auto name = read_data_until(u.socket()); // FIXME: run flow is blocked by 1 user
    u.set_name(name);


    std::scoped_lock lock(users_m_);
    users_.push_back(std::move(u));
  }

  void disconnect(User &u){
    {
      std::scoped_lock io_lock(io_m_);
      std::cout << u.get_name() << " (" << u.get_ip() << ") is disconnected.\n";
    }

    try{
      u.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both);
      u.socket().close();
    } catch (const boost::system::system_error &ec){
      {
        std::scoped_lock io_lock(io_m_);
        std::cout << "Connection isn't closed: " << ec.code() << ", " << ec.what() << '\n';
      }
    }

    try
    {
      std::unique_lock users_lock(users_m_);
      users_.erase(std::remove(users_.begin(), users_.end(), u), users_.end());
    }
    catch (const std::exception &msg){
      std::cout << "Couldn't update users: " << msg.what() << '\n';
    }
  }

 private:
  boost::asio::io_service io_service_;
  boost::asio::ip::tcp::endpoint endpoint_;
  boost::asio::ip::tcp::acceptor acceptor_{io_service_, endpoint_.protocol()};
  int max_listen_connections_ = 10;
  std::vector<User> users_;
  std::recursive_mutex users_m_;
  std::queue<std::pair<std::pair<size_t, std::string>, std::string>> message_queue_;
  std::mutex message_queue_m_;
  std::mutex io_m_;
  std::condition_variable writer_condition_;
  std::atomic<bool> is_session_ended_ = false;
  size_t unique_ids_ = 0;
  std::future<void> listener_future_;
  std::future<void> writer_future_;
};



int main(int argc, char ** argv)
{
  system("chcp 1251");

  auto port = 3333;

  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address_v4::any(), port);

  Session session(endpoint);
  session.start();


  system("pause");

  return EXIT_SUCCESS;
}