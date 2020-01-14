//
// Created by efarhan on 23/12/2019.
//
#include <iostream>
#include <array>
#include <numeric>
#include <asm_func.h>

int main() {
    int a = 3;
    int b = 2;
    std::cout << "A: " << a << ", B: " << b << '\n';
    swap_int(&a, &b);
    std::cout << "A: " << a << ", B: " << b << '\n';

    float f1 = 3.5f;
    float f2 = 2.2f;
    std::cout << "F1: " << f1 << ", F2: " << f2 << '\n';
    swap_float(&f1, &f2);
    std::cout << "F1: " << f1 << ", F2: " << f2 << '\n';

    vec4 v1{1.1f, 2.2f, 3.3f, 4.4f};
    vec4 v2{4.4f, 2.2f, 3.3f, 1.1f};
    auto show_vec4 = [](vec4 v) {
        std::cout << "X: " << v.x << ", Y: " << v.y << ", Z: " << v.z << ", W: " << v.w << '\n';
    };
    std::cout << "V1: ";
    show_vec4(v1);
    std::cout << "V2: ";
    show_vec4(v2);
    swap_vec4(&v1, &v2);
    std::cout << "V1: ";
    show_vec4(v1);
    std::cout << "V2: ";
    show_vec4(v2);

    int v = -64;
    std::cout << "Int: "<<v<<" to bad long value: "<<bad_int2long(v)<<'\n';
    std::cout << "Int: "<<v<<" to good long value: "<<int2long(v)<<'\n';
    std::cout << "Int: "<<v<<" shift right bad value: "<<bad_shiftr(v)<<'\n';
    std::cout << "Int: "<<v<<" shift right good value: "<<shiftr(v)<<'\n';

    std::array<int, 8> values;
    std::iota(values.begin(), values.end(), 1);
    std::cout << "Values avg: "<<avg(values.data(), values.size())<<'\n';
    std::array<int, 3> values2{2,3,2};
    std::cout << "Values avg_float: "<<avg_float(values2.data(), values2.size())<<'\n';
    vec3 v_1 = {1,2,3};
    vec3 v_2 = {3,2,-1};
    std::cout << "Dot product: "<<dot_vec3(v_1, v_2)<<'\n';

    float a_1 = 1.0f;
    float b_1 = 2.0f;
    std::cout << "A: "<<a_1<<", B: "<<b_1<<", A^2+B^2: "<<pythagoras (a_1, b_1)<<'\n';
    return 0;
}