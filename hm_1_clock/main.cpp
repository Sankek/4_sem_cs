#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>


class Timer{
 public:
  using Clock_t = std::chrono::steady_clock;
  using time_point_t = Clock_t::time_point;
  Timer() : m_begin(Clock_t::now()) {};
  ~Timer(){
    auto end = Clock_t::now();

    std::cout << "milliseconds: " << std::chrono::duration_cast<
        std::chrono::milliseconds> (end-m_begin).count() << '\n';
  }
 private:
  time_point_t m_begin;

};

int main() {

  std::vector v(100000U, 0);

  for (auto i = 0U; i < std::size(v); ++i){
    v[i] = 100000U-i;
  }

  {
    Timer t;
    std::sort(v.begin(), v.end());
  }
  return 0;
}