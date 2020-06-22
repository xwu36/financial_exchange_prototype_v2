/**
 * Demo for using Benchmarking using Google's benchmark platform:
 * https://github.com/google/benchmark
 */
#include <benchmark/benchmark.h>
#include <stdlib.h> /* qsort */

#include <cstdlib>
#include <iostream>
#include <numeric>
#include <string>
#include <thread>
#include <vector>

#include "src/lib/search/search.h"

const long g_size = 10000000000;
template <class T>
int Search<T>::number_of_threads = 10;

class Data {
 public:
  Data(long size) { ConstructSortedVector(size); }

  void ConstructRandomVector(long size) {
    std::srand(10);
    v.resize(size);
    std::generate(v.begin(), v.end(), std::rand);
    std::sort(v.begin(), v.end());
  }

  void ConstructSortedVector(long size) {
    v.resize(size);
    std::iota(v.begin(), v.end(), 0);
  }

  std::vector<unsigned long> v;
};

Data init(long size = g_size) {
  Data d(size);
  return d;
}

static void BM_BinarySearch(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto d = init(state.range(0));
    state.ResumeTiming();
    Search<unsigned long>::BinarySearch(d.v, d.v[d.v.size() - 1]);
  }
  state.SetItemsProcessed(state.iterations() * state.range(0));
  state.SetComplexityN(state.range(0));
}

static void BM_ExponentialSearch(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto d = init(state.range(0));
    state.ResumeTiming();
    Search<unsigned long>::ExponentialSearch(d.v, d.v[d.v.size() - 1]);
  }
  state.SetItemsProcessed(state.iterations() * state.range(0));
  state.SetComplexityN(state.range(0));
}

static void BM_TernarySearch(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto d = init(state.range(0));
    state.ResumeTiming();
    Search<unsigned long>::TernarySearch(d.v, d.v[d.v.size() - 1]);
  }
  state.SetItemsProcessed(state.iterations() * state.range(0));
  state.SetComplexityN(state.range(0));
}

static void BM_BinarySearchPar(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto d = init(state.range(0));
    Search<unsigned long>::number_of_threads = state.range(1);

    state.ResumeTiming();
    Search<unsigned long>::BinarySearchPar(d.v, d.v[d.v.size() - 1]);
  }
  state.SetItemsProcessed(state.iterations() * state.range(0));
  state.SetComplexityN(state.range(0));
}

// BENCHMARK(BM_BinarySearchPar)
//     ->RangeMultiplier(2)
//     ->Ranges({{g_size, g_size}, {1, 16}})
//     ->UseRealTime();

// Compare binary search, ternary search, and exponential search
// BENCHMARK(BM_BinarySearch)
//     ->RangeMultiplier(2)
//     ->Range(1 << 8, 1 << 18)
//     ->Complexity();

// BENCHMARK(BM_ExponentialSearch)
//     ->RangeMultiplier(2)
//     ->Range(1 << 8, 1 << 18)
//     ->Complexity();

// BENCHMARK(BM_BinarySearchPar)
//     ->RangeMultiplier(2)
//     ->Ranges({{1 << 8, 1 << 18}, {5, 5}})
//     ->Complexity();

// BENCHMARK(BM_TernarySearch)
//     ->RangeMultiplier(2)
//     ->Range(1 << 8, 1 << 18)
//     ->Complexity();

// BENCHMARK(BM_BinarySearch)->Arg(g_size);
// BENCHMARK(BM_TernarySearch)->Arg(g_size);

// BENCHMARK(BM_BinarySearchPar)->Arg(g_size);
BENCHMARK(BM_BinarySearchPar)
    ->RangeMultiplier(2)
    ->Ranges({{g_size, g_size}, {1, 16}})
    ->UseRealTime();

BENCHMARK_MAIN();