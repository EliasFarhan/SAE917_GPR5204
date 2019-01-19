//
// Created by efarhan on 1/16/19.
//
#ifdef WIN32
#define _USE_MATH_DEFINES
#endif

#include <cmath>
#include <benchmark/benchmark.h>

const int fromRange = 2;
const int toRange = 512;

struct Quaternion
{
    float ax;
    float ay;
    float az;
    float aw;
};
struct Vec3f
{
    float x;
    float y;
    float z;
};

Quaternion AxisAngle(const Vec3f axis, const float angle)
{
  const float sina = sin(angle/2.0f);
  Quaternion q = {
      axis.x*sina,
      axis.y*sina,
      axis.z*sina,
      cos(angle/2.0f),
  };
  return q;
}

Quaternion AxisAngleOpti(const Vec3f axis, const float angle)
{
  Quaternion q = {
      0.0f,
      0.0f,
      axis.z*angle/2.0f,//sina,
      1.0f,
  };
  return q;
}

static void BM_AxisAngle(benchmark::State& state) {
	const Vec3f axis {0.0f,0.0f,1.0f};

  for (auto _ : state) {
	  float angle = static_cast<float>((rand() % 90) - 45)/360.0f*2.0f*M_PI;
      auto result = AxisAngle (axis, angle);
      benchmark::DoNotOptimize (result);
    }

}
BENCHMARK(BM_AxisAngle);
static void BM_AxisAngleOpti(benchmark::State& state) {
	const Vec3f axis {0.0f,0.0f,1.0f};

  for (auto _ : state) {
	  float angle = static_cast<float>((rand() % 90) - 45)/360.0f*2.0f*M_PI;
      auto result = AxisAngleOpti (axis, angle);
      benchmark::DoNotOptimize (result);
    }

}
BENCHMARK(BM_AxisAngleOpti);

BENCHMARK_MAIN ();