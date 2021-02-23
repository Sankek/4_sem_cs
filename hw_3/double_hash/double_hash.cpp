#include <iostream>
#include <unordered_set>
#include <bitset>
#include <cassert>
#include <random>
#include <fstream>
#include <string>
#include <filesystem>


static std::random_device random_device;
static std::mt19937 rng{random_device()};
static std::uniform_real_distribution<double> real_unit(-1, 1);
static std::uniform_real_distribution<double> real(0, std::numeric_limits<double>::max());
static std::uniform_int_distribution<int> bool_rng(0,1);


double random_double_standard(){ // (not used)
  // to avoid assertion failure when uniform real min << 0
  return (bool_rng(rng)) ? real(rng) : -real(rng);
}

double random_double(){
  // randomize 64bits and reinterpret them as double to test all possible hash results
  std::bitset<sizeof(double)*CHAR_BIT> bits{};

  for (int i{}; i < sizeof(double)*CHAR_BIT; ++i) {
    bits[i] = bool_rng(rng);
  }

  auto ul = bits.to_ullong();
  double x = *reinterpret_cast<double*>(&ul);

  return x;
}

double random_double_unit(){
  return real_unit(rng);
}

std::bitset<sizeof(double)*CHAR_BIT> BitRepresentation(const double &d){
  double v = d;
  auto *array = reinterpret_cast<uint8_t*>(&v);

  std::bitset<sizeof(double)*8> bit_repr{};
  for (int i = 0; i < sizeof(double); ++i) {
    uint8_t byte = array[i];
    for (int bit{}; bit<CHAR_BIT; ++bit){
      bit_repr[i*CHAR_BIT+bit] = ((byte & (1 << bit)) == (1 << bit));
    }
  }

  return bit_repr;
}

struct MyDoubleHash{
  uint_least64_t operator()(double d) const {
    auto b = BitRepresentation(d);

    assert(b.size()==64 && "No permutation for hashing bit representation");
    std::vector<int> perm_64{12, 55, 47, 53, 44, 37, 24,  8, 42, 17, 14, 45,  1, 61, 20, 56, 27,
                             5,  9, 32, 34, 16,  3, 33, 15, 29,  6, 36, 46, 49, 39, 26, 19, 35,
                             54, 40, 62,  4, 38, 25, 50, 41,  2, 10,  0, 51, 22, 31, 60, 30, 21,
                             52, 58, 28, 18, 63, 48, 13, 11, 57,  7, 23, 43, 59};


    std::bitset<b.size()> bit_repr;
    for (int i{}; i<b.size(); ++i){
      bit_repr[i] = b[perm_64[i]];

    }
    bit_repr = bit_repr ^ (BitRepresentation(d*17.0+31.7) << 3ULL);


    return bit_repr.to_ullong();
  }
};

template<class F>
void TestDoubleHash(F hash_struct){
  auto hash = hash_struct;

  std::vector<size_t> test_bucket_sizes{127, 128, 250, 1001, 1009, 3919};
  std::vector<std::string> rows{
      "from int",
      "wide-range random",
      "narrow-range random"
  };

  // determines how many ints will be casted to double and hashed
  // and the total count of data points for given sampling method
  int const index_range{10000000};

  // test hash for several variants of bucket size
  for (size_t bucket_size_i{}; bucket_size_i<test_bucket_sizes.size(); ++bucket_size_i) {
    auto bucket_size = test_bucket_sizes[bucket_size_i];
    std::cout << "Testing hash function for bucket size " << bucket_size << "...\n";
    std::vector<std::vector<size_t>> data_per_size(rows.size());
    for (auto &v : data_per_size){
      v.resize(bucket_size, 0);
    }

    // filling buckets with different data sampling methods
    for (int i{-index_range}; i < index_range; ++i) {
      if (std::abs(i)%(index_range/10) == 0){
        std::cout << "Progress: " << static_cast<double>(i+index_range)/(2*index_range)*100 << "%\n";
      }
      data_per_size[0][hash(static_cast<double>(i)) % bucket_size]++;
      data_per_size[1][hash(random_double()) % bucket_size]++;
      data_per_size[2][hash(random_double_unit()) % bucket_size]++;
    }

    // saving these data for given bucket size
    std::cout << "Saving results...\n";
    std::filesystem::create_directory("../results");
    std::fstream file{};
    file.open("../results/buckets_size_" + std::to_string(bucket_size) + ".csv", std::ios::out);
    for (size_t row{}; row<rows.size(); ++row){
      file << rows[row] << ',';
      for (size_t i{}; i<bucket_size-1; ++i){
        file << data_per_size[row][i] << ',';
      }
      file << data_per_size[row][bucket_size-1] << '\n';
    }
  }
}

int main(int argc, char **argv){
  /*
   * Здесь предлагается хэш-функция для чисел с плавающей точкой и определяются её свойства.
   * Хэширование происходит путем перемешивания битов в двоичном представлении числа, нескольких
   * побитовых операций, зависящих от аргумента хэш-функции и последующего преобразования к типу
   * 64-битных беззнаковых целых чисел.
   * Для проверки равномерности заполнения генерируется много double чисел тремя способами:
   * "from int", - берутся целые числа и кастуются к double
   * "wide-range random", - double генерируются из случайных 64-битных последовательностей
   * "narrow-range random", - генерируются случаные double из равномерного распределения (-1, 1).
   * Далее проверяется равномерность заполнения бакетов различных размеров.
   * Результаты в папке results.
   *
   * Свойства:
   * - Равномерное заполнение на бакетах некоторых размеров,
   *   кажется, равномерность вероятнее на бакетах с размером в виде простого числа
   * - Неравноменое заполнение на бакетах с размерами в виде степеней двойки, а также
   *   некоторых других чисел
   * - Детерминированность из построения гарантируется только на данной архитектуре
   *   процессора/компилятора из-за возможно разного расположения битов в представлении double.
   *   Несмотря на это, другие свойства должны сохраняться, так как в процессе хэширования
   *   биты перемешиваются, то есть теряется привязаность битов к конкретному положению.
   * - Хэширование выполняется достаточно быстро, по крайней мере
   *   его скорость не зависит от самого числа.
   */

  std::cout << "Starting testing hash function:\n";
  try{
    TestDoubleHash(MyDoubleHash());
  } catch (std::exception &msg){
    std::cerr << "Hash function test failed: " << msg.what() << '\n';
  }

  return 0;
}