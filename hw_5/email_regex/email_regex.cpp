#include <iostream>
#include <regex>
#include <string>
#include <fstream>
#include <sstream>


int main(int argc, char **argv){
  std::ifstream in("../input.txt");
  std::ofstream out("../output.txt");
  std::cout.rdbuf(out.rdbuf()); // redirect std::cout to output.txt

  std::stringstream str_stream;
  str_stream << in.rdbuf();
  std::string user_str = str_stream.str();

  std::cout << "Found email domains:\n";
  std::regex pattern(R"(([\-\w]+)@(\w+[_\-\w]*\w+(\.[[:alpha:]]+)+))");

  std::sregex_iterator begin(user_str.cbegin(), user_str.cend(), pattern);
  std::sregex_iterator end;

  std::for_each(begin, end, [](const std::smatch & m)
  {
    std::cout << "Found e-mail: " << m[0] << ", domain: " << m[2] << '\n';
  });

  return EXIT_SUCCESS;
}