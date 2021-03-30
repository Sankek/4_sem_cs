#include <iostream>
#include <vector>
#include <future>
#include <atomic>
#include <fstream>

#include <boost/lockfree/stack.hpp>
#include <boost/lockfree/queue.hpp>

#include "threadsafe_queue.h"
#include "threadsafe_stack.h"
#include "../../hw_1/Timer.h"

std::atomic<bool> EXECUTION_STARTED(false);

template <typename BoostT>
void push_op_boost (BoostT & container, size_t num_iter_per_thread)
{
  while (!EXECUTION_STARTED.load())
  {
    std::this_thread::yield();
  }

  for (size_t i{}; i < num_iter_per_thread; ++i)
  {
    container.bounded_push(123);
  }
}

template <typename BoostT>
void pop_op_boost (BoostT & container, size_t num_iter_per_thread)
{
  while (!EXECUTION_STARTED.load()){
    std::this_thread::yield();
  }
  size_t counter = 0;
  int temp;
  while (counter < num_iter_per_thread)
  {
    counter += container.pop(temp);
  }
}

template <typename SeminarT>
void pop_op_seminar (SeminarT & container, size_t num_iter_per_thread)
{
  while (!EXECUTION_STARTED.load())
  {
    std::this_thread::yield();
  }
  std::size_t counter = 0;
  int temp;
  for (size_t i{}; i < num_iter_per_thread; ++i)
  {
    container.wait_and_pop(temp);
  }
}

template <typename SeminarT>
void push_op_seminar (SeminarT & container, size_t num_iter_per_thread)
{
  while (!EXECUTION_STARTED.load())
  {
    std::this_thread::yield();
  }
  for (size_t i{}; i < num_iter_per_thread; ++i)
  {
    container.push(123);
  }
}

template<class BoostT, class SeminarT>
auto measure_structs(BoostT &boost, SeminarT &seminar, size_t num_iter_per_thread) {
  Timer<std::chrono::milliseconds> timer;

  auto hw_concurrency = std::thread::hardware_concurrency();
  auto max_thread = (hw_concurrency ? hw_concurrency : 2);


  {

    std::vector<std::future<void> > pushers(max_thread / 2);
    std::vector<std::future<void> > popers(max_thread / 2 - 1);

    for (auto &future: pushers) {
      future = std::async(std::launch::async, push_op_boost<BoostT>, std::ref(boost), num_iter_per_thread);
    }
    for (auto &future: popers) {
      future = std::async(std::launch::async, pop_op_boost<BoostT>, std::ref(boost), num_iter_per_thread);
    }
    timer.start();
    EXECUTION_STARTED.store(true);
    pop_op_boost(boost, num_iter_per_thread);
    while (!boost.empty()) {
      std::this_thread::yield();
    }
  timer.stop();

  }
  auto boost_result = timer.result();

  {

    std::vector<std::future<void> > pushers(max_thread / 2);
    std::vector<std::future<void> > popers(max_thread / 2 - 1);
    for (auto &future: pushers) {
      future = std::async(std::launch::async, push_op_seminar<SeminarT>, std::ref(seminar), num_iter_per_thread);
    }
    for (auto &future: popers) {
      future = std::async(std::launch::async, pop_op_seminar<SeminarT>, std::ref(seminar), num_iter_per_thread);
    }
    EXECUTION_STARTED.store(true);
    pop_op_seminar(seminar, num_iter_per_thread);
    while (!seminar.empty()) {
      std::this_thread::yield();
    }
    timer.stop();

  }
  auto seminar_result = timer.result();


  return std::make_pair(boost_result, seminar_result);
}

int main(int argc, char** argv) {
  const size_t big_size = 10000000;
  boost::lockfree::stack <int> boost_stack(big_size);
  boost::lockfree::queue <int> boost_queue(big_size);
  threadsafe_stack <int> seminar_stack;
  threadsafe_queue <int> seminar_queue;


  std::ofstream file_stack("../output_queue.csv", std::ios::out);
  std::ofstream file_queue("../output_stack.csv", std::ios::out);

  size_t repeats = 30;
  for (size_t i{1000}; i<big_size; i*=10){
    size_t boost_stack_time{};
    size_t seminar_stack_time{};
    size_t boost_queue_time{};
    size_t seminar_queue_time{};
    for (size_t repeat{}; repeat<repeats; repeat++){
      auto [boost_stack_, seminar_stack_ ] = measure_structs(boost_stack, seminar_stack, i);
      auto [boost_queue_, seminar_queue_ ] = measure_structs(boost_queue, seminar_queue, i);

      boost_stack_time += boost_stack_;
      seminar_stack_time += seminar_stack_;
      boost_queue_time +=  boost_queue_;
      seminar_queue_time += seminar_queue_;
    }
    boost_stack_time /= repeats;
    seminar_stack_time /= repeats;
    boost_queue_time /= repeats;
    seminar_queue_time /= repeats;
    file_stack << i << ',' << boost_stack_time << ',' << seminar_stack_time << '\n';
    file_queue << i << ',' << boost_queue_time << ',' << seminar_queue_time << '\n';
  }

  return EXIT_SUCCESS;
}
