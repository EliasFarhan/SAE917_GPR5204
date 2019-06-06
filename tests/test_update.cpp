
//
// Created by efarhan on 14.02.18.
//

#include <cmath>

#include <misc.h>
#include <benchmark/benchmark.h>


struct Vec2f
{
    float x,y;
    Vec2f operator+(const Vec2f rhs) const
    {
      return Vec2f{x+rhs.x, y+rhs.y};
    }
    Vec2f operator*(const float rhs) const
    {
      return Vec2f{x*rhs, y*rhs};
    }
};

struct LinearPosition
{
    Vec2f position;
    Vec2f dir;
    float intensity;
};


static std::vector<Vec2f> positions;
static std::vector<int> conditions;
static std::vector<Vec2f> directions;
static std::vector<float> intensity;
static std::vector<LinearPosition> tmpInputPositions;
static std::vector<Vec2f> tmpOutputPositions;
static std::vector<int> indexes;

void Init(int length)
{
  positions.resize (length);
  conditions.resize (length);
  directions.resize (length);
  intensity.resize (length);
  for(int i = 0; i < length;i++)
  {
    positions[i] = {(float)rand (), (float)rand ()};
    conditions[i] = rand ()%2;
    directions[i] = {(float)rand (), (float)rand ()};
    float dirLength = sqrtf (directions[i].x*directions[i].x+directions[i].y*directions[i].y);
    directions[i].x /= dirLength;
    directions[i].y /= dirLength;
    intensity[i] = (float)(rand ()%100);
  }
}

void UpdateWithoutTmp(int stateRange)
{
  for(int i = 0; i<stateRange;i++)
    {
      if(conditions[i])
        positions[i] = positions[i] + directions[i] * intensity[i];
    }
}

void UpdateWithTmp(int stateRange)
{
  for(int i = 0;i < stateRange;i++)
  {
    const LinearPosition linPos = tmpInputPositions[i];
    tmpOutputPositions[i] = linPos.position + linPos.dir * linPos.intensity;
  }

  for(int i = 0;i < stateRange;i++)
    {
      positions[indexes[i]] = tmpOutputPositions[i];
    }
}

const int fromRange = 8;
const int toRange = 1<<20;

static void BM_UpdateWithoutTmp(benchmark::State& state) {
  Init(state.range (0));
  for (auto _ : state) {
      UpdateWithoutTmp (state.range (0));

      benchmark::DoNotOptimize (positions);
  }

}
BENCHMARK(BM_UpdateWithoutTmp)->Range(fromRange, toRange);

static void BM_UpdateWithTmp(benchmark::State& state) {

  Init(state.range (0));
  tmpInputPositions.resize (state.range (0));
  tmpOutputPositions.resize (state.range (0));
  indexes.resize (state.range (0));
  int tmpIndex = 0;
  for(int i = 0; i < state.range (0);i++)
    {
      if(conditions[i])
        {
          tmpInputPositions[tmpIndex].position = positions[i];
          tmpInputPositions[tmpIndex].dir = directions[i];
          tmpInputPositions[tmpIndex].intensity = intensity[i];
          indexes[tmpIndex] = i;
          tmpIndex++;
        }
    }
  const int size = tmpIndex;

  const Vec2f move{(float)rand(), (float)rand ()};
  for (auto _ : state) {
    UpdateWithTmp (size);
    benchmark::DoNotOptimize (positions);
  }
}
BENCHMARK(BM_UpdateWithTmp)->Range(fromRange, toRange);

BENCHMARK_MAIN ();