//
// Created by efarhan on 1/16/19.
//

#include <benchmark/benchmark.h>

const int fromRange = 1<<5;
const int toRange = 1<<20;
int mod_optimal(const int m, const int n)
{
  if(m <= n) return m;
  return m % n;
}

static void BM_ModOptimal(benchmark::State& state) {


  for (auto _ : state) {
      const int m = state.range (0);
      const int n = state.range (1);
      benchmark::DoNotOptimize(mod_optimal (m, n));
    }

}
BENCHMARK(BM_ModOptimal)->Ranges({{fromRange, toRange}, {fromRange, toRange}});

static void BM_BM_ModClassic(benchmark::State& state) {


  for (auto _ : state) {
    const int m = state.range (0);
    const int n = state.range (1);
      benchmark::DoNotOptimize(m % n);
    }

}
BENCHMARK(BM_BM_ModClassic)->Ranges({{fromRange, toRange}, {fromRange, toRange}});

BENCHMARK_MAIN ();