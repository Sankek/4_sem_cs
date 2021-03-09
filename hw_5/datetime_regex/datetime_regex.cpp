#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <sstream>


int main(int argc, char **argv){
  std::ifstream in("../input.txt");
  std::ofstream out("../output.txt");
  std::cout.rdbuf(out.rdbuf()); // redirect std::cout to output.txt

  std::stringstream str_stream;
  str_stream << in.rdbuf();
  std::string user_str = str_stream.str();

  // date format: DD.MM.YYYY, time format: HH:MM:SS
  std::cout << "Found dates and times:\n";
  std::regex pattern(R"(((0[1-9]|[12][0-9]|3[01])\.(0[1-9]|1[012])\.([0-9]{4}))|(([01][0-9]|2[0-3]):([0-5][0-9]):([0-5][0-9])))");

  std::sregex_iterator begin(user_str.cbegin(), user_str.cend(), pattern);
  std::sregex_iterator end;

  std::for_each(begin, end, [](const std::smatch & m)
  {
    if (m[1] != ""){
      std::cout << "Found date: " << m[1] << '\n';
    }
    if (m[5] != ""){
      std::cout << "Found time: " << m[5] << '\n';
    }
  });


  return EXIT_SUCCESS;
}
