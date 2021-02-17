#ifndef INC_4_SEM_CS_HM_2_CONTAINERS_SPEEDTEST_TABLE_H_
#define INC_4_SEM_CS_HM_2_CONTAINERS_SPEEDTEST_TABLE_H_

#include <string>
#include <unordered_map>
#include <utility>

#include "DataFrame.h"

class Table : public DataFrame {
 public:
  using string = std::string;
  using index_map_t = std::unordered_map<string, size_t>;
  Table(const std::vector<string>& row_names,
        const std::vector<string>& col_names):
        DataFrame(row_names.size()+1, col_names.size()+1) {
    for (int i{}; i < row_names.size(); ++i) {
      row_name_to_index.insert({row_names[i], i + 1});
      DataFrame::set_element(i + 1, 0, row_names[i]);
    }
    for (int i{}; i < col_names.size(); ++i) {
      col_name_to_index.insert({col_names[i], i + 1});
      DataFrame::set_element(0, i + 1, col_names[i]);
    }
  }

  void set_element(const string& row, const string& col, const string& item){
    DataFrame::set_element(row_name_to_index[row], col_name_to_index[col], item);
  }

  string get_element(const string& row, const string& col){
    return DataFrame::get_element(row_name_to_index[row], col_name_to_index[col]);
  }

 private:
  index_map_t row_name_to_index{};
  index_map_t col_name_to_index{};
};

#endif //INC_4_SEM_CS_HM_2_CONTAINERS_SPEEDTEST_TABLE_H_
