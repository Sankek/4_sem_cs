#include <iostream>
#include <algorithm>
#include <string>
#include <random>
#include <fstream>
#include <filesystem>
#include <unordered_set>

std::string CHARACTERS { "abcdefghijklmnopqrstuvwxyz" };

static std::random_device random_device;
static std::mt19937 rng{random_device()};
static std::uniform_int_distribution<size_t> uniform_char_index(0, CHARACTERS.size()-1);

std::string random_string(size_t size){
  std::string str{};
  str.resize(size);
  std::generate(str.begin(), str.end(), [](){return CHARACTERS[uniform_char_index(rng)];});
  return str;
}

unsigned int RSHash(const std::string &str)
{
  unsigned int b    = 378551;
  unsigned int a    = 63689;
  unsigned int hash = 0;
  unsigned int i    = 0;

  for (i = 0; i < str.size(); ++i)
  {
    hash = hash * a + str[i];
    a    = a * b;
  }

  return hash;
}

unsigned int JSHash(const std::string &str)
{
  unsigned int hash = 1315423911;
  unsigned int i    = 0;

  for (i = 0; i < str.size(); ++i)
  {
    hash ^= ((hash << 5) + str[i] + (hash >> 2));
  }

  return hash;
}

unsigned int PJWHash(const std::string &str)
{
  const auto BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
  const auto ThreeQuarters     = (unsigned int)((BitsInUnsignedInt  * 3) / 4);
  const auto OneEighth         = (unsigned int)(BitsInUnsignedInt / 8);
  const unsigned int HighBits          =
      (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
  unsigned int hash = 0;
  unsigned int test = 0;
  unsigned int i    = 0;

  for (i = 0; i < str.size(); ++i)
  {
    hash = (hash << OneEighth) + str[i];

    if ((test = hash & HighBits) != 0)
    {
      hash = (( hash ^ (test >> ThreeQuarters)) & (~HighBits));
    }
  }

  return hash;
}

unsigned int ELFHash(const std::string &str)
{
  unsigned int hash = 0;
  unsigned int x    = 0;
  unsigned int i    = 0;

  for (i = 0; i < str.size(); ++i)
  {
    hash = (hash << 4) + str[i];

    if ((x = hash & 0xF0000000L) != 0)
    {
      hash ^= (x >> 24);
    }

    hash &= ~x;
  }

  return hash;
}

unsigned int BKDRHash(const std::string &str)
{
  unsigned int seed = 131; /* 31 131 1313 13131 131313 etc.. */
  unsigned int hash = 0;
  unsigned int i    = 0;

  for (i = 0; i < str.size(); ++i)
  {
    hash = (hash * seed) + str[i];
  }

  return hash;
}

unsigned int SDBMHash(const std::string &str)
{
  unsigned int hash = 0;
  unsigned int i    = 0;

  for (i = 0; i < str.size(); ++i)
  {
    hash = str[i] + (hash << 6) + (hash << 16) - hash;
  }

  return hash;
}

unsigned int DJBHash(const std::string &str)
{
  unsigned int hash = 5381;
  unsigned int i    = 0;

  for (i = 0; i < str.size(); ++i)
  {
    hash = ((hash << 5) + hash) + str[i];
  }

  return hash;
}

unsigned int DEKHash(const std::string &str)
{
  unsigned int hash = str.size();
  unsigned int i    = 0;

  for (i = 0; i < str.size(); ++i)
  {
    hash = ((hash << 5) ^ (hash >> 27)) ^ str[i];
  }

  return hash;
}

unsigned int APHash(const std::string &str)
{
  unsigned int hash = 0xAAAAAAAA;
  unsigned int i    = 0;

  for (i = 0; i < str.size(); ++i)
  {
    hash ^= ((i & 1) == 0) ? (  (hash <<  7) ^ str[i] * (hash >> 3)) :
            (~((hash << 11) + (str[i] ^ (hash >> 5))));
  }

  return hash;
}


template<size_t HashFunctionNumber>
struct StringHash{
  unsigned int operator()(const std::string& str) const noexcept{
    switch (HashFunctionNumber) {
      case 0: return RSHash(str);
      case 1: return JSHash(str);
      case 2: return PJWHash(str);
      case 3: return ELFHash(str);
      case 4: return BKDRHash(str);
      case 5: return SDBMHash(str);
      case 6: return DJBHash(str);
      case 7: return DEKHash(str);
      case 8: return APHash(str);
      case 9: return std::hash<std::string>()(str);
      default: return 0;
    }
  }

};

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

template<size_t HashFunctionNumber>
void TestHashFunction(const std::string& hash_function_name){
  std::cout << "Testing " << hash_function_name << "...\n";
  std::unordered_set < std::string, StringHash<HashFunctionNumber> > hash_table{};

  const size_t string_size = 5;
  const float max_load_factor = 2.0f;
  const size_t reserved_size = 1000000; // min num of elements to insert
  size_t save_step = 100;  // skip interval between collision savings

  hash_table.max_load_factor(max_load_factor);
  hash_table.reserve(reserved_size);

  std::filesystem::create_directory("../data");
  std::fstream file{};
  file.open("../data/collisions_test_" + hash_function_name + ".csv", std::ios::out);
  const size_t max_load_size = max_load_factor*hash_table.bucket_count();
  for (size_t i{}; i<max_load_size; ++i){
    if (i%(max_load_size/10) == 0){
      std::cout << "Progress: " << (i*100)/max_load_size << "%\n";
    }

    if (i%save_step == 0){
      file << hash_table.size() << ',' << collisions_number(hash_table) << ',' << hash_table.load_factor() << '\n';
    }
    hash_table.insert(random_string(string_size));
  }
  file.close();

  file.open("../data/uniform_test_" + hash_function_name + ".csv", std::ios::out);
  for (size_t i{}; i<hash_table.bucket_count()-1; ++i){
    file << hash_table.bucket_size(i) << ',';
  }
  file << hash_table.bucket_size(hash_table.bucket_count()-1);
  file.close();
}

int main(int argc, char **argv){

  /*
   *  В этом задании исследуются 9 хэш-функций из статьи
   *  http://www.partow.net/programming/hashfunctions/#AvailableHashFunctions
   *  на равномерность и зависимость количества коллизий от количества элементов в хэш-таблице.
   *
   *  Полученные результаты получились красивыми и наглядными.
   *  (DEKHash не очень)
   */

  TestHashFunction<0>("RSHash");
  TestHashFunction<1>("JSHash");
  TestHashFunction<2>("PJWHash");
  TestHashFunction<3>("ELFHash");
  TestHashFunction<4>("BKDRHash");
  TestHashFunction<5>("SDBMHash");
  TestHashFunction<6>("DJBHash");
  TestHashFunction<7>("DEKHash");
  TestHashFunction<8>("APHash");
  TestHashFunction<9>("STLHash");

  return EXIT_SUCCESS;
}