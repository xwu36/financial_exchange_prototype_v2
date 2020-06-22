/**
 * Demo for using Benchmarking using Google's benchmark platform:
 * https://github.com/google/benchmark
 */
#include <benchmark/benchmark.h>

#include <string>

unsigned long Increment(unsigned long n) {
  unsigned long sum = 0;
  for (unsigned long i = 0; i < n; i++) {
    benchmark::DoNotOptimize(sum++);
    benchmark::ClobberMemory();
  }
  return sum;
}

static void BM_Increment(benchmark::State& state) {
  // Perform setup here
  for (auto _ : state) {
    // This code gets timed
    Increment(1000);
  }
}
// Register the function as a benchmark
BENCHMARK(BM_Increment);

// BENCHMARK(BM_Increment)->RangeMultiplier(2)->Range(1 << 8, 1 << 18);

// Run the benchmark
BENCHMARK_MAIN();