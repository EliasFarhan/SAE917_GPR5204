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
#include "benchmark/benchmark.h"

const int fromRange = 2;
const int toRange = 1024;

float powf_optimal(const float a, const int n)
{
	float result = 1.0f;
	for(int i = 0; i < n; i++)
	{
		result *= a;
	}
	return result;
}

int fib_func(const int i)
{
	const float gold_number = (1.0f + sqrtf(5.0f)) / 2.0f;
	return roundf(powf_optimal (gold_number, i) / sqrtf(5.0f));
}

int fib_serie(const int n)
{
	if (n == 0) return 0;
	if (n == 1) return 1;
	int first = 0;
	int second = 1;
	for (int j = 1; j < n; j++)
	{
		const int tmp = first + second;
		first = second;
		second = tmp;
	}
	return second;
}

static void BM_FibFunc(benchmark::State& state) {
	
	for (auto _ : state)
	{

		benchmark::DoNotOptimize(fib_func(state.range(0)));
	}
}
BENCHMARK(BM_FibFunc)->RangeMultiplier(2)->Range(fromRange, toRange);


static void BM_FibSerie(benchmark::State& state) {

	for (auto _ : state)
	{

		benchmark::DoNotOptimize(fib_serie(state.range(0)));
	}
}
BENCHMARK(BM_FibSerie)->RangeMultiplier(2)->Range(fromRange, toRange);

BENCHMARK_MAIN();