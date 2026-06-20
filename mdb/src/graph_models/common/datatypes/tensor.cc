
#include "tensor.h"

#include "tensor_operations.h"

#include <cassert>
#include <cmath>

template<typename T>
Tensor<T>::Tensor(std::vector<T, AlignedAllocator<T>>&& tensor_) :
    data_ { std::move(tensor_) }
{ }

template<typename T>
Tensor<T> Tensor<T>::from_literal(std::string_view literal, bool* error)
{
    // Creates a decimal from its string representation of the form [n0, n1, n2, ..., nN]
    *error = false;

    if (literal.empty()) {
        // Empty string
        *error = true;
        return Tensor<T>();
    }

    if (literal.front() != '[' || literal.back() != ']') {
        // Missing brackets
        *error = true;
        return Tensor<T>();
    }

    if (literal.size() == 2) {
        // Empty tensor
        return Tensor<T>();
    }

    std::vector<T, AlignedAllocator<T>> tensor;

    bool EXPECT_COMMA = false;
    std::size_t i = 1;
    while (i < literal.size() - 1) {
        // Skip whitespaces
        while (std::isspace(literal[i])) {
            ++i;
        }
        if (i == literal.size() - 1) {
            break;
        }

        // Verify separator/end
        if (EXPECT_COMMA) {
            if (literal[i] == ',') {
                ++i;
                EXPECT_COMMA = false;
                continue;
            } else {
                *error = true;
                return Tensor<T>();
            }
        }

        // Parse values
        try {
            std::size_t bytes_read = 0;
            if constexpr (std::is_same_v<T, float>) {
                tensor.emplace_back(std::stof(&literal[i], &bytes_read));
            } else if constexpr (std::is_same_v<T, double>) {
                tensor.emplace_back(std::stod(&literal[i], &bytes_read));
            } else {
                throw std::runtime_error("Unhandled tensor type");
            }
            i += bytes_read;
            EXPECT_COMMA = true;
        } catch (...) {
            *error = true;
            return Tensor<T>();
        }
    }

    return Tensor<T>(std::move(tensor));
}

template<typename T>
Tensor<T> Tensor<T>::from_external(const char* bytes, std::size_t size, bool* error)
{
    if (size % sizeof(T) != 0) {
        assert(false);
        *error = true;
        return Tensor<T>();
    }

    std::vector<T, AlignedAllocator<T>> tensor;
    tensor.resize(size / sizeof(T));
    std::memcpy(tensor.data(), bytes, size);
    return Tensor<T>(std::move(tensor));
}

template<typename T>
int Tensor<T>::compare(const Tensor<T>& lhs, const Tensor<T>& rhs)
{
    const auto min_size = std::min(lhs.size(), rhs.size());
    for (std::size_t i = 0; i < min_size; ++i) {
        if (lhs.data_[i] != rhs.data_[i]) {
            return lhs.data_[i] < rhs.data_[i] ? -1 : 1;
        }
    }
    if (lhs.size() != rhs.size()) {
        return lhs.size() < rhs.size() ? -1 : 1;
    }
    return 0;
}

template<typename T>
Tensor<T>& Tensor<T>::negate()
{
    TensorOperations::negate(data(), size());
    return *this;
}

template<typename T>
Tensor<T>& Tensor<T>::abs()
{
    TensorOperations::abs(data(), size());
    return *this;
}

template<typename T>
Tensor<T>& Tensor<T>::ceil()
{
    TensorOperations::ceil(data(), size());
    return *this;
}

template<typename T>
Tensor<T>& Tensor<T>::floor()
{
    TensorOperations::floor(data(), size());
    return *this;
}

// Special case where SPARQL-1.1 ROUND != std::round
// SPARQL ROUND(-0.5f) == 0
// std::round(-0.5f) == -1
template<typename T>
Tensor<T>& Tensor<T>::sparql11_round()
{
    TensorOperations::sparql11_round(data(), size());
    return *this;
}

template<typename T>
Tensor<T>& Tensor<T>::multiplicative_inverse()
{
    TensorOperations::multiplicative_inverse(data(), size());
    return *this;
}

template<typename T>
Tensor<T>& Tensor<T>::sqrt()
{
    TensorOperations::sqrt(data(), size());
    return *this;
}

template<typename T>
Tensor<T>& Tensor<T>::add(const Tensor<T>& rhs)
{
    assert(size() == rhs.size());
    TensorOperations::add(data(), rhs.data(), data(), size());
    return *this;
}

template<typename T>
Tensor<T>& Tensor<T>::subtract(const Tensor<T>& rhs)
{
    assert(size() == rhs.size());
    TensorOperations::subtract(data(), rhs.data(), data(), size());
    return *this;
}

template<typename T>
Tensor<T>& Tensor<T>::multiply(const Tensor<T>& rhs)
{
    assert(size() == rhs.size());
    TensorOperations::multiply(data(), rhs.data(), data(), size());
    return *this;
}

template<typename T>
Tensor<T>& Tensor<T>::divide(const Tensor<T>& rhs)
{
    assert(size() == rhs.size());
    TensorOperations::divide(data(), rhs.data(), data(), size());
    return *this;
}

template<typename T>
T Tensor<T>::dot(const Tensor<T>& rhs) const
{
    assert(size() == rhs.size());
    return TensorOperations::dot(data(), rhs.data(), size());
}

template<typename T>
Tensor<T>& Tensor<T>::add(T rhs)
{
    TensorOperations::add(data(), rhs, data(), size());
    return *this;
}

template<typename T>
Tensor<T>& Tensor<T>::subtract(T rhs)
{
    TensorOperations::subtract(data(), rhs, data(), size());
    return *this;
}

template<typename T>
Tensor<T>& Tensor<T>::multiply(T rhs)
{
    TensorOperations::multiply(data(), rhs, data(), size());
    return *this;
}

template<typename T>
Tensor<T>& Tensor<T>::divide(T rhs)
{
    TensorOperations::divide(data(), rhs, data(), size());
    return *this;
}

template<typename T>
Tensor<T>& Tensor<T>::pow(T rhs)
{
    TensorOperations::pow(data(), rhs, data(), size());
    return *this;
}

template<typename T>
T Tensor<T>::sum() const
{
    return TensorOperations::sum(data(), size());
}

template<typename T>
T Tensor<T>::euclidean_distance(const Tensor<T>& rhs) const
{
    assert(size() == rhs.size());
    return TensorOperations::euclidean_distance(data(), rhs.data(), size());
}

template<typename T>
T Tensor<T>::manhattan_distance(const Tensor<T>& rhs) const
{
    assert(size() == rhs.size());
    return TensorOperations::manhattan_distance(data(), rhs.data(), size());
}

template<typename T>
T Tensor<T>::cosine_distance(const Tensor<T>& rhs) const
{
    assert(size() == rhs.size());
    return TensorOperations::cosine_distance(data(), rhs.data(), size());
}

template<typename T>
T Tensor<T>::cosine_similarity(const Tensor<T>& rhs) const
{
    assert(size() == rhs.size());
    return TensorOperations::cosine_similarity(data(), rhs.data(), size());
}

template class Tensor<float>;
template class Tensor<double>;
