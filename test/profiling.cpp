#include <map>
#include <benchmark/benchmark.h>
#include "container.h"
#include "allocator.h"

constexpr size_t SIZE { 10000 };

template <template<typename, typename> typename T, typename I, typename Alloc>
void run(T<I, Alloc> &t) {
  for (size_t i { }; i < SIZE; i++) {
    t.push_back(i);
  }
}

template <typename T, typename K, typename Less, class Alloc>
void run_m(std::map<T, K, Less, Alloc> &m) {
  for (size_t i { }; i < SIZE; i++) {
    m[i] = i * i;
  }
}

static void run_container(benchmark::State& state) {
  for (auto _ : state) {
    otus::Container<int> c;
    run(c);
  }
}

static void run_container_allocator(benchmark::State& state) {
  for (auto _ : state) {
    otus::Container<int, otus::Allocator<int, SIZE>> c;
    run(c);
  }
}

static void run_map(benchmark::State& state) {
  for (auto _ : state) {
    std::map<int, int> m;
    run_m(m);
  }
}

static void run_map_allocator(benchmark::State& state) {
  for (auto _ : state) {
    std::map<int, int, std::less<int>, otus::Allocator<std::pair<int, int>, SIZE>> m;
    run_m(m);
  }
}

BENCHMARK(run_container);
BENCHMARK(run_container_allocator);
BENCHMARK(run_map);
BENCHMARK(run_map_allocator);

BENCHMARK_MAIN();
