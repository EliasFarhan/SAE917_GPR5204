//
// Created by efarhan on 17/01/2020.
//
#include <random>
#include <benchmark/benchmark.h>
#include <cmath>

#ifdef _MSC_VER
bool __builtin_expect (bool a, bool b)
{
    return a;
}
#endif

const int divisor = 13;

const int fromRange = 16;

const int toRange = 1 << 20;

void RandomFill(int *m, const int size)
{
    for (int i = 0; i < size; i++) {
        do {
            m[i] = static_cast<int>(rand() % 100);
        }
        while (m[i] % divisor == 0);
    }
}
int naive_func(const int *v, size_t size)
{
    int result = 0;
    for (int i = 0; i < size; i++) {
        if (__builtin_expect(v[i] % divisor == 0, true)) {
            result += v[i] * v[i];
        }
        else
        {
            result += v[i];
        }
    }
    return result;
}

int opti_func(const int *v, size_t size)
{
    int result = 0;
    for (int i = 0; i < size; i++) {
        if (__builtin_expect(v[i] % divisor == 0, false)) {
            result += v[i] * v[i];
        }
        else
        {
            result += v[i];
        }
    }
    return result;

}

static void BM_BranchNaive(benchmark::State &state)
{
    const size_t size = state.range(0);
    std::vector<int> m1(size);
    RandomFill(m1.data(), size);
    for (auto _ : state) {

        benchmark::DoNotOptimize(naive_func(m1.data(), m1.size()));
    }
}

BENCHMARK(BM_BranchNaive)->Range(fromRange, toRange);

static void BM_BranchOpti(benchmark::State &state)
{
    const size_t size = state.range(0);
    std::vector<int> m1(size);
    RandomFill(m1.data(), size);
    for (auto _ : state) {
        benchmark::DoNotOptimize(opti_func(m1.data(), m1.size()));
    }
}

BENCHMARK(BM_BranchOpti)->Range(fromRange, toRange);

BENCHMARK_MAIN();