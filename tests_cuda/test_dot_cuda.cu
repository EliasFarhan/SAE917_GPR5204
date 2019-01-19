#include <iostream>
#include <math.h>
#include <ctime>
#include <random>
#include <benchmark/benchmark.h>

#define THREADS_PER_BLOCK 512

static const int fromRange = 256;
static const int toRange = 1<<20;
// Kernel function to add the elements of two arrays
__global__
void dot(float *x, float *y, float* result)
{
  __shared__ float temp[THREADS_PER_BLOCK];
  int index = threadIdx.x + blockIdx.x * blockDim.x;
  temp[threadIdx.x] = x[index] * y[index];
  __syncthreads();
  if( 0 == threadIdx.x ) {
      float sum = 0;
      for( int i = 0; i < THREADS_PER_BLOCK; i++ )
        sum += temp[i];
      atomicAdd( result , sum );
    }

}

static void BM_GPU(benchmark::State& state)
{
  const int N = state.range (0);
  float *x, *y, *result;

  // Allocate Unified Memory – accessible from CPU or GPU
  cudaMallocManaged(&x, N*sizeof(float));
  cudaMallocManaged(&y, N*sizeof(float));
  cudaMallocManaged(&result, sizeof(float));

  // initialize x and y arrays on the host
  for (int i = 0; i < N; i++) {
      x[i] = (float) rand();
      y[i] = (float) rand();
    }

  // Run kernel on 1M elements on the GPU
  for (auto _ : state)
  {
    dot<<< N/THREADS_PER_BLOCK, THREADS_PER_BLOCK >>>(x, y,  result);
    benchmark::DoNotOptimize(result);
  }
  cudaFree(x);
  cudaFree(y);
  cudaFree(result);
}
// Register the function as a benchmark
BENCHMARK(BM_GPU)->Range(fromRange, toRange);

BENCHMARK_MAIN ();


