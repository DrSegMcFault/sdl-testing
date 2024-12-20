#pragma once

#include "Util.h"
#include <optional>
#include <algorithm>

struct Camera {
  static constexpr float ZOOM_MAX = 64.f;
  static constexpr float ZOOM_MIN = 8.f;

  // actual pixel values
  int _screen_width_px = 0;
  int _screen_height_px = 0;

  // position in the world
  // top left of the view port (x, y)
  // top left is the begining of the rectangle
  // whose length in world units is viewPortWidthUnits()
  // whose height in world units is viewPortHeightUnits()
  WorldPosition _world_pos;
  WorldSize _world_size;

  // pixels per world unit
  float _pixels_per_unit = 16.f;

  // get the effective rect of this camera
  SDL_Rect getRect() {
    return toRect(_world_pos, _world_size);
  }

  void updateScreenSize(int x_px, int y_px) {
    _screen_width_px = x_px;
    _screen_height_px = y_px;
    updateEffectiveSize();
  }

  void updateEffectiveSize() {
    // update effective world size rect
    _world_size.w = calcWidthUnits();
    _world_size.h = calcHeightUnits();
  }

  // restrict the camera to an object
  struct Object {
    WorldPosition p;
    WorldSize s;
  };
  std::optional<Object> bounds;

  void bindTo(WorldPosition p, WorldSize s) {
    bounds.emplace();
    bounds->p = p;
    bounds->s = s;
  }

  inline float calcWidthUnits () { return _screen_width_px / _pixels_per_unit; }
  inline float calcHeightUnits () { return _screen_height_px / _pixels_per_unit; }

  PixelPosition toPixel(WorldPosition pos) const {
    return {
      static_cast<int>((pos.x - _world_pos.x) * _pixels_per_unit),
      static_cast<int>((pos.y - _world_pos.y) * _pixels_per_unit)
    };
  }

  SDL_Rect toRect(WorldPosition pos, WorldSize s) const {
    return {
      static_cast<int>((pos.x - _world_pos.x) * _pixels_per_unit),
      static_cast<int>((pos.y - _world_pos.y) * _pixels_per_unit),
      static_cast<int>(s.w * _pixels_per_unit),
      static_cast<int>(s.h * _pixels_per_unit)
    };
  }

  PixelSize toPixelSize(WorldSize s) const {
    return {
      static_cast<int>(s.w * _pixels_per_unit),
      static_cast<int>(s.h * _pixels_per_unit)
    };
  }

  WorldPosition toWorld(PixelPosition pixel) const {
    float worldX = (pixel.x / _pixels_per_unit) + _world_pos.x;
    float worldY = (pixel.y / _pixels_per_unit) + _world_pos.y;
    return { worldX, worldY };
  }

  void centerOn(WorldPosition p, WorldSize sz) {
    _world_pos.x = (p.x + (sz.w / 2)) - (_world_size.w / 2);
    _world_pos.y = (p.y + (sz.h / 2)) - (_world_size.h / 2);

    if (bounds) {
      // restrict the cameras position such that it's rect is fully contained within the bounds 
      _world_pos.x = std::clamp(_world_pos.x, bounds->p.x, bounds->p.x + bounds->s.w - _world_size.w);
      _world_pos.y = std::clamp(_world_pos.y, bounds->p.y, bounds->p.y + bounds->s.h - _world_size.h);
    }
  }

  void zoomIn(float amount = 1.f) {
    _pixels_per_unit = std::min(_pixels_per_unit + amount, ZOOM_MAX);
    updateEffectiveSize();
  }

  void zoomOut(float amount = 1.f) {
    _pixels_per_unit = std::max(_pixels_per_unit - amount, ZOOM_MIN);
    updateEffectiveSize();
  }
};
