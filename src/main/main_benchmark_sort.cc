/**
 * Demo for using Benchmarking using Google's benchmark platform:
 * https://github.com/google/benchmark
 */
#include <benchmark/benchmark.h>
#include <stdlib.h> /* qsort */

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

// #include <execution>

#include "src/lib/sort/sort.h"
class Data {
 public:
  Data(int size) { ConstructRandomVector(size); }
  void ReverseSort(int size) {
    for (int i = 0; i < size; ++i) {
      v.push_back(size - i);
    }
  }
  void ConstructRandomVector(int size) {
    std::srand(10);
    v.resize(size);
    std::generate(v.begin(), v.end(), std::rand);
  }

  std::vector<int> v;
};

const int size = 1000000;

static void BM_SelectionSort(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    Data d(size);
    state.ResumeTiming();
    Sort::SelectionSort(d.v);
  }
  state.SetItemsProcessed(state.iterations() * size);
}

static void BM_HeapSort(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    Data d(size);
    state.ResumeTiming();
    Sort::HeapSort(d.v);
  }
  state.SetItemsProcessed(state.iterations() * size);
}

static void BM_InsertionSort(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    Data d(size);
    state.ResumeTiming();
    Sort::InsertionSort(d.v);
  }
  state.SetItemsProcessed(state.iterations() * size);
}

static void BM_BubbleSort(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    Data d(size);
    state.ResumeTiming();

    Sort::BubbleSort(d.v);
  }
  state.SetItemsProcessed(state.iterations() * size);
}

static void BM_BubbleSortImproved(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    Data d(size);
    state.ResumeTiming();

    Sort::BubbleSortImproved(d.v);
  }
  state.SetItemsProcessed(state.iterations() * size);
}

static void BM_MergeSort(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    Data d(size);
    state.ResumeTiming();

    Sort::MergeSort(d.v);
  }
  state.SetItemsProcessed(state.iterations() * size);
}

static void BM_MergeSortPar(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    Data d(size);
    state.ResumeTiming();

    Sort::MergeSortPar(d.v);
  }
  state.SetItemsProcessed(state.iterations() * size);
}

static void BM_QuickSort_iterative(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    Data d(size);
    state.ResumeTiming();

    Sort::QuickSort_iterative(d.v);
  }
  state.SetItemsProcessed(state.iterations() * size);
}

static void BM_QuickSort_oneCall(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    Data d(size);
    state.ResumeTiming();

    Sort::QuickSort_oneCall(d.v);
  }
  state.SetItemsProcessed(state.iterations() * size);
}

static void BM_QuickSort_twoCalls(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    Data d(size);
    state.ResumeTiming();

    Sort::QuickSort_twoCalls(d.v);
  }
  state.SetItemsProcessed(state.iterations() * size);
}

static void BM_QuickSortPar(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    Data d(size);
    state.ResumeTiming();

    Sort::QuickSortPar(d.v);
  }
  state.SetItemsProcessed(state.iterations() * size);
}

static void BM_IntroSort(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    Data d(size);
    state.ResumeTiming();

    Sort::Introsort(d.v);
  }
  state.SetItemsProcessed(state.iterations() * size);
}

static void BM_IntroSortPar(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    Data d(size);
    state.ResumeTiming();

    Sort::IntrosortPar(d.v);
  }
  state.SetItemsProcessed(state.iterations() * size);
}
static void BM_StdSort(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    Data d(size);
    state.ResumeTiming();

    std::sort(d.v.begin(), d.v.end());
  }
  state.SetItemsProcessed(state.iterations() * size);
}

int compare(const void* a, const void* b) { return (*(int*)a - *(int*)b); }
static void BM_StdQSort(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    Data d(size);
    state.ResumeTiming();

    std::qsort(&d.v[0], d.v.size(), sizeof(int), compare);
  }
  state.SetItemsProcessed(state.iterations() * size);
}

// static void BM_StdParSort(benchmark::State& state) {
//   for (auto _ : state) {
//     state.PauseTiming();
//     Data d(size);
//     state.ResumeTiming();
//     std::sort(std::execution::par_unseq, d.v.begin(), d.v.end());

//     std::sort(d.v.begin(), d.v.end());
//   }
//   state.SetItemsProcessed(state.iterations() * size);
// }

// Register the function as a benchmark
BENCHMARK(BM_StdSort);
BENCHMARK(BM_IntroSort)->UseRealTime();
BENCHMARK(BM_QuickSort_iterative);
BENCHMARK(BM_QuickSort_oneCall);
BENCHMARK(BM_QuickSort_twoCalls);
// BENCHMARK(BM_StdQSort);

BENCHMARK(BM_QuickSortPar)->UseRealTime();

BENCHMARK(BM_IntroSortPar)->UseRealTime();

BENCHMARK(BM_MergeSortPar)->UseRealTime();
BENCHMARK(BM_MergeSort);
BENCHMARK(BM_HeapSort);

BENCHMARK(BM_InsertionSort);

BENCHMARK(BM_SelectionSort);
BENCHMARK(BM_BubbleSortImproved);
BENCHMARK(BM_BubbleSort);

BENCHMARK_MAIN();