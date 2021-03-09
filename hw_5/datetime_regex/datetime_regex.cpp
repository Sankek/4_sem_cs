#include <iostream>
#include <string>
#include <regex>

int main(int argc, char **argv){

  // date format: DD.MM.YYYY, time format: HH:MM:SS
  std::cout << "Enter some text to find dates and times:\n";
  while(!std::cin.fail()){
    std::string user_str;
    std::getline(std::cin, user_str);
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

  }

  system("pause");

  return EXIT_SUCCESS;
}
