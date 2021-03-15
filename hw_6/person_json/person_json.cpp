#include <iostream>
#include <filesystem>
#include <fstream>

#include "json.hpp"

using nlohmann::json;


/*
 * see https://stackoverflow.com/questions/7302996/changing-the-delimiter-for-cin-c
 */
struct CommaSeparator : std::ctype<char> {
  CommaSeparator() : std::ctype<char>(get_table()) {}
  static mask const* get_table()
  {
    static mask rc[table_size];
    rc[','] = std::ctype_base::space;
    rc['\n'] = std::ctype_base::space;
    return &rc[0];
  }
};

template<class S>
void change_separator(std::istream &ios){
  ios.imbue(std::locale(ios.getloc(), new S));
}

struct Person{
  std::string name{};
  std::string surname{};
  int age{};
  int height{};
  std::string phone{};
  std::string address{};
  std::pair<double, double> coordinates{};
};

std::istream& operator>>(std::istream &in, Person &p){
  in >> p.name
     >> p.surname
     >> p.age
     >> p.height
     >> p.phone
     >> p.address
     >> p.coordinates.first
     >> p.coordinates.second;

  return in;
}

std::ostream& operator<<(std::ostream &out, const Person &p){
  out << p.name << ','
      << p.surname << ','
      << p.age << ','
      << p.height << ','
      << p.phone << ','
      << p.address << ','
      << p.coordinates.first << ','
      << p.coordinates.second;

  return out;
}


int main(int argc, char** agrv) {
  const std::filesystem::path save_path = "../persons";
  std::filesystem::create_directory(save_path);

  std::ifstream input_file("../person_raw.txt", std::ios::in);
  change_separator<CommaSeparator>(input_file);

  Person person;
  while(input_file >> person){
    json j;

    j["name"] = person.name;
    j["surname"] = person.surname;
    j["age"] = person.age;
    j["height"] = person.height;
    j["phone"] = person.phone;
    j["address"] = person.address;
    j["coordinates"] = {person.coordinates.first, person.coordinates.second};

    std::string save_name = person.surname + "_" + person.name;
    size_t save_try = 1;
    while (std::filesystem::exists(save_path/(save_name+".json"))){
      json check_j;
      std::ifstream check_file(save_path/(save_name+".json"), std::ios::in);
      check_file >> check_j;
      if (j == check_j){
        break;
      }
      save_name = person.surname+"_"+person.name + "_" + std::to_string(save_try++);
    }

    std::ofstream output_file("../persons/" + save_name + ".json", std::ios::out);
    output_file << j.dump(2);
  }

  return EXIT_SUCCESS;
}