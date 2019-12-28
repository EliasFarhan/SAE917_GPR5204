#pragma once
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

#include <ostream>
#include <array>
#include <cmath>
#include <algorithm>
#include <intrinsincs_wrapper.h>

namespace neko {
    template<typename T>
    class Vec2 {
    public:

        Vec2() : x(0), y(0) {
        }

        explicit Vec2(T same) : x(same), y(same) {

        }

        Vec2(T X, T Y) : x(X), y(Y) {
        }

        template<typename U>
        explicit Vec2(const Vec2<U> &vector) : x(static_cast<T>(vector.x)), y(static_cast<T>(vector.y)) {
        }

        T GetSquareMagnitude() { return x * x + y * y; }

        static T Dot(const Vec2<T> &v1, const Vec2<T> &v2) {
            return v1.x * v2.x + v1.y * v2.y;
        }

        Vec2<T> operator+(const Vec2<T> &rhs) const {
            return Vec2<T>(x + rhs.x, y + rhs.y);
        }

        Vec2<T> &operator+=(const Vec2<T> &rhs) {
            this->x += rhs.x;
            this->y += rhs.y;
            return *this;
        }

        Vec2<T> operator-(const Vec2<T> &rhs) const {
            return Vec2<T>(x - rhs.x, y - rhs.y);
        }

        Vec2<T> &operator-=(const Vec2<T> &rhs) {
            this->x -= rhs.x;
            this->y -= rhs.y;
            return *this;
        }

        Vec2<T> operator*(T rhs) const {
            return Vec2<T>(x * rhs, y * rhs);
        }

        Vec2<T> operator*(const Vec2<T> &rhs) const {
            return Vec2<T>(x * rhs.x, y * rhs.y);
        }


        Vec2<T> &operator*=(T rhs) {
            this->x *= rhs;
            this->y *= rhs;
            return *this;
        }

        Vec2<T> operator/(T rhs) const {
            return (*this) * (1.0f / rhs);
        }

        Vec2<T> &operator/=(T rhs) {
            this->x /= rhs;
            this->y /= rhs;
            return *this;
        }

        bool operator==(const Vec2<T> &right) {
            return x == right.x && y == right.y;
        }

        bool operator!=(const Vec2<T> &right) {
            return !(*this == right);
        }

        T GetMagnitude() const;

        Vec2<T> Normalized() const {
            return (*this) / (*this).GetMagnitude();
        }

        Vec2<T> Rotate(T angle) const;

        static Vec2<T> Lerp(const Vec2<T> &v1, const Vec2<T> &v2, T t) {
            return v1 + (v2 - v1) * t;
        }

        friend std::ostream &operator<<(std::ostream &os, const Vec2<T> &dt) {
            os << "Vec2(" << dt.x << "," << dt.y << ")";
            return os;
        }

        template<typename U>
        explicit Vec2(const U &v);

        template<typename U>
        explicit operator U() const;

        template<typename U = float>
        static U AngleBetween(const Vec2 &v1, const Vec2 &v2);

        T x; ///< X coordinate of the vector
        T y; ///< Y coordinate of the vector

        const static Vec2 Zero;
        const static Vec2 One;
    };

    template<typename T>
    Vec2<T> operator*(T lhs, const Vec2<T> &rhs) {
        return Vec2<T>(rhs.x * lhs, rhs.y * lhs);
    }

    using Vec2f = Vec2<float>;


    template<>
    inline float Vec2f::GetMagnitude() const {
        return sqrtf(x * x + y * y);
    }


    template<typename T>
    class alignas(4 * sizeof(T)) Vec3 {
    public:
        T x, y, z;
        const static Vec3 Zero;
        const static Vec3 One;

        Vec3() : x(0), y(0), z(0) {
        }

        explicit Vec3(T same) : x(same), y(same), z(same) {

        }

        Vec3(T X, T Y, T Z) : x(X), y(Y), z(Z) {

        }

        template<typename U>
        explicit
        Vec3(const Vec3<U> &vector) : x(static_cast<T>(vector.x)),
                                      y(static_cast<T>(vector.y)),
                                      z(static_cast<T>(vector.z)) {
        }

        T GetSquareMagnitude() const { return Dot(*this, *this); }


        template<typename ReturnT = float>
        ReturnT GetMagnitude() const {
            return std::sqrt(GetSquareMagnitude());
        }

        static T Dot(const Vec3<T> v1, const Vec3<T> v2) {
            return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
        }

        Vec3<T> operator+(const Vec3<T> &rhs) const {
            return Vec3<T>(x + rhs.x, y + rhs.y, z + rhs.z);
        }

