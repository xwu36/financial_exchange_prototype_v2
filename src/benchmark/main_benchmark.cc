/**
 * Demo for using Benchmarking using Google's benchmark platform:
 * https://github.com/google/benchmark
 */
#include <benchmark/benchmark.h>

#include <chrono>
#include <string>

void SomeFunction(size_t size) {
  std::string s1(size, '-');
  std::string s2(size, '-');
  benchmark::DoNotOptimize(s1.compare(s2));
}

static void BM_SomeFunction(benchmark::State& state) {
  // Perform setup here
  for (auto _ : state) {
    // This code gets timed
    SomeFunction(1000);
  }
}
// Register the function as a benchmark
BENCHMARK(BM_SomeFunction);
// Run the benchmark
BENCHMARK_MAIN();