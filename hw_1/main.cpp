#include <iostream>
#include <vector>
#include <algorithm>

#include "Timer.h"


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

  Timer<std::chrono::milliseconds> timer{};

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