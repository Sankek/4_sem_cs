#include <future>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>


template < typename Iterator, typename UnaryFunction>
void parallel_for_each(Iterator first, Iterator last, UnaryFunction f, bool first_entrance=true)
{
  const std::size_t length = std::distance(first, last);

  static std::size_t max_size;
  if (first_entrance){
    auto n_threads = std::thread::hardware_concurrency();
    max_size = length/(n_threads ? n_threads : 2);
  }

  if (length <= max_size)
  {
    std::for_each(first, last, f);
  }
  else
  {
    Iterator middle = first;
    std::advance(middle, length / 2);

    std::future <void> first_half_result =
        std::async(parallel_for_each <Iterator, UnaryFunction>, first, middle, f, false);

    parallel_for_each(middle, last, f, false);

    first_half_result.wait();
  }
}

int main(int argc, char ** argv)
{
  std::vector < int > v(100000000);

  std::iota(v.begin(), v.end(), 1);


  auto f = [](int &x){ x = (x*x*x*x*x/3-(x+x+x)/(3*x/(x+x)))/19;};

  std::for_each(v.begin(), v.end(), f);
  std::cout << "Done.\n";
  std::iota(v.begin(), v.end(), 1);
  parallel_for_each(v.begin(), v.end(), f);
  std::cout << "Done.\n";
  std::iota(v.begin(), v.end(), 1);
  parallel_for_each(v.begin(), v.end(), f);
  std::cout << "Done.\n";


  system("pause");

  return EXIT_SUCCESS;
}