/**
 * Demo for using Benchmarking using Google's benchmark platform:
 * https://github.com/google/benchmark
 */
#include <benchmark/benchmark.h>
#include <stdlib.h> /* qsort */

#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <numeric>

#include "src/lib/search/search.h"

const int g_size = 1000000000;

class Data {
 public:
  Data(int size) { ConstructSortedVector(size); }

  void ConstructRandomVector(int size) {
    std::srand(10);
    v.resize(size);
    std::generate(v.begin(), v.end(), std::rand);
    std::sort(v.begin(), v.end());
  }

  void ConstructSortedVector(int size) {
    v.resize(size);
    std::iota(v.begin(), v.end(), 0);
  }

  std::vector<int> v;
};

Data init(int size = g_size) {
  Data d(size);

  return d;
}

static void BM_BinarySearch(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto d = init(state.range(0));
    state.ResumeTiming();
    Search::BinarySearch(d.v, d.v[1]);
  }
  // state.SetItemsProcessed(state.iterations() * state.range(0));
  // state.SetComplexityN(state.range(0));
}

static void BM_TernarySearch(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto d = init(state.range(0));
    state.ResumeTiming();
    Search::TernarySearch(d.v, d.v[1]);
  }
  // state.SetItemsProcessed(state.iterations() * state.range(0));
  // state.SetComplexityN(state.range(0));
}

static void BM_BinarySearchPar(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto d = init(state.range(0));
    state.ResumeTiming();
    Search::BinarySearchPar(d.v, d.v[1]);
  }
  // state.SetItemsProcessed(state.iterations() * state.range(0));
  // state.SetComplexityN(state.range(0));
}

// BENCHMARK(BM_BinarySearch)
//     ->RangeMultiplier(2)
//     ->Range(1 << 10, 1 << 15)
//     ->Complexity();
// ;

// BENCHMARK(BM_BinarySearchPar)
//     ->RangeMultiplier(2)
//     ->Range(1 << 10, 1 << 15)
//     ->Complexity();
// ;

BENCHMARK(BM_BinarySearch)->Arg(g_size);
BENCHMARK(BM_TernarySearch)->Arg(g_size);

BENCHMARK(BM_BinarySearchPar)->Arg(g_size)->UseRealTime();

// BENCHMARK(BM_QuickSortPar)
//     ->RangeMultiplier(2)
//     ->Range(1 << 10, 1 << 23)
//     ->Complexity();
// ;

// static void BM_StdParSort(benchmark::State& state) {
//   for (auto _ : state) {
//     state.PauseTiming();
//      auto d = init(state.range(0));
//     state.ResumeTiming();
//     std::sort(std::execution::par_unseq, d.v.begin(), d.v.end());

//     std::sort(d.v.begin(), d.v.end());
//   }
//   state.SetItemsProcessed(state.iterations() * g_size);
// }

// Register the function as a benchmark
// BENCHMARK(BM_StdSort)
//     ->RangeMultiplier(2)
//     ->Range(1 << 10, 1 << 18)
//     ->Complexity();
// ;

BENCHMARK_MAIN();