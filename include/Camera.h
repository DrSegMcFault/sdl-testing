#pragma once

#include "Util.h"

struct Camera {
  static constexpr float ZOOM_MAX = 64.f;
  static constexpr float ZOOM_MIN = 8.f;

  // actual pixel values
  int screen_width_px = 0;
  int screen_height_px = 0;

  // position in the world
  // top left of the view port (x, y)
  // top left is the begining of the rectangle
  // whose length in world units is viewPortWidthUnits()
  // whose height in world units is viewPortHeightUnits()
  WorldPosition world;

  // pixels per world unit
  float pixels_per_unit = 16.f;

  inline float viewPortWidthUnits () { return screen_width_px / pixels_per_unit; }
  inline float viewPortHeightUnits () { return screen_height_px / pixels_per_unit; }

  PixelPosition toPixel(WorldPosition pos) const {
    return {
      static_cast<int>((pos.x - world.x) * pixels_per_unit),
      static_cast<int>((pos.y - world.y) * pixels_per_unit)
    };
  }

  SDL_Rect toRect(WorldPosition pos, WorldSize s) const {
    return {
      static_cast<int>((pos.x - world.x) * pixels_per_unit),
      static_cast<int>((pos.y - world.y) * pixels_per_unit),
      static_cast<int>(s.w * pixels_per_unit),
      static_cast<int>(s.h * pixels_per_unit)
    };
  }

  PixelSize toPixelSize(WorldSize s) const {
    return {
      static_cast<int>(s.w * pixels_per_unit),
      static_cast<int>(s.h * pixels_per_unit)
    };
  }

  WorldPosition toWorld(PixelPosition pixel) const {
    float worldX = (pixel.x / pixels_per_unit) + world.x;
    float worldY = (pixel.y / pixels_per_unit) + world.y;
    return { worldX, worldY };
  }

  void centerOn(WorldPosition p, WorldSize sz) {
    world.x = (p.x + (sz.w / 2)) - (viewPortWidthUnits() / 2);
    world.y = (p.y + (sz.h / 2)) - (viewPortHeightUnits() / 2);
  }

  void zoomIn(float amount = 1.f) {
    pixels_per_unit = std::min(pixels_per_unit + amount, ZOOM_MAX);
  }

  void zoomOut(float amount = 1.f) {
    pixels_per_unit = std::max(pixels_per_unit - amount, ZOOM_MIN);
  }
};
