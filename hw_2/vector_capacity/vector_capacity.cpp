#include <iostream>
#include <vector>
#include <fstream>
#include <string>


template<typename T>
void fill(std::vector<T> &v, const T &item){
  v.resize(v.capacity(), item);
}

template<typename T>
void vector_to_csv(const std::string &path, const std::vector<T> &v){
  // elements must be printable

  std::fstream file;
  file.open(path, std::ofstream::out);
  for (size_t i{}; i<v.size()-1; ++i){
    file << v[i] << ',';
  }
  file << v[v.size()-1];
}

void push_back_test(){
  std::vector<int> test;
  std::vector<size_t> capacities;

  int repeats{1337};
  int item{};
  for (int i{}; i<repeats; ++i){
    fill(test, item);
    try{
      test.push_back(item);
    }
    catch (std::bad_alloc& ba){
      std::cerr << "bad_alloc caught when tried to push_back: " << ba.what() << '\n';
      std::cerr << "reached number of repeats: " << i << '\n';
      break;
    }
    capacities.push_back(test.capacity());
  }

  vector_to_csv("../push_back_capacities.csv", capacities);
}

void reserve_test() {
  std::vector<int> test{1};

  size_t to_reserve = 1;
  bool increasing_reserve_flag{};
  int repeats{1337};
  for (int n{}; n < repeats; ++n) {
    try {
      test.reserve(to_reserve);
    }
    catch (std::bad_alloc &ba) {
      std::cerr << "bad_alloc caught when tried to reserve(" << to_reserve << "): " << ba.what() << '\n';
      break;
    }
    if (test.capacity() != to_reserve){
      std::cout << "Tried to change capacity to " << to_reserve
      << " but it became " << test.capacity() << "!\n";
      increasing_reserve_flag = true;
    }

    to_reserve *= 2;
  }

  if (!increasing_reserve_flag){
    std::cout << "reserve works as expected when increasing capacity!\n";
  }

  test.shrink_to_fit();
  if (test.size() == test.capacity() && test.size() == 1){
    std::cout << "shrink_to_fit works as expected!\n";
  }

  test.reserve(50);
  test.reserve(40);

  if (test.capacity() == 50){
    std::cout << "reserve works as expected when reserving less than already reserved!\n";
  }

  test.resize(100);
  test.reserve(80);

  if (test.capacity() == 100){
    std::cout << "reserve works as expected when reserving less than size!\n";
  }
}

int main(int argc, char **argv){
  /*
   * Здесь проверятся система выделения памяти в std::vector
   *
   * Функция push_back_test() проверяет как увеличивается capacity
   * вектора при вызове push_back() на заполненном векторе.
   * Результаты можно посмотреть в push_back_capacities.pdf
   * Т.о. на моей системе в C++20 capacity увеличивается в 1.5 раза
   *
   * Функция reserve_test() проверяет как меняется capacity
   * вектора при вызове reserve().
   * При экпоненциальном росте аргумента reserve()
   * capacity равен этому аргументу. Насколько я понял, capacity
   * мог стать и больше, но я этого не поймал.
   * При вызове reserve() с аргументом меньшим, чем capacity,
   * capacity не уменьшается.
   *
   * В обоих случаях при выделении объема памяти больше доступного
   * ничего особого не происходит, хотя наверное и могло бы.
   */

  std::cout << "Starting push_back test...\n";
  push_back_test();
  std::cout << "push_back test is done!\n";

  std::cout << "Starting reserve test...\n";
  reserve_test();
  std::cout << "reserve test is done!\n";
}
