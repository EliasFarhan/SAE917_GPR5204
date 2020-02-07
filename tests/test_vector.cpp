//
// Created by efarhan on 12/24/18.
//

#include <random>
#include <vector>

#include <vector.h>
#include <misc.h>
#include <benchmark/benchmark.h>


#ifdef _MSC_VEC
bool __builtin_expect(bool a , bool)
{
    return a;
}
#endif

const long fromRange = 8;
const long toRange = 1 << 15;


inline size_t FindClosestNaive1(const neko::Vec4f *v, size_t len, neko::Vec4f r) {
    assert(len != 0);
    size_t closestIndex = 0;
    float closestDistance = (v[0] - r).GetSquareMagnitude();
    for (size_t i = 1; i < len; i++) {
        const auto delta = v[i] - r;
        const auto distance = delta.GetSquareMagnitude();
        if ((delta.GetSquareMagnitude() < closestDistance)) {
            closestIndex = i;
            closestDistance = distance;
        }
    }
    return closestIndex;
}

inline size_t FindClosestNaive2(const neko::Vec4f *v, size_t len, neko::Vec4f r) {
    assert(len != 0);
    size_t closestIndex = 0;
    float closestDistance = std::numeric_limits<float>::max();
    for (size_t i = 0; i < len; i++) {
        const auto delta = v[i] - r;
        const auto distance = delta.GetSquareMagnitude();
        if (delta.GetSquareMagnitude() < closestDistance) {
            closestIndex = i;
            closestDistance = distance;
        }
    }
    return closestIndex;
}

inline size_t FindClosestAoSoA4(const neko::Vec4f *v, size_t len, neko::Vec4f r) {
    assert(len != 0);
    size_t closestIndex = 0;
    float closestDistance = std::numeric_limits<float>::max();
    for (size_t i = 0; i < len; i += 4) {
        neko::FourVec4f vArray(&v[i]);
        vArray -= r;
        const auto magnitudes = vArray.GetSquareMagnitude();
        for (const auto &magn : magnitudes) {
            if (magn < closestDistance) {
                closestDistance = magn;
                closestIndex = i + (&magn - magnitudes.data());
            }
        }

    }
    return closestIndex;
}

inline size_t FindClosestAoSoA8(const neko::Vec4f *v, size_t len, neko::Vec4f r) {
    assert(len != 0);
    size_t closestIndex = 0;
    float closestDistance =  std::numeric_limits<float>::max();
    for (size_t i = 0; i < len; i += 8) {
        neko::EightVec4f vArray(&v[i]);
        vArray -= r;
        const auto magnitudes = vArray.GetSquareMagnitude();
        for (const auto &magn : magnitudes) {
            if (magn < closestDistance) {
                closestDistance = magn;
                closestIndex = i + (&magn - magnitudes.data());
            }
        }
    }
    return closestIndex;
}
inline size_t FindClosestAoSoA4Intrinsics(const neko::Vec4f *v, size_t len, neko::Vec4f r) {
    assert(len != 0);
    size_t closestIndex = 0;
    float closestDistance = std::numeric_limits<float>::max();
    for (size_t i = 0; i < len; i += 4) {
        neko::FourVec4f vArray(&v[i]);
        vArray -= r;
        const auto magnitudes = vArray.GetSquareMagnitudeIntrinsincs();
        for (const auto &magn : magnitudes) {
            if (magn < closestDistance) {
                closestDistance = magn;
                closestIndex = i + (&magn - magnitudes.data());
            }
        }

    }
    return closestIndex;
}

inline size_t FindClosestAoSoA8Intrinsics(const neko::Vec4f *v, size_t len, neko::Vec4f r) {
    assert(len != 0);
    size_t closestIndex = 0;
    float closestDistance =  std::numeric_limits<float>::max();
    for (size_t i = 0; i < len; i += 8) {
        neko::EightVec4f vArray(&v[i]);
        vArray -= r;
        const auto magnitudes = vArray.GetSquareMagnitudeIntrinsincs();
        for (const auto &magn : magnitudes) {
            if (magn < closestDistance) {
                closestDistance = magn;
                closestIndex = i + (&magn - magnitudes.data());
            }
        }
    }
    return closestIndex;
}

static void BM_ClosestNaive1(benchmark::State &state) {
    const size_t size = state.range(0);
    std::vector<neko::Vec4f> m1;
    m1.resize(size + 1);
    RandomFill(m1);
    for (auto _ : state) {

        benchmark::DoNotOptimize(FindClosestNaive1(m1.data(), size, m1[size]));
    }
}

BENCHMARK(BM_ClosestNaive1)->Range(fromRange, toRange);


static void BM_ClosestNaive2(benchmark::State &state) {
    const size_t size = state.range(0);
    std::vector<neko::Vec4f> m1;
    m1.resize(size + 1);
    RandomFill(m1);
    for (auto _ : state) {

        benchmark::DoNotOptimize(FindClosestNaive2(m1.data(), size, m1[size]));
    }
}

BENCHMARK(BM_ClosestNaive2)->Range(fromRange, toRange);

