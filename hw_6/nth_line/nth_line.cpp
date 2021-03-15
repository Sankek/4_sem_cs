#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

int main(int argc, char** agrv){

  std::ifstream file;
  // std::ios::binary flag to save CR to stream in Windows
  file.open("../input.txt", std::ios::in | std::ios::binary);

  std::string str;
  size_t lines_num{};
  while(std::getline(file, str)){
    lines_num++;
  }
  file.clear();

  std::cout << "Enter line index from 0 to " << (lines_num > 0 ? lines_num-1 : 0) << ":\n";
  size_t index{};
  std::cin >> index;

  file.seekg(0);
  std::getline(file, str);
  const size_t line_size = str.size()+1;

  file.seekg(index*line_size);
  std::string str2;
  std::getline(file, str2);

  std::cout << "Line with index " << index << ":\n";
  std::cout << str2 << '\n';

  return EXIT_SUCCESS;
}