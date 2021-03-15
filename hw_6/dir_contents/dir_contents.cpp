#include <iostream>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <utility>

namespace fs = std::filesystem;

std::string truncate(const std::string &str, size_t width, bool show_ellipsis=true)
{
  if (str.length() > width){
    if (show_ellipsis){
      return str.substr(0, width-3) + "...";
    } else {
      return str.substr(0, width);
    }
  }

  return str;
}

struct FileData{
  fs::path path;
  fs::path relative_to;
  fs::path name;
  fs::path extension;
  fs::file_time_type last_modified;
  std::vector<size_t> ws{};

  FileData(const fs::path &p, fs::path rel,
           std::vector<size_t>  widths) :  path(p),
                                           relative_to(std::move(rel)),
                                           name(p.stem()),
                                           extension(p.extension()),
                                           last_modified(fs::last_write_time(p)),
                                           ws(std::move(widths)){}

};

std::ostream& operator<<(std::ostream &out, const FileData &file_data){
  // FIXME: wait C++20 for adequate filesystem-time solution
  std::time_t t = (file_data.last_modified.time_since_epoch().count()-__std_fs_file_time_epoch_adjustment)/10000000;

  out <<         std::left << std::setw(file_data.ws[0]) << truncate(fs::relative(file_data.path, file_data.relative_to).generic_string(), file_data.ws[0])
      << ' '  << std::left << std::setw(file_data.ws[1]) << truncate(file_data.name.generic_string(), file_data.ws[1])
      << ' '  << std::left << std::setw(file_data.ws[2]) << truncate(file_data.extension.generic_string(), file_data.ws[2])
      << ' '  << std::left << std::put_time(localtime(&t), "%d.%m.%Y %T");
  return out;
}


void write_contents(std::ostream &out, const fs::path &path, const std::vector<size_t>& widths={40, 20, 10}){
  if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path)){
    throw std::exception("Directory doesn't exist.");
  }
  out <<  "Contents of \""<< fs::absolute(path).generic_string() << "\":\n";
  out <<        std::left << std::setw(widths[0]) << "Path"
      << ' ' << std::left << std::setw(widths[1]) << "Name"
      << ' ' << std::left << std::setw(widths[2]) << "Extension"
      << ' ' << std::left << "Last modified\n";
  if (std::filesystem::exists(path) && std::filesystem::is_directory(path))
  {
    for (auto const & entry : std::filesystem::recursive_directory_iterator(path))
    {
      if (std::filesystem::is_regular_file(entry.status()) || std::filesystem::is_symlink(entry.status()))
      {
        try{
          out << FileData(entry.path(), path, widths) << '\n';
        } catch (std::exception &msg){
          out <<         std::left << std::setw(widths[0]) << "???"
              << ' '  << std::left << std::setw(widths[1]) << "???"
              << ' '  << std::left << std::setw(widths[2]) << "???"
              << ' '  << std::left << "???\n";

          std::cerr << msg.what() << '\n';
        }
      }
    }
  }
}

int main(int argc, char** agrv) {
  std::ofstream file("../output.txt", std::ios::out);
  fs::path path = R"(D:\Projects\Cpp\4_sem_cs\hw_6)";

  try{
    write_contents(file, path);
  } catch (std::exception &msg){
    std::cerr << msg.what() << '\n';
  }

  return EXIT_SUCCESS;
}