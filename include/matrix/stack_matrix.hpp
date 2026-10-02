#pragma once
#include <array>
#include <vector>
#include <cstddef>
#include "matrix/config.hpp"

class StackMatrix {
    std::array<Elem, KStackN * KStackN> data;

public:
    StackMatrix();
    explicit StackMatrix(Elem fill);

    Elem  at(std::size_t r, std::size_t c) const;
    Elem& at(std::size_t r, std::size_t c);

    static constexpr std::size_t size()  { return KStackN; }
    static constexpr std::size_t bytes() { return KStackN * KStackN * sizeof(Elem); }

    // по ссылке
    static StackMatrix addRef(const StackMatrix& a, const StackMatrix& b);
    static StackMatrix mulRef(const StackMatrix& a, const StackMatrix& b);

    // по значению
    static StackMatrix addVal(StackMatrix a, StackMatrix b);
    static StackMatrix mulVal(StackMatrix a, StackMatrix b);

    // коллекция
    static StackMatrix sumAll(const std::vector<StackMatrix>& ms);

    // доступ к внутреннему буферу для бенчей (только чтение)
    const Elem* raw() const { return data.data(); }
    Elem*       raw()       { return data.data(); }
};