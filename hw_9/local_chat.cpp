#define BOOST_DATE_TIME_NO_LIB
#define BOOST_CB_DISABLE_DEBUG


#include <iostream>
#include <string>
#include <mutex>
#include <chrono>
#include <algorithm>
#include <future>
#include <atomic>

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/circular_buffer.hpp>

using segment_manager_t = boost::interprocess::managed_shared_memory::segment_manager;
using char_allocator_t = boost::interprocess::allocator<char, segment_manager_t>;
using string_t = std::basic_string<char, std::char_traits<char>, char_allocator_t>;
using int_allocator_t = boost::interprocess::allocator <int, segment_manager_t>;
using string_allocator_t = boost::interprocess::allocator <string_t, segment_manager_t>;
using vector_int_t = boost::interprocess::vector <int, int_allocator_t >;
using vector_string_t = boost::interprocess::vector <string_t, string_allocator_t>;
using buffer_pair_allocator_t = boost::interprocess::allocator<std::pair<int, string_t>, segment_manager_t>;
using buffer_t = boost::circular_buffer<std::pair<int, string_t>, buffer_pair_allocator_t>;

using condition_t = boost::interprocess::interprocess_condition;
using mutex_t = boost::interprocess::interprocess_mutex;

void receive_message(buffer_t *buffer,
                     int *message_count,
                     int user_id,
                     condition_t *ioc,
                     mutex_t *iom,
                     std::atomic<bool> &ready_to_exit){

  auto size = *message_count;
  while(!ready_to_exit){
    std::unique_lock lock(*iom);

    ioc->wait(lock, [message_count, size](){ return *message_count > size; });

    while (*message_count > size){
      auto [msg_user_id, msg] = (*buffer)[size%buffer->capacity()];
      if (msg_user_id != user_id){
        std::cout << msg << '\n';
      }
      size++;
    }
  }
}

int main(int argc, char ** argv){
  system("pause");

  const std::string shmem_name = "local_chat";
  const size_t shmem_size = 65536;

  int user_id = std::chrono::system_clock::now().time_since_epoch().count();

  boost::interprocess::managed_shared_memory shmem(boost::interprocess::open_or_create,
                                                   shmem_name.c_str(),
                                                   shmem_size);

  auto user_ids = shmem.find <vector_int_t> ("user_ids").first;


  if (!user_ids || user_ids->empty()){
    // clearing shared memory, seems not so effective
    boost::interprocess::shared_memory_object::remove(shmem_name.c_str());
    shmem = boost::interprocess::managed_shared_memory(boost::interprocess::create_only,
                                                       shmem_name.c_str(),
                                                       65536);
    user_ids = shmem.construct<vector_int_t>("user_ids")(shmem.get_segment_manager());
  }

  user_ids->push_back(user_id);

  auto m = shmem.find_or_construct<boost::interprocess::interprocess_mutex>("mutex")();
  auto c = shmem.find_or_construct<boost::interprocess::interprocess_condition> ("condition")();

//  auto data = shmem.find_or_construct<vector_string_t>("data")(shmem.get_segment_manager());
  auto message_count = shmem.find_or_construct <int> ("message_count")(0);
  auto buffer = shmem.find_or_construct<buffer_t>("buffer")(shmem.get_segment_manager());
  buffer->set_capacity(10);

  std::atomic<bool> ready_to_exit = false;

  std::string user_name;
  std::cout << "Enter your name, please\n";
  std::getline(std::cin, user_name);

  {
    std::scoped_lock lock(*m);
    if (!buffer->empty()){
      for (const auto &[id, msg] : *buffer){
        std::cout << msg << '\n';
      }
    }
  }

  auto f = std::async(std::launch::async, receive_message, buffer, message_count, user_id, c, m, std::ref(ready_to_exit));
  std::string input;
  while(!ready_to_exit){
    std::getline(std::cin, input);
    std::unique_lock lock(*m);
    if (input == "/exit"){
      ready_to_exit = true;
    }
    string_t shmem_str(shmem.get_segment_manager());
    shmem_str = user_name + ": " + input;
    buffer->push_back(std::make_pair(user_id, shmem_str));
    (*message_count)++;
    c->notify_all();
  }
  try{
    f.get();
  } catch(const std::exception &err){
    std::cerr << err.what() << '\n';
  }

  // del user id
  user_ids->erase(std::remove(user_ids->begin(), user_ids->end(), user_id), user_ids->end());

  if (user_ids->empty()){
    boost::interprocess::shared_memory_object::remove(shmem_name.c_str());
  }

  return EXIT_SUCCESS;
}