//
// Created by efarhan on 22.12.18.
//

#include <cmath>

#include <misc.h>
#include <benchmark/benchmark.h>



const int fromRange = 8;
const int toRange = 1<<20;

static void BM_VectorWithReserve(benchmark::State& state) {
  for (auto _ : state) {
      state.PauseTiming ();
      std::vector<int> randomNumbers;
      randomNumbers.reserve (state.range (0));
      state.ResumeTiming ();
      for(int i = 0; i<state.range (0); i++)
        {
          randomNumbers.push_back (rand());
        }
      benchmark::DoNotOptimize(randomNumbers);
    }

}
BENCHMARK(BM_VectorWithReserve)->Range(fromRange, toRange);

static void BM_VectorWithoutReserve(benchmark::State& state) {

  for (auto _ : state) {

      std::vector<int> randomNumbers;
      for(int i = 0; i<state.range (0); i++)
        {
          randomNumbers.push_back (rand());
        }
      benchmark::DoNotOptimize(randomNumbers);
    }
}
BENCHMARK(BM_VectorWithoutReserve)->Range(fromRange, toRange);

BENCHMARK_MAIN ();