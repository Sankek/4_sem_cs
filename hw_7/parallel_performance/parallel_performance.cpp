#include <iostream>
#include <vector>
#include <execution>
#include <cmath>
#include <fstream>

#include "../../hw_1/Timer.h"

auto measure_for_each(size_t size){
  std::vector < int > v(size);
  std::iota(v.begin(), v.end(), 1);
  
  Timer<std::chrono::nanoseconds> t;

  auto f = [](auto &x){ x = std::sin(x*x*x/((x+3)*(x+4))*std::cos(x*x*x*(x+1)*(x+2)));};
  t.start();
  std::for_each(std::execution::seq, v.begin(), v.end(), f);
  t.stop();
  auto seq_time = t.result();
  
  t.start();
  std::for_each(std::execution::par, v.begin(), v.end(), f);
  t.stop();
  auto par_time = t.result();
  
  return std::make_pair(seq_time, par_time);
}

auto measure_partial_sum(size_t size){
  std::vector < int > v(size);
  std::vector < int > temp(size);
  std::iota(v.begin(), v.end(), 1);
  
  Timer<std::chrono::nanoseconds> t;

  t.start();
  std::partial_sum(v.begin(), v.end(), temp.begin());
  t.stop();
  auto seq_time = t.result();

  t.start();
  std::inclusive_scan(v.begin(), v.end(), temp.begin());
  t.stop();
  auto par_time = t.result();

  return std::make_pair(seq_time, par_time);
}

auto measure_inner_product(size_t size){
  std::vector < int > v1(size);
  std::iota(v1.begin(), v1.end(), 1);
  std::vector < int > v2(size);
  std::iota(v2.rbegin(), v2.rend(), 1);

  Timer<std::chrono::nanoseconds> t;

  t.start();
  auto ipr1 = std::inner_product(v1.begin(), v1.end(), v2.begin(), 0);
  t.stop();
  auto seq_time = t.result();

  t.start();
  auto ipr2 = std::transform_reduce(std::execution::par, v1.begin(), v1.end(), v2.begin(), 0);
  t.stop();
  auto par_time = t.result();

  if (ipr1 == ipr2){
    // do nothing
  }

  return std::make_pair(seq_time, par_time);
}

template<typename F>
auto average_time(F f, size_t size, size_t repeats){
  size_t seq_time_mean = 0;
  size_t par_time_mean = 0;
  for (size_t i = 0; i<repeats; ++i){
    auto [seq_time, par_time] = f(size);
    seq_time_mean += seq_time;
    par_time_mean += par_time;
  }
  seq_time_mean /= repeats;
  par_time_mean /= repeats;

  return std::make_pair(seq_time_mean, par_time_mean);
}


int main(int argc, char ** argv)
{
  std::ofstream file("../output.csv", std::ios::out);
  file << "size,for_each (seq),for_each (par),partial_sum,inclusive_scan,inner_product,transform_reduce\n";
  for (size_t size = 1000; size<100000000; size*=10){
    size_t repeats = 200;
    auto for_each_time = average_time(measure_for_each, size, repeats);
    auto part_sum_time = average_time(measure_partial_sum, size, repeats);
    auto inn_prod_time = average_time(measure_inner_product, size, repeats);

    file << size << ','
         << for_each_time.first << ',' << for_each_time.second << ','
         << part_sum_time.first << ',' << part_sum_time.second << ','
         << inn_prod_time.first << ',' << inn_prod_time.second << '\n';
  }

  return EXIT_SUCCESS;
}