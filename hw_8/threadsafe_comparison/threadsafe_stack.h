#ifndef STACK_THREADSAFE_COMPARISON_THREADSAFE_STACK_H_
#define STACK_THREADSAFE_COMPARISON_THREADSAFE_STACK_H_

#include <exception>
#include <memory>
#include <mutex>
#include <stack>
#include <stdexcept>

template < typename T >
class threadsafe_stack
{
 public:

  threadsafe_stack() = default;

  threadsafe_stack(const threadsafe_stack & other)
  {
    std::lock_guard < std::mutex > lock(other.m_mutex);
    m_data = other.m_data;
  }

  threadsafe_stack& operator=(const threadsafe_stack &) = delete;

 public:

  void push(T value)
  {
    std::lock_guard < std::mutex > lock(m_mutex);
    m_data.push(value);
    m_condition_variable.notify_one();
  }

  void wait_and_pop(T & value)
  {
    std::unique_lock < std::mutex > lock(m_mutex);

    m_condition_variable.wait(lock, [this] {return !m_data.empty(); });
    value = m_data.top();
    m_data.pop();
  }

  bool empty() const
  {
    std::lock_guard < std::mutex > lock(m_mutex);
    return m_data.empty();
  }

 private:

  std::stack < T > m_data;
  std::condition_variable m_condition_variable;

 private:

  mutable std::mutex m_mutex;
};

#endif //STACK_THREADSAFE_COMPARISON_THREADSAFE_STACK_H_
