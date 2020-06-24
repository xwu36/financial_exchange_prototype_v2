/**
 * Demo for using Benchmarking using Google's benchmark platform:
 * https://github.com/google/benchmark
 */
#include <benchmark/benchmark.h>

#include <iostream>
#include <string>

/**
 * Increments n times and returns the value
 */
unsigned long Increment(unsigned long n) {
  unsigned long sum = 0;
  for (unsigned long i = 0; i < n; i++) {
    sum++;
  }
  return sum;
}

/**
 * Adds value n times and returns the result
 */
unsigned long AddByValue(unsigned long n, unsigned long value) {
  unsigned long sum = 0;
  for (unsigned long i = 0; i < n; i++) {
    sum += value;
  }
  return sum;
}

/**
 * A Benchmark for Increment
 */
static void BM_Increment(benchmark::State& state) {
  // Perform setup here
  for (auto _ : state) {
    // This code gets timed
    Increment(state.range(0));
  }
}

/**
 * A Benchmark for AddByValue
 */
static void BM_AddByValue(benchmark::State& state) {
  // Perform setup here
  for (auto _ : state) {
    // This code gets timed
    AddByValue(state.range(0), state.range(1));
  }
}

// Register the function as a benchmark
// BENCHMARK(BM_Increment);

// Using arguments
// BENCHMARK(BM_Increment)->Arg(1000);
// BENCHMARK(BM_Increment)->Arg(2000);
// BENCHMARK(BM_AddByValue)->Args({2000, 2});
// BENCHMARK(BM_Increment)->RangeMultiplier(2)->Range(1 << 8, 1 << 18);
BENCHMARK(BM_Increment)
    ->RangeMultiplier(2)
    ->Ranges({{1 << 8, 1 << 10}, {1, 8}});

// Run the benchmark
BENCHMARK_MAIN();