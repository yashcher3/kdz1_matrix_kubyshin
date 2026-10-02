#include "matrix/heap_matrix.hpp"
#include <stdexcept>
#include <algorithm>

HeapMatrix::HeapMatrix(std::size_t n) : n_(n) {
    if (n == 0) throw std::invalid_argument("HeapMatrix: n must be > 0");
    data_ = new Elem[n * n]{};   // value-init → нули
}

HeapMatrix::HeapMatrix(std::size_t n, Elem fill) : n_(n) {
    if (n == 0) throw std::invalid_argument("HeapMatrix: n must be > 0");
    data_ = new Elem[n * n];
    std::fill(data_, data_ + n * n, fill);
}

HeapMatrix::~HeapMatrix() {
    delete[] data_;
}

Elem HeapMatrix::at(std::size_t r, std::size_t c) const {
    if (r >= n_ || c >= n_)
        throw std::out_of_range("HeapMatrix::at: index out of range");
    return data_[r * n_ + c];
}

Elem& HeapMatrix::at(std::size_t r, std::size_t c) {
    if (r >= n_ || c >= n_)
        throw std::out_of_range("HeapMatrix::at: index out of range");
    return data_[r * n_ + c];
}

HeapMatrix* HeapMatrix::addPtr(const HeapMatrix* a, const HeapMatrix* b) {
    if (!a || !b || a->n_ != b->n_)
        throw std::invalid_argument("HeapMatrix::addPtr: size mismatch");
    std::size_t n = a->n_;
    auto* r = new HeapMatrix(n);
    for (std::size_t i = 0; i < n * n; ++i)
        r->data_[i] = a->data_[i] + b->data_[i];
    return r;
}

HeapMatrix* HeapMatrix::mulPtr(const HeapMatrix* a, const HeapMatrix* b) {
    if (!a || !b || a->n_ != b->n_)
        throw std::invalid_argument("HeapMatrix::mulPtr: size mismatch");
    std::size_t n = a->n_;
    auto* r = new HeapMatrix(n);
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t k = 0; k < n; ++k) {
            Elem aik = a->data_[i * n + k];
            for (std::size_t j = 0; j < n; ++j)
                r->data_[i * n + j] += aik * b->data_[k * n + j];
        }
    }
    return r;
}

HeapMatrix* HeapMatrix::sumAll(const std::vector<const HeapMatrix*>*& ms) {
    if (!ms || ms->empty())
        throw std::invalid_argument("HeapMatrix::sumAll: empty");
    std::size_t n = (*ms)[0]->size();
    auto* acc = new HeapMatrix(n);
    for (const auto* m : *ms) {
        if (m->size() != n)
            throw std::invalid_argument("HeapMatrix::sumAll: size mismatch");
        for (std::size_t i = 0; i < n * n; ++i)
            acc->data_[i] += m->data_[i];
    }
    return acc;
}