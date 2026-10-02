#include "matrix/config.hpp"
#include "matrix/stack_matrix.hpp"
#include "matrix/heap_matrix.hpp"
#include "matrix/timer.hpp"
#include <iostream>
#include <vector>
#include <cstdint>

static std::uint64_t cksum(const StackMatrix& m) {
    std::uint64_t s = 0;
    const Elem* p = m.raw();
    for (std::size_t i = 0; i < KStackN * KStackN; i += 256) s += p[i];
    return s;
}

static std::uint64_t cksum(const HeapMatrix& m) {
    std::uint64_t s = 0;
    for (std::size_t i = 0; i < m.size(); i += 256)
        s += m.at(0, i);   // грубая, но валидная
    return s;
}

#define TIME(expr) do { \
    Timer t; auto r = (expr); \
    std::cout << #expr << " time_ns=" << t.elapsedNs() \
              << " cksum=" << cksum(r) << "\n"; } while(0)

int main() {
    StackMatrix a(1), b(2);

    TIME(StackMatrix::addRef(a, b));
    TIME(StackMatrix::addVal(a, b));
    TIME(StackMatrix::mulRef(a, b));
    TIME(StackMatrix::mulVal(a, b));

    // сумма коллекции из 20
    std::vector<StackMatrix> v(20, StackMatrix(1));
    {
        Timer t;
        auto r = StackMatrix::sumAll(v);
        std::cout << "sumAll(stack) time_ns=" << t.elapsedNs()
                  << " cksum=" << cksum(r) << "\n";
    }

    // куча
    HeapMatrix ha(KStackN, 1), hb(KStackN, 2);
    {
        Timer t;
        auto* r = HeapMatrix::addPtr(&ha, &hb);
        std::cout << "addPtr time_ns=" << t.elapsedNs()
                  << " cksum=" << cksum(*r) << "\n";
        delete r;
    }
    {
        Timer t;
        auto* r = HeapMatrix::mulPtr(&ha, &hb);
        std::cout << "mulPtr time_ns=" << t.elapsedNs()
                  << " cksum=" << cksum(*r) << "\n";
        delete r;
    }
    {
        std::vector<const HeapMatrix*> vp(20, &ha);
        const std::vector<const HeapMatrix*>* pvp = &vp;
        Timer t;
        auto* r = HeapMatrix::sumAll(pvp);
        std::cout << "sumAll(heap) time_ns=" << t.elapsedNs()
                  << " cksum=" << cksum(*r) << "\n";
        delete r;
    }

    // куча на больших N (там, где стек не работает)
    for (std::size_t n : {1000u, 2000u}) {
        HeapMatrix x(n, 1), y(n, 2);
        Timer t;
        auto* r = HeapMatrix::addPtr(&x, &y);
        std::cout << "addPtr n=" << n << " time_ns=" << t.elapsedNs()
                  << " cksum=" << cksum(*r) << "\n";
        delete r;
    }
    return 0;
}