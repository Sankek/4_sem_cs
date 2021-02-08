#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>


class Timer{
 public:
  using Clock_t = std::chrono::steady_clock;
  using time_point_t = Clock_t::time_point;
  Timer() = default;
  ~Timer() = default;

  void start(){
    m_begin = Clock_t::now();
  }

  void stop(){
    m_end = Clock_t::now();
  }

  int_fast64_t result(){
    return std::chrono::duration_cast<std::chrono::milliseconds> (m_end-m_begin).count();
  }

 private:
  time_point_t m_begin{};
  time_point_t m_end{};

};

std::vector<int> random_vector(int size){
  std::vector<int> v(size);
  std::generate(v.begin(), v.end(), std::rand);  // limited randomness!
  return v;
}

double timeit_vector_sort(int size, int n=10){
  /*
   *  Return mean time of computation std::sort on
   *  random std::vector<int> per n iterations.
   */

  Timer timer{};

  int_fast64_t mean{};
  std::vector<int> v(size);
  for (int i{}; i<n; ++i){
    v = random_vector(size);
    timer.start();
    std::sort(v.begin(), v.end());
    timer.stop();
    mean += timer.result();
  }

  return static_cast<double>(mean)/n;
}

int main() {
  std::cout << "std::sort of random n-vectors:\n";
  for (int n{1000}; n<=100000; n*=10){
    std::cout << n << ": " << timeit_vector_sort(n, 100) << " ms\n";
  }
  return 0;
}