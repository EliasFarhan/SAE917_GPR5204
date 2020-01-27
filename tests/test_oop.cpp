//
// Created by efarhan on 1/17/19.
//
#include <random>
#include <vector>

#include <benchmark/benchmark.h>


const int fromRange = 1 << 3;

const int toRange = 1 << 20;

struct Vector2f
{
    float x;
    float y;

    float GetDistance()
    {
        return sqrtf(x * x + y * y);
    }

    friend Vector2f operator+(const Vector2f &lhs, const Vector2f &rhs)
    {
        Vector2f v{lhs.x + rhs.x, lhs.y + rhs.y};
        return v;
    }

    friend Vector2f operator*(const Vector2f &lhs, float rhs)
    {
        Vector2f v{lhs.x * rhs, lhs.y * rhs};
        return v;
    }

};

struct GameObject
{
    float foo[16];//16*4 bytes
    Vector2f position;//8bytes
    Vector2f velocity;//8bytes
    Vector2f scale; //8bytes
    bool isActive; //1byte
    GameObject *parent = nullptr; //4bytes
    GameObject *children = nullptr; //4bytes
    size_t childrenCount = 0;//8bytes
    bool needParentUpdate = false; //1byte
    Vector2f newPos;
    float speed = 0.0f;

    void Update(float dt)
    {
        speed = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y);
        newPos = position + velocity * dt;
    }
};

struct UpdateIn
{
    Vector2f position;
    Vector2f velocity;
};

struct UpdateOut
{
    Vector2f position;
    float speed;
};

void Update(const UpdateIn *dataIn, UpdateOut *dataOut, float dt)
{
    dataOut->speed = std::sqrt(dataIn->velocity.x * dataIn->velocity.x + dataIn->velocity.y * dataIn->velocity.y);
    dataOut->position = dataIn->position + dataIn->velocity * dt;
}

inline float CalulateSpeed(const Vector2f &velocity)
{
    return std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
}

void UpdatePosition(const Vector2f *position, const Vector2f *velocity, UpdateOut *dataOut, float dt)
{
    dataOut->position = (*position) + (*velocity) * dt;
}

static void BM_GameObjectUpdate(benchmark::State &state)
{
    const size_t n = state.range(0);
    std::vector<GameObject> gameObjects(n);
    for (auto &gameObject : gameObjects)
    {
        gameObject.position = Vector2f{(float) rand(), (float) rand()};
        gameObject.velocity = Vector2f{(float) rand(), (float) rand()};
    }
    for (auto _ : state)
    {
        for (auto &gameObject: gameObjects)
        {
            gameObject.Update(0.3f);
        }
    }
}

BENCHMARK(BM_GameObjectUpdate)->Range(fromRange, toRange);

static void BM_FocusOnUpdate(benchmark::State &state)
{
    const size_t n = state.range(0);
    std::vector<UpdateIn> dataIn(n);
    std::vector<UpdateOut> dataOut(n);
    for (auto &d : dataIn)
    {
        d.position = Vector2f{(float) rand(), (float) rand()};
        d.velocity = Vector2f{(float) rand(), (float) rand()};
    }
    for (auto _ : state)
    {

        for (size_t i = 0; i < n; i++)
        {
            Update(&dataIn[i], &dataOut[i], 0.3f);
        }
    }
}

BENCHMARK(BM_FocusOnUpdate)->Range(fromRange, toRange);

static void BM_SeparateCalculus(benchmark::State &state)
{
    const size_t n = state.range(0);
    std::vector<Vector2f> posIn(n);
    std::vector<Vector2f> velIn(n);
    std::vector<UpdateOut> dataOut(n);
    for (auto &d : posIn)
    {
        d = Vector2f{(float) rand(), (float) rand()};
    }
    for (auto &d : velIn)
    {
        d = Vector2f{(float) rand(), (float) rand()};
    }
    for (auto _ : state)
    {
        UpdateOut *dataOutPtr = &dataOut[0];
        for (size_t i = 0; i < n; i++)
        {
            dataOutPtr[i].speed = CalulateSpeed(velIn[i]);
        }
        for (size_t i = 0; i < n; i++)
        {
            UpdatePosition(&posIn[i], &velIn[i], &dataOutPtr[i], 0.3f);
        }
    }
}

BENCHMARK(BM_SeparateCalculus)->Range(fromRange, toRange);

BENCHMARK_MAIN();

