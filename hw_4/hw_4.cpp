#include <iostream>
#include <random>
#include <vector>
#include <iterator>
#include <numeric>
#include <algorithm>
#include <limits>
#include <cmath>
#include <cassert>
#include <fstream>

void print(const std::vector<int> &v, std::ostream &out, const std::string& name){
  std::cout << name << ": ";
  std::copy(std::cbegin(v),
            std::cend(v),
            std::ostream_iterator<int>(out, " "));
  std::cout << '\n';
}


int main(int argc, char **argv){
  std::ifstream in("../input.txt");
  std::ofstream out("../output.txt");
  std::cin.rdbuf(in.rdbuf()); //redirect std::cin to input.txt
  std::cout.rdbuf(out.rdbuf()); // redirect std::cout to output.txt

  static std::random_device random_device{};
  static std::mt19937 rng{random_device()};
  static std::uniform_int_distribution<int> random_int(-117, 987);


  // Task #1
  std::cout << "1. Создадим последовательность p1 целых чисел от 1 до 10\n";

  const size_t init_size = 10;
  std::vector<int> p1(init_size);
  const int start_value = 1;
  std::iota(std::begin(p1), std::end(p1), start_value);

  print(p1, std::cout, "p1");

  // Task #2
  std::cout << "2. Добавим еще несколько чисел в конец p1 из cin\n";

  std::copy(
      std::istream_iterator<int>(std::cin),
      std::istream_iterator<int>(),
      std::back_inserter(p1));
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  print(p1, std::cout, "p1");

  // Task #3
  std::cout << "3. Перемешаем p1 случайным образом\n";

  std::shuffle(std::begin(p1), std::end(p1), rng);

  print(p1, std::cout, "p1");

  // Task #4
  std::cout << "4. Удалим дубликаты из p1\n";

  std::sort(std::begin(p1), std::end(p1));
  p1.erase(std::unique(std::begin(p1), std::end(p1)), p1.end());

  print(p1, std::cout, "p1");

  // Task #5
  std::cout << "5. Подсчитаем количество нечетных чисел в p1\n";

  size_t odd_count = std::count_if(std::cbegin(p1), std::cend(p1), [](int x){ return std::abs(x)%2==1; });
  std::cout << "Quantity of odd numbers: " << odd_count << '\n';

  // Task #6
  std::cout << "6. Определим минимальное и максимальное значения в p1\n";

  const auto [min, max] = std::minmax_element(std::cbegin(p1), std::cend(p1));
  std::cout << "Min: " << *min << ", max: " << *max << '\n';

  // Task #7
  std::cout << "7. Попробуем найти хотя бы одно простое число в p1\n";

  auto prime = std::find_if(std::cbegin(p1), std::cend(p1), [](int x) {
    if (x == 2) {
      return true;
    } else if (x < 2 || x % 2 == 0) {
      return false;
    } else {
      for (size_t i{3}; i<static_cast<int>(std::sqrt(x))+1; i+=2){
        if (x % i == 0){
          return false;
        }
      }
      return true;
    }
  });
  if (prime != std::end(p1)){
    std::cout << "Prime number: " << *prime << '\n';
  } else {
    std::cout << "Prime number not found.\n";
  }


  // Task #8
  std::cout << "8. Заменим все числа в p1 их квадратами\n";

  std::transform(std::begin(p1), std::end(p1), std::begin(p1), [](int x){ return x*x;});

  print(p1, std::cout, "p1");

  // Task #9
  std::cout << "9. Создадим последовательность p2 из N случайных чисел, где N - длина p1\n";

  std::vector<int> p2(p1.size());
  std::generate(std::begin(p2), std::end(p2), [](){ return random_int(rng); });

  print(p2, std::cout, "p2");


  // Task #10
  std::cout << "10. Вычислим сумму чисел в p2\n";

  std::cout << "p2 sum: " << std::accumulate(std::cbegin(p2), std::cend(p2), 0) << '\n';

  // Task #11
  std::cout << "11. Заменим первые несколько чисел из p2 числом 1\n";

  const int fill_value = 1;
  std::fill(std::begin(p2),
            std::next(std::begin(p2),
            std::abs(random_int(rng)) % std::distance(std::cbegin(p2), std::cend(p2))),
            fill_value);

  print(p2, std::cout, "p2");

  // Task #12
  std::cout << "12. Создадим последовательность p3 как разность p1 и p2\n";

  assert(p1.size() == p2.size() && "p1 and p2 sequences' sizes are differ\n");

  std::vector<int> p3;
  std::transform(std::cbegin(p1), std::cend(p1), std::cbegin(p2),
                 std::back_inserter(p3), [](int x, int y){ return x-y;});

  print(p3, std::cout, "p3");

  // Task #13
  std::cout << "13. Заменим каждый отрицательный элемент в p3 нулем\n";

  const int replace_value = 0;
  std::replace_if(std::begin(p3), std::end(p3), [](int x){ return (x < 0); }, replace_value);

  print(p3, std::cout, "p3");

  // Task #14
  std::cout << "14. Удалим полностью все нулевые элементы из p3\n";

  const int remove_value = 0;
  p3.erase(std::remove(p3.begin(), p3.end(), remove_value), p3.end());

  print(p3, std::cout, "p3");

  // Task #15
  std::cout << "15. Изменим порядок следования элементов в p3 на обратный\n";

  std::reverse(p3.begin(), p3.end());

  print(p3, std::cout, "p3");

  // Task #16
  std::cout << "16. Определим быстро топ-3 наибольших элемента в p3\n";

  const size_t largest_count = 3;
  std::cout << "Largest numbers: ";
  if (p3.size() >= largest_count){
    std::nth_element(std::begin(p3), std::next(std::begin(p3), largest_count-1), std::end(p3), std::greater<>());
    std::copy(std::cbegin(p3),
              std::next(std::cbegin(p3), largest_count),
              std::ostream_iterator<int>(out, " "));
    std::cout << '\n';
  } else {
    std::copy(std::cbegin(p3),
              std::cend(p3),
              std::ostream_iterator<int>(out, " "));
    std::cout << '\n';
  }


  // Task #17
  std::cout << "17. Отсортируем полностью p1 и p2 по возрастанию\n";

  std::sort(std::begin(p1), std::end(p1));
  std::sort(std::begin(p2), std::end(p2));

  print(p1, std::cout, "p1");
  print(p2, std::cout, "p2");

  // Task #18
  std::cout << "18. Создадим последовательность p4 как слияние p1 и p2\n";

  std::vector<int> p4;
  std::merge(std::cbegin(p1), std::cend(p1),
             std::cbegin(p2), std::cend(p2),
             std::back_inserter(p4));

  print(p4, std::cout, "p4");

  // Task #19
  std::cout << "19. Определим диапазон для упорядоченной вставки числа 1 в p4\n";

  const int range_value = 1;
  auto [lower_bound, upper_bound] = std::equal_range(std::cbegin(p4), std::cend(p4), range_value);

  std::cout << "Sorted range to insert " << range_value << " is from "
            << std::distance(std::cbegin(p4), lower_bound) << " to "
            << std::distance(std::cbegin(p4), upper_bound) << '\n';

  // Task #20
  std::cout << "20. Выведем все последовательности в cout\n";

  print(p1, std::cout, "p1");
  print(p2, std::cout, "p2");
  print(p3, std::cout, "p3");
  print(p4, std::cout, "p4");

  return EXIT_SUCCESS;
}