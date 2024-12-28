#pragma once

#include <optional>
#include <algorithm>
#include <SDL2/SDL.h>

#include "util/types.hpp"

struct ViewOffset {
  // pixels to shift from 0,0
  int x_px = 0;
  int y_px = 0;

  // width and height factors [0, 1], 
  // 1 taking up the entire screen in that dimension
  float width_factor = 1.f;
  float height_factor = 1.f;
};

// the Camera is always at pixel 0,0
// any relative positioning of this camera within the
// window is done via setting the view port
// prior to any draw calls using a Camera
class Camera {
public:

  Camera() = default;
  Camera(
   WorldPosition pos, 
   ViewOffset offsets = {});

  static constexpr SDL_Rect empty_rect = { 0, 0, 0, 0 };
  static constexpr float ZOOM_MAX = 40.f; // maximum ppu (pixels per world unit)
  static constexpr float ZOOM_MIN = 8.f; // minimum ppu (pixels per world unit)

private:
  // view size in pixels - how big does the camera appear on the screen?
  int _view_width_px = 0;
  int _view_height_px = 0;

  ViewOffset _view_offset;

  // where on the screen will the _output_rect be drawn
  // this is the positioned output rect
  // *items will NOT be drawn using this
  // this is used to position the camera within the screen.
  // and _output_rect is used to render items
  // you could call this the clipping rect
  SDL_Rect _render_rect = { 0, 0, 0, 0 };

  // position in the world
  WorldPosition _pos;
  // 'size' in the world - how many world units can the camera see
  WorldSize _size;

  // SDL_Rect representation of the above;
  // *note that x and y will always be zero
  // and the width and height will be the same as
  // the _render_rect width and height
  // _output_rect can be thought of as a positionless box in which things will
  // be drawn relative to
  SDL_Rect _output_rect { 0, 0, 0, 0 };

  // pixels per world unit (ppu), this is the zoom level
  float _pixels_per_unit = 16.f;

  void updateEffectiveSize();

public:
  // restrict the camera to an object
  struct Bounds {
    WorldPosition p;
    WorldSize s;
  };
  std::optional<Bounds> bounds;

  // where the camera will be drawn on the screen
  const SDL_Rect* getRenderRect() const {
    return &_render_rect;
  }

  const WorldPosition& getPosition() const {
    return _pos;
  }

  float getZoomLevel() const {
    return _pixels_per_unit;
  }

  void setZoom(float value) {
    _pixels_per_unit = 
      std::clamp(value, ZOOM_MIN, ZOOM_MAX);
  }

  // notify the camera that the screen dimensions have changed
  void updateScreenSize(int screen_width, int screen_height);

  // Convert a sized world object to a renderable
  // surface
  SDL_Rect toRect(WorldPosition pos, WorldSize s) const;

  // Convert a World Position to an on-screen position
  PixelPosition toPixel(WorldPosition pos) const;

  // Convert a World size to a screen size
  PixelSize toPixelSize(WorldSize s) const;

  // Convert a screen position to a world position
  WorldPosition toWorld(PixelPosition pixel) const;

  // Center the camera on the World Object
  void centerOn(WorldPosition p, WorldSize sz);

  // increase the ppu (pixels per world unit)
  void zoomIn(float amount = 1.f);

  // decrease the ppu (pixels per world unit)
  void zoomOut(float amount = 1.f);
};
