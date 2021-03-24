#include <algorithm>
#include <future>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>
#include <fstream>

#include "../../hw_1/Timer.h"

class Threads_Guard
{
 public:

  explicit Threads_Guard(std::vector < std::thread > & threads) :
      m_threads(threads)
  {}

  Threads_Guard			(Threads_Guard const&) = delete;

  Threads_Guard& operator=(Threads_Guard const&) = delete;

  ~Threads_Guard() noexcept
  {
    try
    {
      for (auto & m_thread : m_threads)
      {
        if (m_thread.joinable())
        {
          m_thread.join();
        }
      }
    }
    catch (...)
    {
      // std::abort();
    }
  }

 private:

  std::vector < std::thread > & m_threads;
};

template < typename Iterator, typename T >
struct accumulate_block
{
  T operator()(Iterator first, Iterator last)
  {
    return std::accumulate(first, last, T());
  }
};

template < typename Iterator, typename T >
T parallel_accumulate(Iterator first, Iterator last, T init, size_t num_threads)
{
  const std::size_t length = std::distance(first, last);

  if (!length)
    return init;

  const std::size_t block_size = length / num_threads;

  std::vector < std::future < T > > futures(num_threads - 1);
  std::vector < std::thread >		  threads(num_threads - 1);

  Threads_Guard guard(threads);

  Iterator block_start = first;

  for (std::size_t i = 0; i < (num_threads - 1); ++i)
  {
    Iterator block_end = block_start;
    std::advance(block_end, block_size);

    std::packaged_task < T(Iterator, Iterator) > task{
        accumulate_block < Iterator, T >() };

    futures[i] = task.get_future();
    threads[i] = std::thread(std::move(task), block_start, block_end);

    block_start = block_end;
  }

  T last_result = accumulate_block < Iterator, T > ()(block_start, last);

  T result = init;

  for (std::size_t i = 0; i < (num_threads - 1); ++i)
  {
    result += futures[i].get();
  }

  result += last_result;

  return result;
}

int main() {
  std::vector < int > v(100000000);
  std::iota(std::begin(v), std::end(v), 1);

  const size_t num_threads = 300;
  std::vector <size_t> threads (num_threads);
  std::iota(std::begin(threads), std::end(threads), 1);

  std::fstream file ("../output.csv", std::ios::out);

  Timer<std::chrono::microseconds> time;

  for (auto n: threads)
  {
    time.start();
    parallel_accumulate(v.begin(), v.end(), 0, n);
    time.stop();
    file << n << ',' << time.result() << std::endl;
  }
  /*
   * Оптимальное количество потоков примерно равно количеству числу логических процессов минус 1,
   * большее количество дает более долгое время работы из-за необходимости контекстного переключения
   * между задачами.
   */

  return EXIT_SUCCESS;
}