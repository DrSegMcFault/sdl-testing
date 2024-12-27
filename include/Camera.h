#pragma once

#include <optional>
#include <algorithm>

#include "Util.h"

// the Camera is always at pixel 0,0
// any relative positioning of this camera within the
// window is done via setting the view port
// prior to any draw calls using a Camera
struct Camera {
  static constexpr SDL_Rect empty_rect = { 0, 0, 0, 0 };
  static constexpr float ZOOM_MAX = 40.f;
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

  // where on the screen will the _output_rect be drawn
  // this is the positioned output rect
  // *items will NOT be drawn using this
  // this is used to position the camera within the screen.
  // and _output_rect is used to render items
  // you could call this the clipping rect
  SDL_Rect _render_rect = { 0, 0, 0, 0 };

  // position in the world
  WorldPosition _world_pos;
  // 'size' in the world - how many world units can the camera see
  WorldSize _world_size;

  // SDL_Rect representation of the above;
  // *note that x and y will always be zero
  // and the width and height will be the same as
  // the _render_rect width and height
  // _output_rect can be thought of as a positionless box in which things will
  // be drawn relative to
  SDL_Rect _output_rect { 0, 0, 0, 0 };

  // pixels per world unit
  float _pixels_per_unit = 16.f;

  // where the camera will be drawn on the screen
  const SDL_Rect* getRenderRect() const {
    return &_render_rect;
  }

  void updateScreenSize(int x_px, int y_px) {
    _screen_width_px = x_px;
    _screen_height_px = y_px;

    _view_width_px = static_cast<float>(_screen_width_px) * _view_width_factor;
    _view_height_px = static_cast<float>(_screen_height_px) * _view_height_factor;
    _view_offset_x_px = (1.f - _view_width_factor) * _screen_width_px;
    _view_offset_y_px = (1.f - _view_height_factor) * _screen_height_px;

    _render_rect = {
      _view_offset_x_px,
      _view_offset_y_px,
      _view_width_px,
      _view_height_px
    };

    updateEffectiveSize();
  }

  void updateEffectiveSize() {
    // update effective world size rect
    _world_size.w = _view_width_px / _pixels_per_unit;
    _world_size.h = _view_height_px / _pixels_per_unit;

    // update actual pixel rect
    _output_rect.w = static_cast<int>(_world_size.w * _pixels_per_unit);
    _output_rect.h = static_cast<int>(_world_size.h * _pixels_per_unit);
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
      static_cast<int>((pos.x - _world_pos.x) * _pixels_per_unit),
      static_cast<int>((pos.y - _world_pos.y) * _pixels_per_unit)
    };
  }

  SDL_Rect toRect(WorldPosition pos, WorldSize s) const {
    SDL_Rect incoming {
      static_cast<int>((pos.x - _world_pos.x) * _pixels_per_unit),
      static_cast<int>((pos.y - _world_pos.y) * _pixels_per_unit),
      static_cast<int>(s.w * _pixels_per_unit),
      static_cast<int>(s.h * _pixels_per_unit)
    };
    return SDL_HasIntersection(&_output_rect, &incoming) ? incoming : empty_rect;
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
    _world_pos.x = (p.x + (sz.w / 2.f)) - (_world_size.w / 2.f);
    _world_pos.y = (p.y + (sz.h / 2.f)) - (_world_size.h / 2.f);

    if (bounds) {
      // restrict the cameras position such that it's rect is fully contained within the bounds 
      _world_pos.x = std::clamp(_world_pos.x, bounds->p.x, std::max(bounds->p.x + bounds->s.w - _world_size.w, 0.f));
      _world_pos.y = std::clamp(_world_pos.y, bounds->p.y, std::max(bounds->p.y + bounds->s.h - _world_size.h, 0.f));
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
