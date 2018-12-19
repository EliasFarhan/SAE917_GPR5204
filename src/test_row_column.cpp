/*
MIT License

Copyright (c) 2017 SAE Institute Switzerland AG

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

#include <vector>
#include "benchmark/benchmark.h"


class Matrix
{
public:
	Matrix(int width, int height): width(width), height(height)
	{
		values.resize(static_cast<unsigned long>(height) * static_cast<unsigned long>(width));

	}

	void Prefill()
	{
		for (auto& v : values)
		{
			v = static_cast<float>(rand());
		}

	}

	Matrix Transpose() const
	{
		Matrix m(height, width);
		for(int i = 0; i < height; i++)
		{
			for(int j = 0; j < width; j++)
			{
				m.values[j*width+i] = values[i*width+j];
			}
		}
		return m;
	}

	Matrix Mult(const Matrix& m2) const
	{
		if(width == m2.height)
		{
			Matrix newMatrix(m2.width, height);
			for (int y = 0; y < newMatrix.height; y++)
			{
				for (int x = 0; x < newMatrix.width; x++)
				{
					float newValue = 0.0f;

					for(int i = 0; i < width;i++)
					{
						newValue += values[y*width+i] * m2.values[i*m2.width+x];
					}
							//

					newMatrix.values[x*newMatrix.width+y] = newValue;
				}
			}
			return newMatrix;
		}
		else
		{
			return Matrix(0, 0);
		}
	}

  float MultLocal(const float * const m1, const float * const m2, const int width) const
  {
    float result = 0.0f;
    for(int i = 0; i < width; i++)
  	{
        result += m1[i]*m2[i];
  	}
    return result;
  }

  Matrix MultOptimize(const Matrix& m2T) const
  {

    const int localWidth = width;
    if (localWidth == m2T.width)
      {
        Matrix newMatrix(m2T.width, height);
        const int newWidth = newMatrix.width;
        const int newHeight = newMatrix.height;
        for (int y = 0; y < newHeight; y++)
          {
            for (int x = 0; x < newWidth; x++)
              {
                const int xOrigin = x*m2T.width;
                const int yOrigin = y*width;
                float newValue = MultLocal(&values[yOrigin], &m2T.values[xOrigin], localWidth);
                newMatrix.values[x*newWidth+y] = newValue;
              }
          }
        return newMatrix;
      }
    else
      {
        return Matrix(0, 0);
      }
  }

private:
	int width = 0;
	int height = 0;
	std::vector<float> values;
};


static void BM_MatrixMult(benchmark::State& state)
{
	for (auto _ : state)
	{
		state.PauseTiming();
		Matrix m1(state.range(0), state.range(1));
		m1.Prefill();
		Matrix m2(state.range(1), state.range(0));
		m2.Prefill();
		state.ResumeTiming();

		benchmark::DoNotOptimize(m1.Mult(m2));
	}
}
BENCHMARK(BM_MatrixMult)->Args ({64,64})->Args({128,128})->Args({256,256});


static void BM_MatrixMultOptimize(benchmark::State& state) {
	
	for (auto _ : state)
	{
		state.PauseTiming();
		Matrix m1(state.range(0), state.range(1));
		m1.Prefill();
		Matrix m2(state.range(1), state.range(0));
		m2.Prefill();

        const Matrix m2T = m2.Transpose ();
		state.ResumeTiming();

		benchmark::DoNotOptimize(m1.MultOptimize(m2T));
	}
}
BENCHMARK(BM_MatrixMultOptimize)->Args ({64,64})->Args({128,128})->Args({256,256});

BENCHMARK_MAIN();

