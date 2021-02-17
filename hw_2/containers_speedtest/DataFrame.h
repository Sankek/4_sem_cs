#ifndef INC_4_SEM_CS_HM_2_CONTAINERS_SPEEDTEST_DATAFRAME_H_
#define INC_4_SEM_CS_HM_2_CONTAINERS_SPEEDTEST_DATAFRAME_H_

#include <vector>
#include <string>
#include <fstream>

class DataFrame{
 public:
  using row_t = std::vector<std::string>;
  using data_t = std::vector<row_t>;

  DataFrame(size_t rows, size_t cols){
    data.resize(rows);
    for (auto &r : data){
      r.resize(cols, "");
    }
  }

  void to_csv (const std::string& path, char delimiter = ','){
    std::fstream file;
    file.open(path, std::ofstream::out);

    for (size_t m{}, n{}; m<data.size(); ++m){
      for (; n<data[m].size()-1; ++n){
        file << data[m][n] << ',';
      }
      file << data[m][n] << '\n';
      n = 0;
    }
  }

  void set_element(size_t row, size_t col, const std::string& item){
    data[row][col] = item;
  }

  std::string get_element(size_t row, size_t col){
    return data[row][col];
  }

 private:
  data_t data{};
};

#endif //INC_4_SEM_CS_HM_2_CONTAINERS_SPEEDTEST_DATAFRAME_H_
