#ifndef THREADSAVE_PRIORITY_QUEUE_H_
#define THREADSAVE_PRIORITY_QUEUE_H_

#include <queue>
#include <mutex>
#include <condition_variable>

template < typename T >
class threadsafe_priority_queue {
 public:
  threadsafe_priority_queue() = default;

  threadsafe_priority_queue(const threadsafe_priority_queue & other)
    {
      std::lock_guard < std::mutex > lock(other.mutex_);
      priority_queue_ = other.priority_queue_;
    }

  threadsafe_priority_queue & operator=(const threadsafe_priority_queue & other)
    {
      std::scoped_lock(mutex_, other.mutex_);
      priority_queue_ = other.priority_queue_;
    }

  void push(T value)
  {
    std::lock_guard < std::mutex > lock(mutex_);
    priority_queue_.push(value);
    condition_variable_.notify_one();
  }

  void wait_and_pop(T & value)
  {
    std::unique_lock < std::mutex > lock(mutex_);

    condition_variable_.wait(lock, [this] {return !priority_queue_.empty(); });
    value = priority_queue_.top();
    priority_queue_.pop();
  }

  std::shared_ptr < T > wait_and_pop()
  {
    std::unique_lock < std::mutex > lock(mutex_);

    condition_variable_.wait(lock, [this] {return !priority_queue_.empty(); });
    auto result = std::make_shared < T > (priority_queue_.top());
    priority_queue_.pop();

    return result;
  }

  bool try_pop(T & value)
  {
    std::lock_guard < std::mutex > lock(mutex_);

    if (priority_queue_.empty())
    {
      return false;
    }

    value = priority_queue_.top();
    priority_queue_.pop();

    return true;
  }

  std::shared_ptr < T > try_pop()
  {
    std::lock_guard < std::mutex > lock(mutex_);

    if (priority_queue_.empty())
    {
      return std::shared_ptr < T > ();
    }

    auto result = std::make_shared < T > (priority_queue_.top());
    priority_queue_.pop();

    return result;
  }

  bool empty() const
  {
    std::lock_guard < std::mutex > lock(mutex_);
    return priority_queue_.empty();
  }

private:

  std::priority_queue<T> priority_queue_;
  std::condition_variable condition_variable_;
  mutable std::mutex mutex_;
};

#endif //THREADSAVE_PRIORITY_QUEUE_H_
