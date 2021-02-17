#include <iostream>
#include <vector>
#include <list>
#include <forward_list>
#include <deque>
#include <array>
#include <algorithm>
#include <random>
#include <iomanip>
#include <sstream>

#include "../../hw_1/Timer.h"
#include "Table.h"

static std::random_device random_device;
static std::mt19937 rng{random_device()};
static std::uniform_int_distribution<int> random_int(-1000000000, 1000000000);

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
std::string number_to_string(T number, size_t precision=2){
  std::stringstream stream;
  stream << std::fixed << std::setprecision(precision) << number;
  std::string s = stream.str();
  return s;
}


template<class Container>
void sort_f(Container &container) {
  std::sort(container.begin(), container.end());
}

template<class T>
void sort_f(std::list<T> &container){
  container.sort();
}

template<class T>
void sort_f(std::forward_list<T> &container){
  container.sort();
}


template<class Container, class T>
double measure_sort_time(Container &container, const std::vector<T> &data, int iters= 10){
  std::copy(data.begin(), data.end(), container.begin());

  Timer timer{};
  int_fast64_t mean{};
  for (int i{}; i<iters; ++i) {
    std::copy(data.begin(), data.end(), container.begin());
    timer.start();
    sort_f(container);
    timer.stop();
    mean += timer.result();
  }

  return static_cast<double>(mean)/iters;
}


template<size_t Size>
void fill(Table &table, const std::string& column, int iters_num=1000){
  auto data {random_vector<int>(Size)};
  double time{};
  {
    std::vector<int> v{};
    v.resize(Size);
    time = measure_sort_time(v, data, iters_num);
    std::cout << time << '\n';
    table.set_element("std::vector", column, number_to_string(time));
  }

  {
    std::list<int> l{};
    l.resize(Size);
    time = measure_sort_time(l, data, iters_num);
    std::cout << time << '\n';
    table.set_element("std::list", column, number_to_string(time));
  }

  {
    std::forward_list<int> fl{};
    fl.resize(Size);
    time = measure_sort_time(fl, data, iters_num);
    std::cout << time << '\n';
    table.set_element("std::forward_list", column, number_to_string(time));
  }

  {
    std::deque<int> d{};
    d.resize(Size);
    time = measure_sort_time(d, data, iters_num);
    std::cout << time << '\n';
    table.set_element("std::deque", column, number_to_string(time));
  }

  {
    auto arr_p = new std::array<int, Size>;
    time = measure_sort_time(*arr_p, data, iters_num);
    delete arr_p;
    std::cout << time << '\n';
    table.set_element("std::array", column, number_to_string(time));
  }

  std::cout << '\n';

}

int main(int argc, char **argv){
  /* 
   * Здесь проверяется скорость сортировки 5 последовательных контейнеров
   * из стандартной библиотеки.
   * 
   * Сортировка производится функцией std::sort, а для списков --
   * методом sort. Для всех контейнеров замеры производятся на 
   * одинаковых случайных данных разных размеров. 
   * std::array переведен на heap для большего размера и 
   * одинаковых условий замеров. 
   * Полученные результаты сведены в таблицу table.pdf и изображены
   * на графике graph.pdf в миллисекундах на одну сортировку.
   *
   * std::deque оказался наиболее медленным.
   * std::list и std::forward_list показали 
   * почти одинаковые результаты, быстрее std::deque.
   * std::vector и std::array показали лучшие результаты.
   */

  constexpr int sizes_num = 7;
  constexpr int sizes[sizes_num] = {100, 1000, 10000, 100000, 1000000, 10000000, 100000000};

  Table table(
      {"std::vector", "std::list", "std::forward_list", "std::deque", "std::array"},
      {"1e3", "1e4", "1e5", "1e6", "1e7"}
  );

//  fill<sizes[0]>(table, "1e2", 5);
  fill<sizes[1]>(table, "1e3", 5000);
  fill<sizes[2]>(table, "1e4", 5000);
  fill<sizes[3]>(table, "1e5", 2500);
  fill<sizes[4]>(table, "1e6", 250);
  fill<sizes[5]>(table, "1e7", 25);
//  fill<sizes[6]>(table, "1e8", 1);

  table.to_csv("../results.csv");

}
