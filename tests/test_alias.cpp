//
// Created by efarhan on 1/16/19.
//

/*
MIT License

Copyright (c) 2019 SAE Institute Switzerland AG

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include<cmath>

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

#include "benchmark/benchmark.h"

const int fromRange = 1 << 3;
const int toRange = 1 << 20;

struct MyIntArray1
{
    int* basePtr;
    int count;

    void SetToValue(int val)
    {
      for(int i = 0; i<count; i++)
        {
          basePtr[i] = val;
        }
    }
};
struct MyIntArray2
{
    int* basePtr;
    size_t count;

    void SetToValue(int val)
    {
      for(size_t i = 0; i<count; i++)
        {
          basePtr[i] = val;
        }
    }
};
struct MyIntArray3
{
    int* basePtr;
    int count;

    void SetToValue(int val)
    {
      const auto localCount = count;
      for(int i = 0; i < localCount; i++)
        {
          basePtr[i] = val;
        }
    }
};

void foo(const float* a, const float* b, float* out, const int length)
{
	for(int i = 0; i < length; i++)
	{
		out[i] = a[i] + b[i];
	}
}
static void BM_Foo(benchmark::State& state) {


	const int count = state.range(0);
	float* a = new float[count];
	float* b = new float[count];
	for(int i = 0; i<count;i++)
	{
		a[i] = rand();
		b[i] = rand();
	}
	float* out = new float[count];
	for (auto _ : state) {
		foo(a, b, out, count);
		benchmark::DoNotOptimize(out);
	}
	delete[](a);
	delete[](b);
	delete[](out);
}
BENCHMARK(BM_Foo)->Range(fromRange, toRange);


static void BM_FooAlias(benchmark::State& state) {

  const int count = state.range(0);
  float* a = new float[count+2];
  for(int i = 0; i<count;i++)
    {
      a[i] = rand();
    }
  for (auto _ : state)
    {
      foo (a, a + 1, a + 1, count);
      benchmark::DoNotOptimize (a);
    }
  delete[](a);
}
BENCHMARK(BM_FooAlias)->Range(fromRange, toRange);
#ifdef __SSE__
void foo_sse(const float* a, const float* b, float* out, const size_t length)
{
	assert(length >= 4);
	const size_t sse_length = length - (length%4);

	assert(sse_length % 4 == 0);
      for (size_t i = 0; i < sse_length; i += 4) {
          __m128 v1 =  _mm_loadu_ps(a + i);//equivalent to float[4] or Vec4
          __m128 v2 =  _mm_loadu_ps(b + i);//equivalent to float[4] or Vec4
          __m128 sum = _mm_add_ps(v1, v2);//equivalent to sum = v1+v2

          _mm_storeu_ps(out + i, sum);
      }
      for(size_t i = sse_length; i < length;i++)
	  {
		out[i] = a[i] + b[i];
	  }
}

static void BM_Foo_SSE(benchmark::State& state) {

	const int count = state.range(0);
	float* a = new float[count];
	float* b = new float[count];
	for (int i = 0; i < count; i++)
	{
		a[i] = rand();
		b[i] = rand();
	}
	float* out = new float[count];
	for (auto _ : state) {
		foo_sse(a, b, out, count);
		benchmark::DoNotOptimize(out);
	}
	delete[](a);
	delete[](b);
	delete[](out);
}
BENCHMARK(BM_Foo_SSE)->Range(fromRange, toRange);
#endif


#ifdef __AVX2__
void foo_avx2(const float* a, const float* b, float* out, const size_t length) {

	assert(length >= 8);
	const size_t avx2_length = length - (length%8);
	assert(avx2_length % 8 == 0);
	for (size_t i = 0; i < avx2_length; i += 8) {
		__m256 v1 = _mm256_loadu_ps(a + i);
		__m256 v2 = _mm256_loadu_ps(b + i);
		__m256 sum = _mm256_add_ps(v1, v2);
		_mm256_storeu_ps(out + i, sum);
	}
	for(size_t i = avx2_length; i < length;i++)
	{
		out[i] = a[i] + b[i];
	}

}

static void BM_Foo_AVX2(benchmark::State& state) {

	const int count = state.range(0);
	float* a = new float[count];
	float* b = new float[count];
	for (int i = 0; i < count; i++)
	{
		a[i] = rand();
		b[i] = rand();
	}
	float* out = new float[count];
	for (auto _ : state) {
		foo_avx2(a, b, out, count);
		benchmark::DoNotOptimize(out);
	}
	delete[](a);
	delete[](b);
	delete[](out);
}
BENCHMARK(BM_Foo_AVX2)->Range(fromRange, toRange);
#endif

static void BM_Array1(benchmark::State& state) {

  MyIntArray1 intArray;
  intArray.count = state.range (0);
  intArray.basePtr = new int[intArray.count];
  for (auto _ : state) {
      state.PauseTiming ();
      const int newValue = rand();
      state.ResumeTiming ();
      intArray.SetToValue (newValue);
      benchmark::DoNotOptimize (intArray);
    }
    delete[] (intArray.basePtr);
}
BENCHMARK(BM_Array1)->Range(fromRange, toRange);


static void BM_Array2(benchmark::State& state) {

  MyIntArray2 intArray;
  intArray.count = state.range (0);
  intArray.basePtr = new int[intArray.count];
  for (auto _ : state) {
      state.PauseTiming ();
      const int newValue = rand();
      state.ResumeTiming ();
      intArray.SetToValue (newValue);
      benchmark::DoNotOptimize (intArray);
    }
  delete[] (intArray.basePtr);
}
BENCHMARK(BM_Array2)->Range(fromRange, toRange);

static void BM_Array3(benchmark::State& state) {

  MyIntArray3 intArray;
  intArray.count = state.range (0);
  intArray.basePtr = new int[intArray.count];
  for (auto _ : state) {
      state.PauseTiming ();
      const int newValue = rand();
      state.ResumeTiming ();
      intArray.SetToValue (newValue);
      benchmark::DoNotOptimize (intArray);
    }
  delete[] (intArray.basePtr);
}
BENCHMARK(BM_Array3)->Range(fromRange, toRange);

BENCHMARK_MAIN();