        Vec3<T> &operator+=(const Vec3<T> &rhs) {
            this->x += rhs.x;
            this->y += rhs.y;
            this->z += rhs.z;
            return *this;
        }


        Vec3<T> operator-(const Vec3<T> &rhs) const {
            return Vec3<T>(x - rhs.x, y - rhs.y, z - rhs.z);
        }

        Vec3<T> &operator-=(const Vec3<T> &rhs) {
            this->x -= rhs.x;
            this->y -= rhs.y;
            this->z -= rhs.z;
            return *this;
        }

        Vec3<T> operator*(T rhs) const {
            return Vec3<T>(x * rhs, y * rhs, z * rhs);
        }

        Vec3<T> operator*(const Vec3<T> &rhs) const {
            return Vec3<T>(x * rhs.x, y * rhs.y, z * rhs.z);
        }


        Vec3<T> &operator*=(T rhs) {
            this->x *= rhs;
            this->y *= rhs;
            this->z *= rhs;
            return *this;
        }

        Vec3<T> operator/(T rhs) const {
            return (*this) * (1.0f / rhs);
        }

        Vec3<T> &operator/=(T rhs) {
            *this = *this / rhs;
            return *this;
        }

        bool operator==(const Vec3<T> &right) const {
            return x == right.x && y == right.y && z == right.z;
        }

        bool operator!=(const Vec3<T> &right) const {
            return !(*this == right);
        }

        template<typename U = float>
        static U AngleBetween(const Vec3 &v1, const Vec3 &v2);

    };

    using Vec3f = Vec3<float>;


    template<typename T>
    class alignas(4 * sizeof(T)) Vec4 {
    public:
        T x, y, z, w;
        const static Vec4 Zero;
        const static Vec4 One;

        Vec4() noexcept : x(0), y(0), z(0), w(0) {
        }

        explicit Vec4(T same) : x(same), y(same), z(same), w(same) {

        }

        Vec4(T X, T Y, T Z, T W) : x(X), y(Y), z(Z), w(W) {

        }

        template<typename U>
        explicit Vec4(const Vec4<U> &vector) : x(static_cast<T>(vector.x)),
                                               y(static_cast<T>(vector.y)),
                                               z(static_cast<T>(vector.z)),
                                               w(static_cast<T>(vector.w)) {
        }

        T GetSquareMagnitude() const { return Dot(*this, *this); }


        template<typename ReturnT = float>
        ReturnT GetMagnitude() const {
            return std::sqrt(GetSquareMagnitude());
        }

        static T Dot(const Vec4<T> v1, const Vec4<T> v2) {
            return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
        }

        static T Dot3(const Vec4<T> v1, const Vec4<T> v2) {
            return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
        }

