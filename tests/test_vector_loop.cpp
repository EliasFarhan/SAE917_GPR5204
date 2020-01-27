//
// Created by efarhan on 23/01/2020.
//

#include <vector>
#include <iostream>
#include <benchmark/benchmark.h>

const unsigned long fromRange = 8;
const unsigned long toRange = 1<<15;

void execute1(std::vector<int>& v)
{
    for(int i = 0; i < v.size(); i++)
    {
        v[i] = i;
    }
}
void execute2(std::vector<int>& v)
{
    int i = 0;
    for(auto& vi : v)
    {
        vi = i;
        i++;
    }
}

static void BM_VectorIndexLoop(benchmark::State& state) {
    const size_t n = state.range(0);
    std::vector<int> v(n);
    for (auto _ : state) {
        execute1(v);
    }
}

BENCHMARK(BM_VectorIndexLoop)->Range(fromRange, toRange);


static void BM_VectorRangeLoop(benchmark::State& state) {
    const size_t n = state.range(0);
    std::vector<int> v(n);
    for (auto _ : state) {
        execute2(v);
    }
}

BENCHMARK(BM_VectorRangeLoop)->Range(fromRange, toRange);

BENCHMARK_MAIN();
