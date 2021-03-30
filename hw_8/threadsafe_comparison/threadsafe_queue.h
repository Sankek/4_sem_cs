#ifndef STACK_THREADSAFE_COMPARISON_THREADSAFE_QUEUE_H_
#define STACK_THREADSAFE_COMPARISON_THREADSAFE_QUEUE_H_

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

template < typename T >
class threadsafe_queue
{
 public:

  threadsafe_queue() = default;

  threadsafe_queue(const threadsafe_queue & other)
  {
    std::lock_guard < std::mutex > lock(other.m_mutex);
    m_queue = other.m_queue;
  }

  threadsafe_queue & operator=(const threadsafe_queue & other)
  {
    std::scoped_lock(m_mutex, other.m_mutex);
    m_queue = other.m_queue;
  }

 public:

  void push(T value)
  {
    std::lock_guard < std::mutex > lock(m_mutex);
    m_queue.push(value);
    m_condition_variable.notify_one();
  }

  void wait_and_pop(T & value)
  {
    std::unique_lock < std::mutex > lock(m_mutex);

    m_condition_variable.wait(lock, [this] {return !m_queue.empty(); });
    value = m_queue.front();
    m_queue.pop();
  }

  bool empty() const
  {
    std::lock_guard < std::mutex > lock(m_mutex);
    return m_queue.empty();
  }

 private:

  std::queue < T >		m_queue;
  std::condition_variable m_condition_variable;

 private:

  mutable std::mutex m_mutex;
};


#endif //STACK_THREADSAFE_COMPARISON_THREADSAFE_QUEUE_H_
