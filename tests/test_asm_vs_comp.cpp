//
// Created by efarhan on 14/01/2020.
//

#include "benchmark/benchmark.h"
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <asm_func.h>

const int fromRange = 16;
const int toRange = 1<<20;

int c_avg(const int *v, size_t length) {
    if (length == 0)
        return 0;
    int result = 0;
    for(size_t i = 0; i < length;i++)
    {
        result += v[i];
    }
    return result / length;
}

int cpp_avg(const std::vector<int> &v) {
    const auto size = v.size();
    return size == 0 ? 0 : std::accumulate(v.cbegin(), v.cend(), 0) / size;
};

static void BM_AsmAvg(benchmark::State &state) {
    const int size = state.range(0);
    std::vector<int> v(size);
    std::default_random_engine generator;
    std::uniform_int_distribution<int> randFunc(-100,100);
    std::generate(v.begin(), v.end(), [&generator, &randFunc](){return randFunc(generator);});

    for (auto _ : state) {

        benchmark::DoNotOptimize(avg(v.data(), v.size()));
    }
}

BENCHMARK(BM_AsmAvg)->Range(fromRange, toRange);

static void BM_CAvg(benchmark::State &state) {
    const int size = state.range(0);
    std::vector<int> v(size);
    std::default_random_engine generator;
    std::uniform_int_distribution<int> randFunc(-100,100);
    std::generate(v.begin(), v.end(), [&generator, &randFunc](){return randFunc(generator);});

    for (auto _ : state) {

        benchmark::DoNotOptimize(c_avg(v.data(), v.size()));

    }
}

BENCHMARK(BM_CAvg)->Range(fromRange, toRange);

static void BM_CppAvg(benchmark::State &state) {
    const int size = state.range(0);
    std::vector<int> v(size);
    std::default_random_engine generator;
    std::uniform_int_distribution<int> randFunc(-100,100);
    std::generate(v.begin(), v.end(), [&generator, &randFunc](){return randFunc(generator);});

    for (auto _ : state) {

        benchmark::DoNotOptimize(cpp_avg(v));
    }
}

BENCHMARK(BM_CppAvg)->Range(fromRange, toRange);

BENCHMARK_MAIN();