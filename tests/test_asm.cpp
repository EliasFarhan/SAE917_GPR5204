//
// Created by efarhan on 23/12/2019.
//
#include <iostream>
#include <asm_func.h>

int main()
{
    int a = 3;
    int b = 2;
    std::cout << "A: "<< a << ", B: "<<b<<'\n';
    swap_int(&a, &b);
    std::cout << "A: "<< a << ", B: "<<b<<'\n';

    float f1 = 3.5f;
    float f2 = 2.2f;
    std::cout << "F1: "<< f1 << ", F2: "<<f2<<'\n';
    swap_float(&f1, &f2);
    std::cout << "F1: "<< f1 << ", F2: "<<f2<<'\n';
    return 0;
}