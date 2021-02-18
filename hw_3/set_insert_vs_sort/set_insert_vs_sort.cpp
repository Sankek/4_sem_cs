#include <iostream>
#include <set>
#include <vector>
#include <array>
#include <utility>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <random>
#include <limits>
#include <iomanip>
#include <string>
#include <sstream>


#include "../../hw_1/Timer.h"
#include "../../hw_2/containers_speedtest/Table.h"


using value_stdev_t = std::pair<double, double>;
using timer_t = Timer<std::chrono::microseconds>;


static std::random_device random_device;
static std::mt19937 rng{random_device()};
static std::uniform_int_distribution<int> random_int(std::numeric_limits<int>::min(),
                                                     std::numeric_limits<int>::max());

template<class T>
T random_number(){
  return static_cast<T>(random_int(rng));
}

template<class T>
std::vector<T> random_vector(size_t size){
  std::vector<T> v(size);
  std::generate(v.begin(), v.end(), random_number<T>);
  return v;
}

template<class T>
std::string number_to_string(T number, size_t precision=3){
  std::stringstream stream;
  stream << std::fixed << std::setprecision(precision) << number;
  std::string s = stream.str();
  return s;
}

namespace math {

template<class Container>
double mean(Container container) {
  double sum = std::accumulate(std::begin(container), std::end(container), 0.0);
  double result = sum / std::size(container);

  return result;
}

template<class Container>
double stdev(Container container, double mean) {
  double square_sum{};
  std::for_each(std::begin(container), std::end(container), [&](const double item) {
    square_sum += (item - mean) * (item - mean);
  });
  double result = std::sqrt(square_sum / (std::size(container) - 1));

  return result;
}

}


template<class Container, class T>
value_stdev_t measure_insert_time(Container &container, const std::vector<T> &data, int iters=10){
  std::vector<int_least64_t> times(iters);

  timer_t timer{};
  for (int i{}; i<iters; ++i) {
    container.clear();
    timer.start();
    container.insert(data.begin(), data.end());
    timer.stop();
    times[i] = timer.result();
  }

  double mean = math::mean(times);
  double stdev = math::stdev(times, mean);
  return std::make_pair(mean, stdev);
}

template<class Container, class T>
value_stdev_t measure_sort_time(Container &container, const std::vector<T> &data, int iters=10){
  std::vector<int_least32_t> times(iters);

  timer_t timer{};
  for (int i{}; i<times.size(); ++i) {
    timer.start();
    std::copy(data.begin(), data.end(), container.begin());
    std::sort(container.begin(), container.end());
    timer.stop();
    times[i] = timer.result();
  }

  double mean = math::mean(times);
  double stdev = math::stdev(times, mean);
  return std::make_pair(mean, stdev);
}

template<class T, size_t Size>
void make_measurements(Table &table, const std::string& column, int iters_num=1000){
  std::cout << "Started filling column " << column << "...\n";

  auto data {random_vector<T>(Size)};
  {
    std::vector<T> v{};
    v.resize(Size);
    auto [time, time_std] = measure_sort_time(v, data, iters_num);
    std::cout << "std::vector : " << time << ' ' << time_std << '\n';
    table.set_element("std::vector (μs)", column, number_to_string(time));
    table.set_element("std::vector stdev (μs)", column, number_to_string(time_std));
  }

  {
    auto arr_p = new std::array<T, Size>;
    auto [time, time_std] = measure_sort_time(*arr_p, data, iters_num);
    delete arr_p;
    std::cout << "std::array : " << time << ' ' << time_std << '\n';
    table.set_element("std::array (μs)", column, number_to_string(time));
    table.set_element("std::array stdev (μs)", column, number_to_string(time_std));
  }

  {
    std::set<T> s{};
    auto [time, time_std] = measure_insert_time(s, data, iters_num);
    std::cout << "std::set : " << time << ' ' << time_std << '\n';
    table.set_element("std::set (μs)", column, number_to_string(time));
    table.set_element("std::set stdev (μs)", column, number_to_string(time_std));
  }

  std::cout << '\n';
}

int main(int argc, char **argv){
  constexpr int sizes_num = 7;
  constexpr int sizes[sizes_num] = {100, 1000, 10000, 100000, 1000000, 10000000, 100000000};

  std::vector<std::string> rows = {
      "std::vector (μs)", "std::vector stdev (μs)",
      "std::array (μs)", "std::array stdev (μs)",
      "std::set (μs)", "std::set stdev (μs)"};

  std::vector<std::string> cols = {
      "1e2", "1e3", "1e4", "1e5", "1e6", "1e7"
  };

  Table table(rows, cols);

  make_measurements<int, sizes[0]>(table, "1e2", 100000);
  make_measurements<int, sizes[1]>(table, "1e3", 5000);
  make_measurements<int, sizes[2]>(table, "1e4", 1000);
  make_measurements<int, sizes[3]>(table, "1e5", 1000);
  make_measurements<int, sizes[4]>(table, "1e6", 5);
  make_measurements<int, sizes[5]>(table, "1e7", 5);

  table.to_csv("../results.csv");

  return 0;
}