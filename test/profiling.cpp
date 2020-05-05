#include <map>
#include <benchmark/benchmark.h>
#include "container.h"
#include "allocator.h"

constexpr size_t SIZE { 100 };

template <typename T>
void run_container() {
  T t { };
  for (size_t i { }; i < SIZE; i++) {
    t.push_back(i);
  }
}

template <typename T>
void run_map() {
  T t { };
  for (size_t i { }; i < SIZE; i++) {
    t[i] = i * i;
  }
}

static void run_container(benchmark::State& state) {
  for (auto _ : state) {
    run_container<otus::Container<int>>();
  }
}

static void run_container_allocator(benchmark::State& state) {
  for (auto _ : state) {
      run_container<otus::Container<int, otus::Allocator<int, SIZE>>>();
  }
}

static void run_map(benchmark::State& state) {
  for (auto _ : state) {
    std::map<int, int> m;
    run_map<std::map<int, int>>();
  }
}

static void run_map_allocator(benchmark::State& state) {
  for (auto _ : state) {
    run_map<
      std::map<int, int, std::less<int>,
      otus::Allocator<std::pair<int, int>, SIZE>>>();
  }
}

BENCHMARK(run_container);
BENCHMARK(run_container_allocator);
BENCHMARK(run_map);
BENCHMARK(run_map_allocator);

BENCHMARK_MAIN();
