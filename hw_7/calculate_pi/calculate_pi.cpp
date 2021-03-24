#include <iostream>
#include <random>
#include <thread>
#include <future>


auto sequential_algorithm (uint64_t N) {
  std::uniform_real_distribution < double > distribution(0.0, 2.0);
  std::random_device rd;
  std::mt19937 engine(rd());
  std::vector < double > x (N);
  std::vector < double > y (N);
  std::generate(std::begin(x), std::end(x), [&engine, &distribution](){return distribution(engine);});
  std::generate(std::begin(y), std::end(y), [&engine, &distribution](){return distribution(engine);});
  std::uint64_t counter {};
  double dx, dy;
  for (size_t i = 0; i < N; ++i)
  {
    dx = x[i] - 1.0;
    dy = y[i] - 1.0;
    if (dx*dx + dy*dy <= 1.0)
    {
      counter++;
    }
  }
  return 4*counter / static_cast < double > (N);
}

auto parallel_algorithm (uint64_t N) {
  const std::uint64_t min_per_thread = 25;
  const std::uint64_t max_threads =
      (N + min_per_thread - 1) / min_per_thread;
  const std::uint64_t hardware_threads =
      std::thread::hardware_concurrency();
  const std::uint64_t num_threads =
      std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
  const std::uint64_t block_size = N / num_threads;
  std::vector < std::future < double > > futures(num_threads - 1);

  for (auto i = 0U; i < num_threads-1; i++)
  {
    futures[i] = std::async(sequential_algorithm, block_size);
  }
  auto last_N = N - block_size*(num_threads-1);
  auto last_res = last_N * sequential_algorithm(last_N);
  for (auto i = 0U; i < num_threads-1; i++)
  {
    last_res += block_size * futures[i].get();
  }
  return last_res / static_cast < double > (N);
}

int main() {
  std::cout << "Sequential algorithm working:\n";
  std::cout << sequential_algorithm(100000000) << std::endl;
  std::cout << "Parallel algorithm working:\n";
  std::cout << parallel_algorithm(100000000) << std::endl;
  system("pause");
  return EXIT_SUCCESS;
}