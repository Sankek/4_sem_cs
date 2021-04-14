#define BOOST_DATE_TIME_NO_LIB
#define BOOST_REGEX_NO_LIB

#include <iostream>

#include <boost/asio.hpp>

void write_data(boost::asio::ip::tcp::socket & socket, const std::string& data)
{
  boost::asio::write(socket, boost::asio::buffer(data));
}

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

void receive_message(std::mutex &m,
                     std::atomic<bool> &ready_to_exit,
                     boost::asio::ip::tcp::socket &socket){

  while(!ready_to_exit){
    std::this_thread::yield();
    std::scoped_lock lock(m);

    while (socket.available()) {
      std::string msg;
      try {
        msg = read_data_until(socket);
      } catch (const boost::system::system_error &er) {
        std::cout << "Connection is lost!\n";
        system("pause");
        exit(1);
      }
      std::cout << msg << '\n';
    }
  }
}

int main(int argc, char ** argv)
{
  system("chcp 1251");

  std::string raw_ip_address = "93.175.6.132";

  auto port = 3333;

  try
  {
    boost::asio::ip::tcp::endpoint endpoint(
        boost::asio::ip::address::from_string(raw_ip_address), port);

    boost::asio::io_service io_service;

    boost::asio::ip::tcp::socket socket(io_service, endpoint.protocol());

    socket.connect(endpoint);

    std::string input;
    std::cout << "Enter your name, please: ";
    std::getline(std::cin, input);
    input += char(4);
    write_data(socket, input);

    std::atomic<bool> ready_to_exit = false;

    std::mutex m;

    auto f = std::async(std::launch::async,
                        receive_message,
                        std::ref(m),
                        std::ref(ready_to_exit),
                        std::ref(socket));

    while(!ready_to_exit){
      std::getline(std::cin, input);
      if (input == "/exit"){
        ready_to_exit = true;
      }
      input += char(4);
      std::scoped_lock lock(m);
      try {
        write_data(socket, input);
      } catch (const boost::system::system_error &er) {
        std::cout << "Connection is lost!\n";
        system("pause");
        exit(1);
      }
    }

    try{
      f.get();
    } catch(const std::exception &err){
      std::cerr << err.what() << '\n';
    }

  }
  catch (boost::system::system_error & e)
  {
    std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what() << std::endl;

    system("pause");

    return e.code().value();
  }

  system("pause");

  return EXIT_SUCCESS;
}