static void BM_ClosestAoSoA4(benchmark::State &state) {
    const size_t size = state.range(0);
    std::vector<neko::Vec4f> m1;
    m1.resize(size + 1);
    RandomFill(m1);
    for (auto _ : state) {

        benchmark::DoNotOptimize(FindClosestAoSoA4(m1.data(), size, m1[size]));
    }
    assert(FindClosestNaive1(m1.data(), size, m1[size] == FindClosestAoSoA4(m1.data(), size, m1[size])));
}

BENCHMARK(BM_ClosestAoSoA4)->Range(fromRange, toRange);

static void BM_ClosestAoSoA8(benchmark::State &state) {
    const size_t size = state.range(0);
    std::vector<neko::Vec4f> m1;
    m1.resize(size + 1);
    RandomFill(m1);
    for (auto _ : state) {

        benchmark::DoNotOptimize(FindClosestAoSoA8(m1.data(), size, m1[size]));
    }

    assert(FindClosestNaive1(m1.data(), size, m1[size] == FindClosestAoSoA8(m1.data(), size, m1[size])));
}

BENCHMARK(BM_ClosestAoSoA8)->Range(fromRange, toRange);
static void BM_ClosestAoSoA4Intrinsics(benchmark::State &state) {
    const size_t size = state.range(0);
    std::vector<neko::Vec4f> m1;
    m1.resize(size + 1);
    RandomFill(m1);
    for (auto _ : state) {

        benchmark::DoNotOptimize(FindClosestAoSoA4Intrinsics(m1.data(), size, m1[size]));
    }
    assert(FindClosestNaive1(m1.data(), size, m1[size] == FindClosestAoSoA4(m1.data(), size, m1[size])));
}

BENCHMARK(BM_ClosestAoSoA4Intrinsics)->Range(fromRange, toRange);

static void BM_ClosestAoSoA8Intrinsics(benchmark::State &state) {
    const size_t size = state.range(0);
    std::vector<neko::Vec4f> m1;
    m1.resize(size + 1);
    RandomFill(m1);
    for (auto _ : state) {

        benchmark::DoNotOptimize(FindClosestAoSoA8Intrinsics(m1.data(), size, m1[size]));
    }

    assert(FindClosestNaive1(m1.data(), size, m1[size] == FindClosestAoSoA8(m1.data(), size, m1[size])));
}

BENCHMARK(BM_ClosestAoSoA8Intrinsics)->Range(fromRange, toRange);
static void BM_MagnitudeNaive(benchmark::State &state) {
    const size_t size = state.range(0);
    std::vector<neko::Vec4f> m1;
    m1.resize(size);
    RandomFill(m1);
    for (auto _ : state) {
        for (auto v : m1) {
            benchmark::DoNotOptimize(v.GetMagnitude());
        }
    }
}
BENCHMARK(BM_MagnitudeNaive)->Range(fromRange, toRange);

static void BM_MagnitudeAoSoA4(benchmark::State &state) {
    const size_t size = state.range(0);
    std::vector<neko::Vec4f> m1;
    m1.resize(size);
    RandomFill(m1);
    for (auto _ : state) {
        for(size_t i = 0; i < size; i+=4)
        {
            neko::FourVec4f v(&m1[i]);
            benchmark::DoNotOptimize(v.GetMagnitude());
        }
    }
}
BENCHMARK(BM_MagnitudeAoSoA4)->Range(fromRange, toRange);

static void BM_MagnitudeAoSoA4Intrinsincs(benchmark::State &state) {
    const size_t size = state.range(0);
    std::vector<neko::Vec4f> m1;
    m1.resize(size);
    RandomFill(m1);
    for (auto _ : state) {
        for(size_t i = 0; i < size; i+=4)
        {
            neko::FourVec4f v(&m1[i]);
            benchmark::DoNotOptimize(v.GetMagnitudeIntrinsincs());
        }
    }
}
BENCHMARK(BM_MagnitudeAoSoA4Intrinsincs)->Range(fromRange, toRange);

static void BM_MagnitudeAoSoA8(benchmark::State &state) {
    const size_t size = state.range(0);
    std::vector<neko::Vec4f> m1;
    m1.resize(size);
    RandomFill(m1);
    for (auto _ : state) {
        for(size_t i = 0; i < size; i+=8)
        {
            neko::EightVec4f v(&m1[i]);
            benchmark::DoNotOptimize(v.GetMagnitude());
        }
    }
}
BENCHMARK(BM_MagnitudeAoSoA8)->Range(fromRange, toRange);

static void BM_MagnitudeAoSoA8Intrinsincs(benchmark::State &state) {
    const size_t size = state.range(0);
    std::vector<neko::Vec4f> m1;
    m1.resize(size);
    RandomFill(m1);
    for (auto _ : state) {
        for(size_t i = 0; i < size; i+=8)
        {
            neko::EightVec4f v(&m1[i]);
            benchmark::DoNotOptimize(v.GetMagnitudeIntrinsincs());
        }
    }
}
BENCHMARK(BM_MagnitudeAoSoA8Intrinsincs)->Range(fromRange, toRange);

BENCHMARK_MAIN ();