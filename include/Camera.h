#pragma once

#include <optional>
#include <algorithm>

#include "Util.h"

// there may be a need for the camera to take up only a portion
// of the screen.
// Currently, the camera's viewport is the entire screen, 
// with no way to change or configure it.
// I need to implement a way to have the camera be able
// take up a specified amount of the screen.
// this means that all things that are seen by the camera 
// need to be drawn at an offset from the cameras position on screen

// Step 1. What are the current assumptions in this class?
//  a. The 'location' of the camera on the screen is always 0, 0
//  Something in toRect() is fishy, 
//      _pixels_per_unit is sus
//      The x,y calculation specifially
//      

// Previous Assumption was that the _view_px = _screen_px
// and that _view_offset = 0

struct Camera {
  static constexpr float ZOOM_MAX = 64.f;
  static constexpr float ZOOM_MIN = 1.f;

  // window size in pixels, gets updated when the window is resized
  // it is literally just the current window dimensions
  int _screen_width_px = 0;
  int _screen_height_px = 0;

  // width and height factors
  float _view_width_factor = 1.f;
  float _view_height_factor = 1.f;

  // view size in pixels - how big does the camera appear on the screen?
  int _view_width_px = 0;
  int _view_height_px = 0;

  // where is the camera positioned within the screen? - usually it is 0,0
  // view offset relative to 0,0
  int _view_offset_x_px = 0;
  int _view_offset_y_px = 0;

  // position in the world
  WorldPosition _world_pos;
  // 'size' in the world - how many world units can the camera see
  WorldSize _world_size;

  // pixels per world unit
  float _pixels_per_unit = 16.f;

  // get the effective rect of this camera
  SDL_Rect getRect() const {
    return {
      _view_offset_x_px,
      _view_offset_y_px,
      _view_width_px,
      _view_height_px
    };

    // return toRect(_world_pos, _world_size);
  }

  void updateScreenSize(int x_px, int y_px) {
    _screen_width_px = x_px;
    _screen_height_px = y_px;
    _view_width_px = static_cast<float>(_screen_width_px) * _view_width_factor;
    _view_height_px = static_cast<float>(_screen_height_px) * _view_height_factor;
    updateEffectiveSize();
  }

  void updateEffectiveSize() {
    // update effective world size rect
    _world_size.w = _view_width_px / _pixels_per_unit;
    _world_size.h = _view_height_px / _pixels_per_unit;
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

  PixelPosition toPixel(WorldPosition pos) const {
    return {
      static_cast<int>((pos.x - _world_pos.x) * _pixels_per_unit + _view_offset_x_px),
      static_cast<int>((pos.y - _world_pos.y) * _pixels_per_unit + _view_offset_y_px)
    };
  }

  SDL_Rect toRect(WorldPosition pos, WorldSize s) const {
    return {
      static_cast<int>((pos.x - _world_pos.x) * _pixels_per_unit + _view_offset_x_px),
      static_cast<int>((pos.y - _world_pos.y) * _pixels_per_unit + _view_offset_y_px),
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
    float worldX = ((pixel.x - _view_offset_x_px) / _pixels_per_unit) + _world_pos.x;
    float worldY = ((pixel.y - _view_offset_y_px) / _pixels_per_unit) + _world_pos.y;
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
