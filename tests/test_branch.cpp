//
// Created by efarhan on 17/01/2020.
//
#include <random>
#include <benchmark/benchmark.h>
#include <cmath>
#include <asm_func.h>

#ifdef _MSC_VER
bool __builtin_expect (bool a, bool b)
{
    return a;
}
#endif


const int fromRange = 16;

const int toRange = 1 << 20;



size_t count_positive_c(const float *v, size_t size)
{
    size_t result = 0;
    for (size_t i = 0; i < size; i++) {
        result += v[i] > 0.0f ? 1 : 0;
    }
    return result;
}

static void BM_BranchNaive(benchmark::State &state)
{
    const size_t size = state.range(0);
    std::vector<float> m1(size);
    std::generate(m1.begin(), m1.end(), []{return rand()%2 ? -1.0f:1.0f;});
    for (auto _ : state) {

        benchmark::DoNotOptimize(count_positive(m1.data(), m1.size()));
    }
}

BENCHMARK(BM_BranchNaive)->Range(fromRange, toRange);

static void BM_BranchO1(benchmark::State &state)
{
    const size_t size = state.range(0);
    std::vector<float> m1(size);
    std::generate(m1.begin(), m1.end(), []{return rand()%2 ? -1.0f:1.0f;});
    for (auto _ : state) {

        benchmark::DoNotOptimize(count_positive_o1(m1.data(), m1.size()));
    }
}

BENCHMARK(BM_BranchO1)->Range(fromRange, toRange);

static void BM_BranchOpti(benchmark::State &state)
{
    const size_t size = state.range(0);
    std::vector<float> m1(size);
    std::generate(m1.begin(), m1.end(), []{return rand()%2 ? -1.0f:1.0f;});
    for (auto _ : state) {

        benchmark::DoNotOptimize(count_positive_c(m1.data(), m1.size()));
    }
}

BENCHMARK(BM_BranchOpti)->Range(fromRange, toRange);

BENCHMARK_MAIN();