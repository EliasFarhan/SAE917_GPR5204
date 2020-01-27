//
// Created by efarhan on 21/01/2020.
//

#include <benchmark/benchmark.h>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <vector>
#include <asm_func.h>

const unsigned long fromRange = 8;
const unsigned long toRange = 1<<20;

static void BM_CountNaive(benchmark::State& state) {
    const size_t n = state.range(0);
    std::vector<float> v(n);
    std::generate(v.begin(), v.end(),[]{ return rand()%2==0?-1:1;});
    for (auto _ : state) {
        benchmark::DoNotOptimize(count_positive(v.data(), n));
    }

}

BENCHMARK(BM_CountNaive)->Range(fromRange, toRange);

static void BM_CountSortNaive(benchmark::State& state) {
    const size_t n = state.range(0);
    std::vector<float> v(n);
    std::generate(v.begin(), v.end(),[]{ return rand()%2==0?-1:1;});

    std::partition(v.begin(), v.end(), [](float x){return x > 0.0f;});
    for (auto _ : state) {
        benchmark::DoNotOptimize(count_positive(v.data(), n));
    }

}

BENCHMARK(BM_CountSortNaive)->Range(fromRange, toRange);

static void BM_Count(benchmark::State& state) {
    const size_t n = state.range(0);
    std::vector<float> v(n);
    std::generate(v.begin(), v.end(),[]{ return rand()%2==0?-1:1;});
    for (auto _ : state) {
        benchmark::DoNotOptimize(std::count_if(v.cbegin(), v.cend(), [](float x){return x > 0.0f;}));
    }

}

BENCHMARK(BM_Count)->Range(fromRange, toRange);

static void BM_CountSort(benchmark::State& state) {
    const size_t n = state.range(0);
    std::vector<float> v(n);
    std::generate(v.begin(), v.end(),[]{ return rand()%2==0?-1:1;});

    std::partition(v.begin(), v.end(), [](float x){return x > 0.0f;});
    for (auto _ : state) {
        benchmark::DoNotOptimize(std::count_if(v.cbegin(), v.cend(), [](float x){return x > 0.0f;}));
    }

}

BENCHMARK(BM_CountSort)->Range(fromRange, toRange);
BENCHMARK_MAIN();
