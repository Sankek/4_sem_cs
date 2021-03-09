#include <iostream>
#include <iomanip>

#include <Windows.h>

std::string currency_symbol(const std::string & locale_name)
{
  return std::use_facet < std::moneypunct < char, false > > (
      std::locale(locale_name)).curr_symbol();
}

int main(int argc, char **argv){

  const double EUR_RUB_COURSE = 88.57;

  SetConsoleCP      (CP_UTF8);
  SetConsoleOutputCP(CP_UTF8);

  std::cout << "EUR ( " << currency_symbol("eu_DE.utf8") << " )"
            << " -> RUB ( " << currency_symbol("eu_DE.utf8") << " )"
            << " CONVERTER\n\n";
  std::cout << "Your value:\n";

  long double user_eur_value;

  std::cout.imbue(std::locale("eu_DE.utf8"));
  std::cin.imbue(std::locale("eu_DE.utf8"));
  std::cin >> std::get_money(user_eur_value, false);

  std::cout << "You entered: ";
  std::cout << std::showbase << std::put_money(user_eur_value, false) << '\n';

  std::cout.imbue(std::locale("ru_RU.utf8"));
  std::cin.imbue(std::locale("ru_RU.utf8"));

  std::cout << "Your value in RUB: ";
  std::cout << std::showbase << std::put_money(user_eur_value*EUR_RUB_COURSE, false) << '\n';

  system("pause");

  return EXIT_SUCCESS;
}