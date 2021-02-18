#ifndef INC_4_SEM_CS_HM_1_TIMER_H_
#define INC_4_SEM_CS_HM_1_TIMER_H_

#include <chrono>

template<class Duration>
class Timer{
 public:
  using Clock_t = std::chrono::steady_clock;
  using time_point_t = Clock_t::time_point;
  Timer() = default;
  ~Timer() = default;

  void start(){
    m_begin = Clock_t::now();
  }

  void stop(){
    m_end = Clock_t::now();
  }

  int_fast64_t result(){
    return std::chrono::duration_cast<Duration> (m_end-m_begin).count();
  }

 private:
  time_point_t m_begin{};
  time_point_t m_end{};

};

#endif //INC_4_SEM_CS_HM_1_TIMER_H_
