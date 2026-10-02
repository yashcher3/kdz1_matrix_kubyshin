#include "matrix/config.hpp"
#include "matrix/stack_matrix.hpp"
#include "matrix/heap_matrix.hpp"
#include "matrix/timer.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>

static constexpr int WARMUP = 5;
static constexpr int RUNS   = 5;

template <typename MakeFn>
static std::int64_t measureNanosPerOp(int K, MakeFn make) {
    std::vector<std::int64_t> samples;

    // Прогрев
    for (int w = 0; w < WARMUP; ++w) {
        auto obj = make();
        doNotOptimize(obj);
    }

    for (int r = 0; r < RUNS; ++r) {
        Timer t;
        std::uint64_t sum = 0;
        for (int i = 0; i < K; ++i) {
            auto obj = make();
            std::size_t n = obj.size();   // StackMatrix: KStackN, HeapMatrix: n_
            sum += obj.at(0, 0);
            sum += obj.at(n / 2, n / 2);
            sum += obj.at(n - 1, n - 1);
            doNotOptimize(obj);
        }
        doNotOptimize(sum);
        samples.push_back(t.elapsedNs() / K);
    }
    std::sort(samples.begin(), samples.end());
    return samples[samples.size() / 2];
}

int main() {
    constexpr int K = 1000;

    auto s_ns = measureNanosPerOp(K, [] { return StackMatrix(3); });
    std::cout << "StackMatrix N=" << KStackN << " ns/op = " << s_ns << "\n";

    for (std::size_t n : {50u, 100u, 250u, 500u, 1000u, 2000u}) {
        auto h_ns = measureNanosPerOp(K, [n] { return HeapMatrix(n, 3); });
        std::cout << "HeapMatrix n=" << n << " ns/op = " << h_ns << "\n";
    }
    return 0;
}