#include <iostream>
#include <future>
#include <atomic>

#include "threadsafe_priority_queue.h"

using ts_priority_queue = threadsafe_priority_queue<int>;

std::atomic<bool> END_OF_INSERT(false);

void inserter(ts_priority_queue &queue, int x){
  auto id = std::this_thread::get_id();
  for (int i{}; i<5; i++){
    std::cout << "PUSHING " << x+i << " (id:" << id << ")\n";
    queue.push(x+i);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

void printer(ts_priority_queue &queue){
  auto id = std::this_thread::get_id();
  while (!END_OF_INSERT.load()){
    int x;
    bool success;
    success = queue.try_pop(x);
    if (!success){
      continue;
    }
    std::cout << "PRINTING " << x << " (id:" << id << ")\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
  }
}


int main(int argc, char** argv) {
  ts_priority_queue queue{};

  auto hw_concurrency = std::thread::hardware_concurrency();
  auto max_thread = (hw_concurrency ? hw_concurrency : 2);

  std::cout << "Max threads: " << max_thread << '\n';

  std::vector<std::shared_future<void>> futures;

  for (int i{}; i<max_thread/2-1; ++i){
    auto f = std::async(std::launch::async, inserter, std::ref(queue), (i+1)*100).share();
    futures.emplace_back(f);
  }

  for (int i{}; i<max_thread-max_thread/2; ++i){
    auto f = std::async(std::launch::async, printer, std::ref(queue)).share();
    futures.emplace_back(f);
  }

  inserter(queue, 1000);

  std::cout << "FUTURES : " << futures.size() << '\n';

  while (!queue.empty()){
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  END_OF_INSERT.store(true);

  return EXIT_SUCCESS;
}