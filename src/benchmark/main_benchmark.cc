/**
 * Demo for using Benchmarking using Google's benchmark platform:
 * https://github.com/google/benchmark
 */
#include <benchmark/benchmark.h>

#include <string>

void SomeFunction(int a, int b){
  // ...
}

static void BM_SomeFunction(benchmark::State& state) {
  // Perform setup here
  for (auto _ : state) {
    // This code gets timed
    std::string s1(state.range(0), '-');
    std::string s2(state.range(0), '-');
    benchmark::DoNotOptimize(s1.compare(s2));
  }
}
// Register the function as a benchmark
BENCHMARK(BM_SomeFunction);
// Run the benchmark
BENCHMARK_MAIN();