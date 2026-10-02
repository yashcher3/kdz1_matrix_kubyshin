#pragma once
#include <cstddef>
#include <vector>
#include "matrix/config.hpp"

class HeapMatrix {
    std::size_t n_;
    Elem* data_;

public:
    explicit HeapMatrix(std::size_t n);
    HeapMatrix(std::size_t n, Elem fill);
    ~HeapMatrix();

    // Копирование НЕ удаляем — нужно для демонстрации в 5.2.
    // (Компилятор сгенерирует дефолтный shallow copy.)

    Elem  at(std::size_t r, std::size_t c) const;
    Elem& at(std::size_t r, std::size_t c);

    std::size_t size()  const { return n_; }
    std::size_t bytes() const { return n_ * n_ * sizeof(Elem); }

    static HeapMatrix* addPtr(const HeapMatrix* a, const HeapMatrix* b);
    static HeapMatrix* mulPtr(const HeapMatrix* a, const HeapMatrix* b);
    static HeapMatrix* sumAll(const std::vector<const HeapMatrix*>*& ms);
};