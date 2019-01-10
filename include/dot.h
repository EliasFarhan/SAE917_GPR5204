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

#pragma once

#include <x86intrin.h>
#include <cstddef>
#include <cassert>
#include <algorithm>
#include <list>

float MultLocalList(std::list<float>& m1, std::list<float>& m2);
float MultLocal(float * m1, float * m2, const int width);
float MultLocal2(const float * const m1, const float * const m2, const int width);
float MultLocal4 (const float *const m1, const float *const m2, const int width);
float MultLocal8(const float * const m1, const float * const m2, const int width);
float MultLocal16(const float * const m1, const float * const m2, const int width);
float MultLocal32(const float * const m1, const float * const m2, const int width);

#ifdef __SSE__
float dot128(float *x1, float *x2, size_t len);
#endif
#ifdef __SSE4_2__
float dot128dt(float *x1, float *x2, size_t len);
#endif
#ifdef __FMA__
float dot128fma(float *x1, float *x2, size_t len);
#endif
#ifdef __AVX2__
float dot256(float *x1, float *x2, size_t len);
#endif
#ifdef __FMA__
float dot256fma(float *x1, float *x2, size_t len) ;
#endif
#ifdef __AVX512F__
float dot512(float *x1, float *x2, size_t len);
float dot512fma(float *x1, float *x2, size_t len);
float dot512fma2(float *x1, float *x2, size_t len);

#endif