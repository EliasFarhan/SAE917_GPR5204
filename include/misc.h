//
// Created by efarhan on 22.12.18.
//

#ifndef GPR5204_MISC_H
#define GPR5204_MISC_H

#include <random>
#include <vector>


const float maxNmb = 100.0f;

void RandomFill(float* m, const int size)
{
    for(int i = 0; i<size; i++)
    {
        m[i] = static_cast<float>(rand());
    }
}

void RandomFill(std::vector<neko::Vec4f>& numbers, float start = -maxNmb, float end = maxNmb)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_real_distribution<float> dist{ start, end };
    std::generate(numbers.begin(), numbers.end(), [&g, &dist]() {return neko::Vec4f(dist(g), dist(g), dist(g), 0.0f); });
}

#endif //GPR5204_MISC_H
