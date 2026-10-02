#pragma once
#include <cstddef>
#include <cstdint>

using Elem = std::uint32_t;                      // тип элемента матрицы

// Рабочий размер стековой матрицы (меняем при А.2, А.3).
inline constexpr std::size_t KStackN = 500;