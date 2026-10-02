#include "matrix/stack_matrix.hpp"
#include <stdexcept>
#include <algorithm>

StackMatrix::StackMatrix() { data.fill(0); }

StackMatrix::StackMatrix(Elem fill) { data.fill(fill); }

Elem StackMatrix::at(std::size_t r, std::size_t c) const {
    if (r >= KStackN || c >= KStackN)
        throw std::out_of_range("StackMatrix::at: index out of range");
    return data[r * KStackN + c];
}

Elem& StackMatrix::at(std::size_t r, std::size_t c) {
    if (r >= KStackN || c >= KStackN)
        throw std::out_of_range("StackMatrix::at: index out of range");
    return data[r * KStackN + c];
}

StackMatrix StackMatrix::addRef(const StackMatrix& a, const StackMatrix& b) {
    StackMatrix r;
    for (std::size_t i = 0; i < KStackN * KStackN; ++i)
        r.data[i] = a.data[i] + b.data[i];
    return r;
}

StackMatrix StackMatrix::mulRef(const StackMatrix& a, const StackMatrix& b) {
    StackMatrix r;
    for (std::size_t i = 0; i < KStackN; ++i) {
        for (std::size_t k = 0; k < KStackN; ++k) {
            Elem aik = a.data[i * KStackN + k];
            for (std::size_t j = 0; j < KStackN; ++j)
                r.data[i * KStackN + j] += aik * b.data[k * KStackN + j];
        }
    }
    return r;
}

StackMatrix StackMatrix::addVal(StackMatrix a, StackMatrix b) {
    StackMatrix r;
    for (std::size_t i = 0; i < KStackN * KStackN; ++i)
        r.data[i] = a.data[i] + b.data[i];
    return r;
}

StackMatrix StackMatrix::mulVal(StackMatrix a, StackMatrix b) {
    StackMatrix r;
    for (std::size_t i = 0; i < KStackN; ++i) {
        for (std::size_t k = 0; k < KStackN; ++k) {
            Elem aik = a.data[i * KStackN + k];
            for (std::size_t j = 0; j < KStackN; ++j)
                r.data[i * KStackN + j] += aik * b.data[k * KStackN + j];
        }
    }
    return r;
}

StackMatrix StackMatrix::sumAll(const std::vector<StackMatrix>& ms) {
    if (ms.empty()) throw std::invalid_argument("StackMatrix::sumAll: empty");
    StackMatrix r = ms[0];
    for (std::size_t m = 1; m < ms.size(); ++m)
        r = addRef(r, ms[m]);
    return r;
}