        Vec4<T> operator+(const Vec4<T> &rhs) const {
            return Vec4<T>(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
        }

        Vec4<T> &operator+=(const Vec4<T> &rhs) {
            this->x += rhs.x;
            this->y += rhs.y;
            this->z += rhs.z;
            this->w += rhs.w;
            return *this;
        }

        Vec4<T> operator-(const Vec4<T> &rhs) const {
            return Vec4<T>(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
        }

        Vec4<T> &operator-=(const Vec4<T> &rhs) {
            this->x -= rhs.x;
            this->y -= rhs.y;
            this->z -= rhs.z;
            this->w -= rhs.w;
            return *this;
        }

        Vec4<T> operator*(T rhs) const {
            return Vec4<T>(x * rhs, y * rhs, z * rhs, w * rhs);
        }

        Vec4<T> operator*(const Vec4<T> &rhs) const {
            return Vec4<T>(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
        }


        Vec4<T> &operator*=(T rhs) {
            this->x *= rhs;
            this->y *= rhs;
            this->z *= rhs;
            this->w *= rhs;
            return *this;
        }

        Vec4<T> operator/(T rhs) const {
            return (*this) * (1.0f / rhs);
        }

        Vec4<T> &operator/=(T rhs) {
            *this = *this / rhs;
            return *this;
        }

        bool operator==(const Vec4<T> &right) const {
            return x == right.x && y == right.y && z == right.z && w == right.w;
        }

        bool operator!=(const Vec4<T> &right) const {
            return !(*this == right);
        }


    };

    using Vec4f = Vec4<float>;

    template<typename T, int N>
    struct alignas(N * sizeof(T)) NVec4 {
        std::array<T, N> xs;
        std::array<T, N> ys;
        std::array<T, N> zs;
        std::array<T, N> ws;

        NVec4() : xs{}, ys{}, zs{}, ws{} {};

        explicit NVec4(const std::array<Vec4<T>, N> soaV) {
            for (int i = 0; i < N; i++) {
                xs[i] = soaV[i].x;
                ys[i] = soaV[i].y;
                zs[i] = soaV[i].z;
                ws[i] = soaV[i].w;
            }
        }

        explicit NVec4(const Vec3<T> *soaV) {
            for (int i = 0; i < N; i++) {
                xs[i] = soaV[i].x;
                ys[i] = soaV[i].y;
                zs[i] = soaV[i].z;
                ws[i] = 0.0f;
            }
        }

        explicit NVec4(const Vec4<T> *soaV) {
            for (int i = 0; i < N; i++) {
                xs[i] = soaV[i].x;
                ys[i] = soaV[i].y;
                zs[i] = soaV[i].z;
                ws[i] = soaV[i].w;
            }
        }

        NVec4<T, N> operator-(const Vec4<T> &rhs) const {
            NVec4<T, N> v = this;
            std::for_each(v.xs.begin(), v.xs.end(), [rhs](T x) { return x - rhs.x; });
            std::for_each(v.ys.begin(), v.ys.end(), [rhs](T y) { return y - rhs.y; });
            std::for_each(v.zs.begin(), v.zs.end(), [rhs](T z) { return z - rhs.z; });
            std::for_each(v.ws.begin(), v.ws.end(), [rhs](T w) { return w - rhs.w; });
            return v;
        }

        NVec4<T, N> &operator-=(const Vec4<T> &rhs) {
            std::for_each(xs.begin(), xs.end(), [rhs](T x) { return x - rhs.x; });
            std::for_each(ys.begin(), ys.end(), [rhs](T y) { return y - rhs.y; });
            std::for_each(zs.begin(), zs.end(), [rhs](T z) { return z - rhs.z; });
            std::for_each(ws.begin(), ws.end(), [rhs](T w) { return w - rhs.w; });
            return *this;
        }

        std::array<T, N> GetSquareMagnitude() const {
            std::array<T, N> result;
            for (int i = 0; i < N; i++) {
                result[i] = xs[i] * xs[i] + ys[i] * ys[i] + zs[i] * zs[i] + ws[i] * ws[i];
            }
            return result;
        }

        std::array<T, N> GetMagnitude() const {
            std::array<T, N> result;
            for (int i = 0; i < N; i++) {
                result[i] = xs[i] * xs[i] + ys[i] * ys[i] + zs[i] * zs[i] + ws[i] * ws[i];
            }
            for(int i = 0; i < N; i++)
            {
                result[i] = std::sqrt(result[i]);
            }
            return result;
        }

        std::array<T, N> DotProduct(const Vec4<T> &v) {
            std::array<T, N> result;
            for (int i = 0; i < N; i++) {
                result[i] = xs[i] * v.x + ys[i] * v.y + zs[i] * v.z + ws[i] * v.w;
            }
            for(int i = 0; i < N; i++)
            {
                result[i] = std::sqrt(result[i]);
            }
            return result;
        }

        std::array<T, N> GetMagnitudeIntrinsincs() const;

    };


    using FourVec4f = NVec4<float, 4>;
    using EightVec4f = NVec4<float, 8>;
#ifdef __SSE__

    template<>
    std::array<float, 4> FourVec4f::GetMagnitudeIntrinsincs() const {
        std::array<float, 4> result;
        auto x = _mm_load_ps(xs.data());
        auto y = _mm_load_ps(ys.data());
        auto z = _mm_load_ps(zs.data());
        auto w = _mm_load_ps(ws.data());

        x = _mm_mul_ps(x, x);
        y = _mm_mul_ps(y, y);
        z = _mm_mul_ps(z, z);
        w = _mm_mul_ps(w, w);

        x = _mm_add_ps(x, y);
        z = _mm_add_ps(z, w);
        x = _mm_add_ps(x, z);
        x = _mm_sqrt_ps(x);
        _mm_store_ps(result.data(), x);
        return result;
    }

#endif

#ifdef __AVX2__
    template<>
    std::array<float, 8> EightVec4f::GetMagnitudeIntrinsincs() const {
        std::array<float, 8> result;
        auto x = _mm256_load_ps(xs.data());
        auto y = _mm256_load_ps(ys.data());
        auto z = _mm256_load_ps(zs.data());
        auto w = _mm256_load_ps(ws.data());

        x = _mm256_mul_ps(x, x);
        y = _mm256_mul_ps(y, y);
        z = _mm256_mul_ps(z, z);
        w = _mm256_mul_ps(w, w);

        x = _mm256_add_ps(x, y);
        z = _mm256_add_ps(z, w);
        x = _mm256_add_ps(x, z);
        x = _mm256_sqrt_ps(x);
        _mm256_store_ps(result.data(), x);
        return result;
    }
#endif

}
