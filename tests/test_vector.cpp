//
// Created by efarhan on 12/24/18.
//

#include <random>
#include <vector>

#include <vector.h>
#include <misc.h>
#include <benchmark/benchmark.h>
#ifdef WIN32
#include <intrin.h>
#ifdef __AVX2__
#define __SSE__
#define __SSE4_2__
#define __FMA__
#endif
#else
#include <x86intrin.h>
#endif
const long fromRange = 8;
const long toRange = 1 << 15;
const float maxNmb = 100.0f;

void RandomFill(std::vector<neko::Vec4f>& numbers, float start = -maxNmb, float end = maxNmb)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_real_distribution<float> dist{ start, end };
    std::generate(numbers.begin(), numbers.end(), [&g, &dist]() {return neko::Vec4f(dist(g), dist(g), dist(g), 0.0f); });
}

size_t FindClosestNaive1(const neko::Vec4f* v, size_t len, neko::Vec4f r)
{
    assert(len != 0);
    size_t closestIndex = 0;
    float closestDistance = (v[0]-r).GetSquareMagnitude();
	for(size_t i = 1; i < len; i++)
	{
        const auto delta = v[i] - r;
        const auto distance = delta.GetSquareMagnitude();
		if(delta.GetSquareMagnitude() < closestDistance)
		{
            closestIndex = i;
            closestDistance = distance;
		}
	}
    return closestIndex;
}

size_t FindClosestNaive2(const neko::Vec4f* v, size_t len, neko::Vec4f r)
{
    assert(len != 0);
    size_t closestIndex = 0;
    float closestDistance = -1.0f;
    for (size_t i = 0; i < len; i++)
    {
        const auto delta = v[i] - r;
        const auto distance = delta.GetSquareMagnitude();
        if (closestDistance < 0.0f || delta.GetSquareMagnitude() < closestDistance)
        {
            closestIndex = i;
            closestDistance = distance;
        }
    }
    return closestIndex;
}
size_t FindClosestAoSoA4(const neko::Vec4f* v, size_t len, neko::Vec4f r)
{
    assert(len != 0);
    size_t closestIndex = 0;
    float closestDistance = -1.0f;
    for (size_t i = 0; i < len; i+=4)
    {
        neko::FourVec4f vArray(&v[i]);
        vArray -= r;
        const auto magnitudes = vArray.GetSquareMagnitude();
        const auto minMagn = std::min_element(magnitudes.cbegin(), magnitudes.cend());
    	if(closestDistance < 0.0f || *minMagn < closestDistance)
    	{
            closestDistance = *minMagn;
            closestIndex = i + (minMagn - magnitudes.cbegin());
    	}
    }
    return closestIndex;
}
size_t FindClosestAoSoA8(const neko::Vec4f* v, size_t len, neko::Vec4f r)
{
    assert(len != 0);
    size_t closestIndex = 0;
    float closestDistance = -1.0f;
    for (size_t i = 0; i < len; i += 8)
    {
        neko::EightVec4f vArray(&v[i]);
        vArray -= r;
        const auto magnitudes = vArray.GetSquareMagnitude();
        const auto minMagn = std::min_element(magnitudes.cbegin(), magnitudes.cend());
        if (closestDistance < 0.0f || *minMagn < closestDistance)
        {
            closestDistance = *minMagn;
            closestIndex = i + (minMagn - magnitudes.cbegin());
        }
    }
    return closestIndex;
}

static void BM_ClosestNaive(benchmark::State& state) {
  const size_t size = state.range (0);
  std::vector<neko::Vec4f> m1;
  m1.resize (size+1);
  RandomFill (m1);
  for (auto _ : state) {

      benchmark::DoNotOptimize(FindClosestNaive1(m1.data(), size, m1[size]));
    }
}
BENCHMARK(BM_ClosestNaive)->Range(fromRange, toRange);


static void BM_ClosestNaive2(benchmark::State& state) {
    const size_t size = state.range(0);
    std::vector<neko::Vec4f> m1;
    m1.resize(size + 1);
    RandomFill(m1);
    for (auto _ : state) {

        benchmark::DoNotOptimize(FindClosestNaive2(m1.data(), size, m1[size]));
    }
}
BENCHMARK(BM_ClosestNaive2)->Range(fromRange, toRange);

static void BM_ClosestAoSoA4(benchmark::State& state) {
    const size_t size = state.range(0);
    std::vector<neko::Vec4f> m1;
    m1.resize(size + 1);
    RandomFill(m1);
    for (auto _ : state) {

        benchmark::DoNotOptimize(FindClosestAoSoA4(m1.data(), size, m1[size]));
    }
}
BENCHMARK(BM_ClosestAoSoA4)->Range(fromRange, toRange);

static void BM_ClosestAoSoA8(benchmark::State& state) {
    const size_t size = state.range(0);
    std::vector<neko::Vec4f> m1;
    m1.resize(size + 1);
    RandomFill(m1);
    for (auto _ : state) {

        benchmark::DoNotOptimize(FindClosestAoSoA8(m1.data(), size, m1[size]));
    }
}
BENCHMARK(BM_ClosestAoSoA8)->Range(fromRange, toRange);

BENCHMARK_MAIN ();