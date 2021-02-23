#include <iostream>
#include <string>
#include <unordered_set>
#include <random>
#include <fstream>
#include <filesystem>
#include <iomanip>

std::string CHARACTERS { "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ" };

static std::random_device random_device;
static std::mt19937 rng{random_device()};
static std::uniform_int_distribution<size_t> uniform_size(std::numeric_limits<size_t>::min(),
                                                          std::numeric_limits<size_t>::max());
static std::uniform_int_distribution<size_t> uniform_char_index(0, CHARACTERS.size()-1);

std::string random_string(size_t size){
  std::string str{};
  str.resize(size);
  std::generate(str.begin(), str.end(), [](){return CHARACTERS[uniform_char_index(rng)];});
  return str;
}

size_t random_size(size_t max=std::numeric_limits<size_t>::max()){
  return uniform_size(rng)%max;
}


// http://www.boost.org/doc/libs/1_35_0/doc/html/hash/combine.html

template < typename T >
void hash_combine(std::size_t & seed, const T & value) noexcept
{
  seed ^= std::hash < T > ()(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template < typename T >
void hash_value(std::size_t & seed, const T & value) noexcept
{
  hash_combine(seed, value);
}

template < typename T, typename ... Types >
void hash_value(std::size_t & seed, const T & value, const Types & ... args) noexcept
{
  hash_combine(seed, value);
  hash_value(seed, args...);
}

template < typename ... Types >
std::size_t hash_value(const Types & ... args) noexcept
{
  std::size_t seed = 0;
  hash_value(seed, args...);
  return seed;
}

class Customer
{
 public:
  explicit Customer(const std::string & surname, const std::string & name, const std::size_t mark) :
      surname_(surname), name_(name), mark_(mark)
  {}

  ~Customer() noexcept = default;

  friend std::ostream & operator << (std::ostream & stream, const Customer & customer)
  {
    return (stream << customer.surname_ << ',' << customer.name_ << "," << customer.mark_);
  }

 private:
  friend struct Customer_Hash;
  friend struct Customer_Equal;
  std::string name_;
  std::string surname_;
  std::size_t mark_;
};

struct Customer_Hash
{
  std::size_t operator() (const Customer & customer) const noexcept
  {
    return hash_value(customer.name_, customer.mark_);
  }
};

struct Customer_Equal
{
  bool operator() (const Customer & lhs, const Customer & rhs) const noexcept
  {
    return (lhs.surname_ == rhs.surname_ &&
            lhs.name_ == rhs.name_ &&
            lhs.mark_ == rhs.mark_);
  }
};

Customer random_customer(){
  return Customer(random_string(random_size(10)), random_string(random_size(10)), random_size());
}

template<class ... Types>
size_t collisions_number(const std::unordered_set<Types ...> &s){
  size_t collisions{};
  for (size_t i{}; i<s.bucket_count(); ++i){
    if (s.bucket_size(i) != 0){
      collisions += s.bucket_size(i)-1;
    }
  }
  return collisions;
}


int main(int argc, char ** argv)
{
  /*
   * Здесь исследуется равномерность заполнения хэш-таблицы и зависимость
   * количества коллизий от количества элементов в ней при использовании
   * хэш-функции из Boost для набора значений.
   *
   * Количество коллизий пре load factor меньшем 1 растет квадратично, далее линейно.
   * Данная хэш-функция достаточно равномерна, большинство баектов имеют 0-3 элемента.
   *
   * Результаты измерений в папке results.
   */


  std::unordered_set < Customer, Customer_Hash, Customer_Equal > customers;

  const float max_load_factor = 2;
  const size_t reserved_size = 1000000; // min num of elements to insert
  size_t save_step = 100;  // skip interval between collision savings

  customers.max_load_factor(max_load_factor);
  customers.reserve(reserved_size);

  std::filesystem::create_directory("../results");
  std::fstream file{};
  file.open("../results/collisions_test.csv", std::ios::out);
  const size_t max_load_size = max_load_factor*customers.bucket_count();
  for (size_t i{}; i<max_load_size; ++i){
    if (i%(max_load_size/100) == 0){
      std::cout << "Progress: " << std::setprecision(2) << static_cast<float>(i*100)/max_load_size << "%\n";
    }

    if (i%save_step == 0){
      file << customers.size() << ',' << collisions_number(customers) << ',' << customers.load_factor() << '\n';
    }
    customers.insert(random_customer());
  }
  file.close();

  file.open("../results/uniform_test.csv", std::ios::out);
  for (size_t i{}; i<customers.bucket_count()-1; ++i){
    file << customers.bucket_size(i) << ',';
  }
  file << customers.bucket_size(customers.bucket_count()-1);
  file.close();

  return EXIT_SUCCESS;
}