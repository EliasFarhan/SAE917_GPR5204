//
// Created by efarhan on 12/24/18.
//

#include <random>
#include <vector>
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

void VectorAdd1(float* a, float* b, float* out, int size)
{
  for(int i = 0; i < size;i++)
    {
      out[i] = a[i]+b[i];
    }
}
void VectorAdd2(const float* a, const float* b, float* out, const int size)
{
  for(int i = 0; i < size;i++)
    {
      out[i] = a[i]+b[i];
    }
}
void VectorAdd3(const float* a, const float* b, float* __restrict out, const size_t size)
{
  for(int i = 0; i < size;i++)
    {
      out[i] = a[i]+b[i];
    }
}

void VectorAdd4(const float* a, const float* b, float* out, const size_t size)
{
#ifdef __SSE__
    for(size_t i = 0; i < size; i+=4)
    {
        auto v1 = _mm_load_ps(a + i);//equivalent to float[4] or Vec4
        auto v2 = _mm_load_ps(b + i);//equivalent to float[4] or Vec4
        v1 = _mm_add_ps(v1, v2);//equivalent to sum = sum + v1 * v2
        _mm_store_ps(out+i, v1);
    }
#endif
}

void VectorAdd8(const float* a, const float* b, float* out, const size_t size)
{
#ifdef __AVX2__
    for(size_t i = 0; i < size; i+=8)
    {
        auto v1 = _mm256_load_ps(a + i);//float[8]
        auto v2 = _mm256_load_ps(b + i);//float[8]
        v1 = _mm256_add_ps(v1, v2);
        _mm256_store_ps(out+i, v1);

    }
#endif
}

static void BM_Add1(benchmark::State& state) {
  const int size = state.range (0);
  std::vector<float> m1;
  m1.resize (size);
  RandomFill (&m1[0], size);

  std::vector<float> m2;
  m2.resize ( size);
  RandomFill (&m2[0], size);

  std::vector<float> out;
  out.resize ( size);
  for (auto _ : state) {

      VectorAdd1(&m1[0],&m2[0],&out[0], size);
    }
}
BENCHMARK(BM_Add1)->Range(16, 8<<20);

static void BM_Add2(benchmark::State& state) {
  const int size = state.range (0);
  std::vector<float> m1;
  m1.resize (size);
  RandomFill (&m1[0], size);

  std::vector<float> m2;
  m2.resize ( size);
  RandomFill (&m2[0], size);

  std::vector<float> out;
  out.resize ( size);

  for (auto _ : state) {

      VectorAdd2(&m1[0],&m2[0],&out[0], size);
    }
}
BENCHMARK(BM_Add2)->Range(16, 8<<20);
static void BM_Add3(benchmark::State& state) {
  const int size = state.range (0);
  std::vector<float> m1;
  m1.resize (size);
  RandomFill (&m1[0], size);

  std::vector<float> m2;
  m2.resize ( size);
  RandomFill (&m2[0], size);


  std::vector<float> out;
  out.resize ( size);
  for (auto _ : state) {

      VectorAdd3(&m1[0],&m2[0],&out[0], size);
    }
}
BENCHMARK(BM_Add3)->Range(16, 8<<20);

static void BM_Add4(benchmark::State& state) {
    const int size = state.range (0);
    std::vector<float> m1;
    m1.resize (size);
    RandomFill (&m1[0], size);

    std::vector<float> m2;
    m2.resize ( size);
    RandomFill (&m2[0], size);


    std::vector<float> out;
    out.resize ( size);
    for (auto _ : state) {

        VectorAdd4(&m1[0],&m2[0],&out[0], size);
    }
}
BENCHMARK(BM_Add4)->Range(16, 8<<20);

static void BM_Add8(benchmark::State& state) {
    const int size = state.range (0);
    std::vector<float> m1;
    m1.resize (size);
    RandomFill (&m1[0], size);

    std::vector<float> m2;
    m2.resize ( size);
    RandomFill (&m2[0], size);


    std::vector<float> out;
    out.resize ( size);
    for (auto _ : state) {

        VectorAdd8(&m1[0],&m2[0],&out[0], size);
    }
}
BENCHMARK(BM_Add8)->Range(16, 8<<20);

BENCHMARK_MAIN ();