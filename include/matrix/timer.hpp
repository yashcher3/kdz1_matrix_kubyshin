#pragma once
#include <chrono>
#include <cstdint>

class Timer {
    std::chrono::steady_clock::time_point start_;
public:
    Timer() : start_(std::chrono::steady_clock::now()) {}
    std::int64_t elapsedNs() const {
        using namespace std::chrono;
        return duration_cast<nanoseconds>(steady_clock::now() - start_).count();
    }
};

// Защита от выбрасывания кода оптимизатором (копировать как есть).
template <typename T>
inline void doNotOptimize(T const& value) {
    asm volatile("" : : "r,m"(value) : "memory");
}