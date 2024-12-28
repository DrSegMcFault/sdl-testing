#pragma once

#include <ranges>

namespace util {

template<typename T>
constexpr auto range(T stop) {
  return std::ranges::views::iota(static_cast<T>(0), stop);
}

template<typename T>
constexpr auto range(T start, T stop) {
  return std::ranges::views::iota(start, stop);
}

} // namespace util
