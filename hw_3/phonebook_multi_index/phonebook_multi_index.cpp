#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>

using namespace boost::multi_index;

struct Phonebook{
  std::string surname{};
  std::string name{};
  std::string phone{};
  bool has_dog{};
};

using phonebook_multi_index = multi_index_container <
    Phonebook, indexed_by <
        ordered_non_unique <
            member < Phonebook, std::string, &Phonebook::surname > >,
        random_access <>,
        hashed_non_unique <
            member < Phonebook, std::string, &Phonebook::phone > > > >;

int main(int argc, char **argv){
  std::ofstream out("../output.txt");
  std::cout.rdbuf(out.rdbuf()); // redirect std::cout to output.txt

  phonebook_multi_index phonebook;

  phonebook.insert({ "Izyurov", "Alexander",    "88005553535"});
  phonebook.insert({ "Polezhaykin", "Alexander",    "84454458282", true});
  phonebook.insert({ "Nasakin", "Arthur",    "89315554545" });
  phonebook.insert({ "Navalniy", "Leha",    "89315554545"});
  phonebook.insert({ "Ivanov", "Kirill",    "unknown", true});
  phonebook.insert({ "Izyurov", "Sankek",    "89220317777" });
  phonebook.insert({ "Korobochkin", "Alexander",    "89998887766" });
  phonebook.insert({ "Papasha", "Semen",    "84217535594", true});
  phonebook.insert({ "Abramova", "Sophia",    "84869096901", true});
  phonebook.insert({ "Bogdanov", "Roman",    "80539033241" });
  phonebook.insert({ "Chel", "Kakoito",    "84217535594"});
  phonebook.insert({ "Guseva", "Nika",    "88945380335", true});
  phonebook.insert({ "Gavrilov", "Artem",    "85985549755", true});
  phonebook.insert({ "Differential", "Mixwell",    "80000000001"});
  phonebook.insert({ "Zhuravlev", "Michail",    "84728698633" });

  std::cout << "PRINTING HOUSE USE\n";
  const auto & ordered_surnames_index = phonebook.get < 0 > ();

  auto begin = ordered_surnames_index.begin();
  auto end = ordered_surnames_index.end();
  for (; begin != end; ++begin){
  	std::cout << std::left << std::setw(20) << begin->surname << ' '
  	          << std::left << std::setw(20) << begin->name << ' '
  	          << std::left << std::setw(20) << begin->phone << '\n';
  }

  std::cout << '\n';
  std::cout << "ADVERTISING AGENCY USE\n";

  const auto & random_access_index = phonebook.get < 1 > ();
  std::cout << "Some people that have a dog:\n";
  for (size_t i{}; i<random_access_index.size(); i+=2){
    if (random_access_index[i].phone == "unknown" ||
        !random_access_index[i].has_dog){
      continue;
    }
    std::cout << i+1 << ". " << random_access_index[i].name
                     << ' ' << random_access_index[i].phone << '\n';
  }

  std::cout << '\n';
  std::cout << "REGULAR USER USE\n";

  auto & hashed_phones_index = phonebook.get<2> ();
  const std::string number = "84217535594";
  std::cout << "People with number " << number << ":\n";
  auto person_range = hashed_phones_index.equal_range(number);
  for (auto p{person_range.first}; p != person_range.second; ++p){
    std::cout << p->name << ' ' << p->surname << '\n';
  }

  return 0;
}