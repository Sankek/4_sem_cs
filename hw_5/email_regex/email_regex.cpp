#include <iostream>
#include <regex>
#include <string>


int main(int argc, char **argv){

//  const std::string user_str = "EMAIL: my_email228@phystech.edu, чтож@да.нет?, new_email = \"name.surname@mipt.ru\" ";

  std::cout << "Enter some text to find email domains:\n";
  while(!std::cin.fail()){
    std::string user_str;
    std::getline(std::cin, user_str);
    std::regex pattern(R"(([\-\w]+)@([\-\w]+\.[_\-[:alpha:]]+))");


    std::sregex_iterator begin(user_str.cbegin(), user_str.cend(), pattern);
    std::sregex_iterator end;

    std::for_each(begin, end, [](const std::smatch & m)
    {
      std::cout << "Found e-mail: " << m[0] << ", domain: " << m[2] << '\n';
    });
  }

  system("pause");

  return EXIT_SUCCESS;
}