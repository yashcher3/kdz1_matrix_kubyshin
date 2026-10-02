#include "matrix/config.hpp"
#include "matrix/stack_matrix.hpp"
#include "matrix/heap_matrix.hpp"
#include "matrix/timer.hpp"
#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cmath>
#include <sys/resource.h>

static std::size_t stackLimitBytes() {
    rlimit rl{};
    getrlimit(RLIMIT_STACK, &rl);
    if (rl.rlim_cur == RLIM_INFINITY) return 0;
    return static_cast<std::size_t>(rl.rlim_cur);
}

static std::uint64_t checksum(const StackMatrix& m) {
    std::uint64_t s = 0;
    const Elem* p = m.raw();
    for (std::size_t i = 0; i < KStackN * KStackN; i += 1024) s += p[i];
    return s;
}

// Демонстрация shallow copy (п.5.2) — вынести ОТДЕЛЬНО, падает по сигналу.
__attribute__((noinline))
static void demo_double_free() {
    std::cout << "\n=== 5.2: shallow copy ===\n" << std::flush;
    HeapMatrix a(10, 1);
    HeapMatrix b = a;          // <-- намеренное нарушение
    std::cout << "b.size()=" << b.size() << "\n" << std::flush;
    std::cout << "before scope exit...\n" << std::flush;
}

int main(int argc, char** argv) {
    std::size_t lim = stackLimitBytes();
    std::cout << "ulimit -s bytes = " << lim
              << " (" << (lim / 1024) << " KiB)\n";

    // --- A.0: расчёт границ ---
    auto maxN = [&](double matrices) {
        double b = static_cast<double>(lim) / (matrices * sizeof(Elem));
        return static_cast<std::size_t>(std::floor(std::sqrt(b)));
    };
    std::cout << "Predicted max N for 1 matrix: " << maxN(1) << "\n";
    std::cout << "Predicted max N for 3 matrix: " << maxN(3) << "\n";
    std::cout << "Predicted max N for 5 matrix: " << maxN(5) << "\n";

    // --- A.1: работает ли при KStackN=500 ---
    if (argc > 1 && std::string(argv[1]) == "mulval") {
        std::cout << "--- mulVal start ---\n" << std::flush;
        StackMatrix a(1), b(2);
        Timer t;
        StackMatrix c = StackMatrix::mulVal(a, b);
        std::cout << "mulVal time ns = " << t.elapsedNs()
                  << ", cksum=" << checksum(c) << "\n";
    } else if (argc > 1 && std::string(argv[1]) == "mulref") {
        std::cout << "--- mulRef start ---\n" << std::flush;
        StackMatrix a(1), b(2);
        Timer t;
        StackMatrix c = StackMatrix::mulRef(a, b);
        std::cout << "mulRef time ns = " << t.elapsedNs()
                  << ", cksum=" << checksum(c) << "\n";
    } else if (argc > 1 && std::string(argv[1]) == "one") {
        StackMatrix a(7);
        std::cout << "one matrix cksum=" << checksum(a) << "\n";
    } else if (argc > 1 && std::string(argv[1]) == "heap") {
        // A.4
        for (std::size_t n : {1000u, 2000u, 4000u}) {
            Timer t;
            HeapMatrix m(n, 1);
            auto ns = t.elapsedNs();
            std::cout << "HeapMatrix n=" << n
                      << " bytes=" << m.bytes()
                      << " (" << (m.bytes() >> 20) << " MiB)"
                      << " ctor_ns=" << ns << "\n";
        }
    } else if (argc > 1 && std::string(argv[1]) == "shallow") {
        demo_double_free();
    } else {
        std::cout << "usage: bench_limits [one|mulref|mulval|heap|shallow]\n";
    }
    return 0;
}