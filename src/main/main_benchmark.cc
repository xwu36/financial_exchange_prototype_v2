/**
 * Demo for using Benchmarking using Google's benchmark platform:
 * https://github.com/google/benchmark
 */
#include <benchmark/benchmark.h>

#include <iostream>
#include <string>

static void BM_StringCreation(benchmark::State& state) {
  for (auto _ : state) std::string empty_string;
}
static void BM_memcpy(benchmark::State& state) {
  char* src = new char[state.range(0)];
  char* dst = new char[state.range(0)];
  memset(src, 'x', state.range(0));
  for (auto _ : state) memcpy(dst, src, state.range(0));
  state.SetBytesProcessed(int64_t(state.iterations()) *
                          int64_t(state.range(0)));
  delete[] src;
  delete[] dst;
}

static void BM_DenseRange(benchmark::State& state) {
  for (auto _ : state) {
    std::vector<int> v(state.range(0), state.range(0));
    benchmark::DoNotOptimize(v.data());
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_DenseRange)->DenseRange(0, 1024, 128);
BENCHMARK(BM_memcpy)->Range(8, 8 << 16);

BENCHMARK_MAIN();