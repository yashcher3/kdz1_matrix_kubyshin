#include "matrix/layout.hpp"
#include "matrix/timer.hpp"
#include <iostream>
#include <vector>
#include <cstdint>
#include <cstddef>

template <typename T>
static std::uint64_t traverse(const std::vector<T>& arr) {
    std::uint64_t s = 0;
    for (const auto& x : arr) s += x.u64b;
    return s;
}

template <typename T>
static void run(const char* name, std::size_t N) {
    std::vector<T> arr(N);
    // заполним
    for (std::size_t i = 0; i < N; ++i) {
        if constexpr (std::is_same_v<T, Naive> || std::is_same_v<T, ForcedNaive>)
            arr[i].u64b = i;
        else
            arr[i].u64b = i;
    }
    Timer t;
    auto s = traverse(arr);
    auto ns = t.elapsedNs();
    std::cout << name
              << " sizeof=" << sizeof(T)
              << " total_MiB=" << (sizeof(T) * N) / (1024.0 * 1024.0)
              << " time_ns=" << ns
              << " ns/elem=" << (double)ns / N
              << " cksum=" << s << "\n";
}

int main() {
    constexpr std::size_t N = 5'000'000;
    run<Naive>("Naive", N);
    run<Packed>("Packed", N);
    run<ForcedNaive>("ForcedNaive", N);
    return 0;
}