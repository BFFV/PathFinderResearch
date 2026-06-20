// TODO: Optimize vectorization by using aligned intrinsics explicitly (e.g. SSE, AVX)

#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>

namespace TensorOperations {

template<typename T>
inline void negate(T* data, std::size_t size)
{
    const T* end = data + size;
    for (; data != end; ++data) {
        *data = -*data;
    }
}

template<typename T>
inline void abs(T* data, std::size_t size)
{
    const T* end = data + size;
    for (; data != end; ++data) {
        *data = std::abs(*data);
    }
}

template<typename T>
inline void ceil(T* data, std::size_t size)
{
    const T* end = data + size;
    for (; data != end; ++data) {
        *data = std::ceil(*data);
    }
}

template<typename T>
inline void floor(T* data, std::size_t size)
{
    const T* end = data + size;
    for (; data != end; ++data) {
        *data = std::floor(*data);
    }
}

template<typename T>
inline void sparql11_round(T* data, std::size_t size)
{
    const T* end = data + size;
    for (; data != end; ++data) {
        T whole;
        const T frac = std::modf(*data, &whole);
        if (frac == T(-0.5)) {
            *data = whole;
        } else {
            *data = std::round(*data);
        }
    }
}

template<typename T>
inline void multiplicative_inverse(T* data, std::size_t size)
{
    const T* end = data + size;
    for (; data != end; ++data) {
        *data = T(1) / *data;
    }
}

template<typename T>
inline void sqrt(T* data, std::size_t size)
{
    const T* end = data + size;
    for (; data != end; ++data) {
        *data = std::sqrt(*data);
    }
}

template<typename T>
inline void add(const T* lhs, const T* rhs, T* res, std::size_t size)
{
    const T* end = lhs + size;
    for (; lhs != end; ++lhs, ++rhs, ++res) {
        *res = *lhs + *rhs;
    }
}

template<typename T>
inline void subtract(const T* lhs, const T* rhs, T* res, std::size_t size)
{
    const T* end = lhs + size;
    for (; lhs != end; ++lhs, ++rhs, ++res) {
        *res = *lhs - *rhs;
    }
}

template<typename T>
inline void multiply(const T* lhs, const T* rhs, T* res, std::size_t size)
{
    const T* end = lhs + size;
    for (; lhs != end; ++lhs, ++rhs, ++res) {
        *res = *lhs * *rhs;
    }
}

template<typename T>
inline void divide(const T* lhs, const T* rhs, T* res, std::size_t size)
{
    const T* end = lhs + size;
    for (; lhs != end; ++lhs, ++rhs, ++res) {
        *res = *lhs / *rhs;
    }
}

template<typename T>
inline T dot(const T* lhs, const T* rhs, std::size_t size)
{
    const T* end = lhs + size;
    T sum = 0;
    for (; lhs != end; ++lhs, ++rhs) {
        sum += *lhs * *rhs;
    }
    return sum;
}

template<typename T>
inline void add(const T* lhs, T rhs, T* res, std::size_t size)
{
    const T* end = lhs + size;
    for (; lhs != end; ++lhs, ++res) {
        *res = *lhs + rhs;
    }
}

template<typename T>
inline void subtract(const T* lhs, T rhs, T* res, std::size_t size)
{
    const T* end = lhs + size;
    for (; lhs != end; ++lhs, ++res) {
        *res = *lhs - rhs;
    }
}

template<typename T>
inline void multiply(const T* lhs, T rhs, T* res, std::size_t size)
{
    const T* end = lhs + size;
    for (; lhs != end; ++lhs, ++res) {
        *res = *lhs * rhs;
    }
}

template<typename T>
inline void divide(const T* lhs, T rhs, T* res, std::size_t size)
{
    const T* end = lhs + size;
    for (; lhs != end; ++lhs, ++res) {
        *res = *lhs / rhs;
    }
}

template<typename T>
inline void pow(const T* lhs, T rhs, T* res, std::size_t size)
{
    const T* end = lhs + size;
    for (; lhs != end; ++lhs, ++res) {
        *res = std::pow(*lhs, rhs);
    }
}

template<typename T>
inline T sum(const T* data, std::size_t size)
{
    const T* end = data + size;
    T sum { 0 };
    for (; data != end; ++data) {
        sum += *data;
    }
    return sum;
}

template<typename T>
inline T euclidean_distance(const T* lhs, const T* rhs, std::size_t size)
{
    const T* end = lhs + size;
    T sum = 0;
    for (; lhs != end; ++lhs, ++rhs) {
        const T diff = *lhs - *rhs;
        sum += diff * diff;
    }
    return std::sqrt(sum);
}

template<typename T>
inline T manhattan_distance(const T* lhs, const T* rhs, std::size_t size)
{
    const T* end = lhs + size;
    T sum = 0;
    for (; lhs != end; ++lhs, ++rhs) {
        const T diff = *lhs - *rhs;
        sum += std::abs(diff);
    }
    return sum;
}

template<typename T>
inline T cosine_similarity(const T* lhs, const T* rhs, std::size_t size)
{
    const T* end = lhs + size;
    T ab = 0;
    T aa = 0;
    T bb = 0;
    for (; lhs != end; ++lhs, ++rhs) {
        ab += *lhs * *rhs;
        aa += *lhs * *lhs;
        bb += *rhs * *rhs;
    }
    const T aabb = aa * bb;
    if (aabb > 0) {
        // Due to floating point precision we could underflow -1.0 or overflow 1.0, so std::min and std::max is a must
        return std::min(std::max(ab / std::sqrt(aabb), T(-1)), T(1));
    }
    // We consider the zero tensor perfectly dissimilar to any other
    return T(-1);
}

template<typename T>
inline T cosine_distance(const T* lhs, const T* rhs, std::size_t size)
{
    return 1 - cosine_similarity(lhs, rhs, size);
}

} // namespace TensorOperations
