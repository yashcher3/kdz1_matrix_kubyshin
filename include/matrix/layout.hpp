#pragma once
#include <cstdint>
#include <cstddef>

struct Naive {
    bool          b;
    std::uint64_t u64a;
    std::uint32_t u32;
    char          c;
    std::uint64_t u64b;
};

struct Packed {
    std::uint64_t u64a;
    std::uint64_t u64b;
    std::uint32_t u32;
    bool          b;
    char          c;
};

#pragma pack(push, 1)
struct ForcedNaive {
    bool          b;
    std::uint64_t u64a;
    std::uint32_t u32;
    char          c;
    std::uint64_t u64b;
};
#pragma pack(pop)

// Предсказание ДО компиляции (x86-64, GCC).
static_assert(sizeof(Naive)       == 32, "Naive expected 32");
static_assert(sizeof(Packed)      == 24, "Packed expected 24");
static_assert(sizeof(ForcedNaive) == 22, "ForcedNaive expected 22");