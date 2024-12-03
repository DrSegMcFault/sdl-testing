#pragma once
#include <ranges>

template<typename T>
constexpr auto range(T stop) {
  return std::ranges::views::iota(static_cast<T>(0), stop);
}

template<typename T>
constexpr auto range(T start, T stop) {
  return std::ranges::views::iota(start, stop);
}

using unit_type = float;
struct WorldPosition {
  unit_type x;
  unit_type y;

  WorldPosition() = default;
  WorldPosition(float x, float y ) : x(x), y(y) {}
};

struct WorldSize {
  unit_type w;
  unit_type h;
};

struct WorldRect {
  unit_type x;
  unit_type y;
  unit_type w;
  unit_type h;
};

struct PixelPosition {
  int x,y;
};

struct PixelSize {
  int w,h;
};

struct PixelRect {
  int x,y,w,h;
};