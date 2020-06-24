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

#include "src/lib/sort/sort.h"
class Data {
 public:
  Data(int size) { ReverseSort(size); }

  /**
   * Creates a reversely sorted vector
   */
  void ReverseSort(int size) {
    for (int i = 0; i < size; ++i) {
      v.push_back(size - i);
    }
  }

  /**
   * Creates a random vector
   */
  void ConstructRandomVector(int size) {
    std::srand(10);
    v.resize(size);
    std::generate(v.begin(), v.end(), std::rand);
  }

  std::vector<int> v;
};

const int g_size = 10000;

int Sort::QUICKSORT_THREASHOLD;
int Sort::MERGESORT_THREASHOLD;
int Sort::INTROSORT_THREASHOLD;

Data init(int size = g_size) {
  Data d(size);

  Sort::QUICKSORT_THREASHOLD = g_size / std::thread::hardware_concurrency();
  Sort::MERGESORT_THREASHOLD = g_size / std::thread::hardware_concurrency();
  Sort::INTROSORT_THREASHOLD = g_size / std::thread::hardware_concurrency();
  return d;
}

static void BM_SelectionSort(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto d = init(state.range(0));
    state.ResumeTiming();
    Sort::SelectionSort(d.v);
  }
  state.SetItemsProcessed(state.iterations() * g_size);
  state.SetComplexityN(state.range(0));
}

static void BM_HeapSort(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto d = init(state.range(0));
    state.ResumeTiming();
    Sort::HeapSort(d.v);
  }
  state.SetItemsProcessed(state.iterations() * g_size);
}

static void BM_InsertionSort(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto d = init(state.range(0));
    state.ResumeTiming();
    Sort::InsertionSort(d.v);
  }
  state.SetItemsProcessed(state.iterations() * g_size);
}

static void BM_BubbleSort(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto d = init(state.range(0));
    state.ResumeTiming();

    Sort::BubbleSort(d.v);
  }
  state.SetItemsProcessed(state.iterations() * g_size);
  state.SetComplexityN(state.range(0));
}

static void BM_BubbleSortImproved(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto d = init(state.range(0));
    state.ResumeTiming();

    Sort::BubbleSortImproved(d.v);
  }
  state.SetItemsProcessed(state.iterations() * g_size);
}

static void BM_MergeSort(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto d = init(state.range(0));
    state.ResumeTiming();

    Sort::MergeSort(d.v);
  }
  state.SetItemsProcessed(state.iterations() * g_size);
}

static void BM_MergeSortPar(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto d = init(state.range(0));
    state.ResumeTiming();

    Sort::MergeSortPar(d.v);
  }
  state.SetItemsProcessed(state.iterations() * g_size);
}

static void BM_QuickSort_iterative(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto d = init(state.range(0));
    state.ResumeTiming();

    Sort::QuickSort_iterative(d.v);
  }
  state.SetItemsProcessed(state.iterations() * g_size);
  state.SetComplexityN(state.range(0));
}

static void BM_QuickSort_oneCall(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto d = init(state.range(0));
    state.ResumeTiming();

    Sort::QuickSort_oneCall(d.v);
  }
  state.SetItemsProcessed(state.iterations() * g_size);
  state.SetComplexityN(state.range(0));
}

static void BM_QuickSort_twoCalls(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto d = init(state.range(0));
    state.ResumeTiming();

    Sort::QuickSort_twoCalls(d.v);
  }
  state.SetItemsProcessed(state.iterations() * g_size);
  state.SetComplexityN(state.range(0));
}

static void BM_QuickSortPar(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto d = init(state.range(0));
    state.ResumeTiming();

    Sort::QuickSortPar(d.v);
  }
  state.SetItemsProcessed(state.iterations() * g_size);
  state.SetComplexityN(state.range(0));
}

static void BM_IntroSort(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto d = init(state.range(0));
    state.ResumeTiming();

    Sort::Introsort(d.v);
  }
  state.SetItemsProcessed(state.iterations() * g_size);
}

static void BM_IntroSortPar(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto d = init(state.range(0));
    state.ResumeTiming();

    Sort::IntrosortPar(d.v);
  }
  state.SetItemsProcessed(state.iterations() * g_size);
}
static void BM_StdSort(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto d = init(state.range(0));
    state.ResumeTiming();

    std::sort(d.v.begin(), d.v.end());
  }
  state.SetItemsProcessed(state.iterations() * g_size);
  state.SetComplexityN(state.range(0));
}

int compare(const void* a, const void* b) { return (*(int*)a - *(int*)b); }
static void BM_StdQSort(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto d = init(state.range(0));
    state.ResumeTiming();

    std::qsort(&d.v[0], d.v.size(), sizeof(int), compare);
  }
  state.SetItemsProcessed(state.iterations() * g_size);
}

// Below are various benchmar registrations for measuring sort algorithms:

// BENCHMARK(BM_QuickSort_iterative)
//     ->RangeMultiplier(2)
//     ->Range(1 << 10, 1 << 23)
//     ->Complexity();
// ;

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

BENCHMARK(BM_MergeSort)->Arg(g_size);
BENCHMARK(BM_HeapSort)->Arg(g_size);
BENCHMARK(BM_InsertionSort)->Arg(g_size);
BENCHMARK(BM_SelectionSort)->Arg(g_size);
BENCHMARK(BM_BubbleSortImproved)->Arg(g_size);
BENCHMARK(BM_BubbleSort)->Arg(g_size);
BENCHMARK_MAIN();