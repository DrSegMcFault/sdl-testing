#include "Camera.hpp"

/******************************************************************************
 *
 * Method: Camera()
 *
 *****************************************************************************/
Camera::Camera(WorldPosition pos, ViewOffset offsets)
  : _pos(pos)
  , _view_offset(offsets)
{

}

/******************************************************************************
 *
 * Method: updateEffectiveSize()
 *
 *****************************************************************************/
void Camera::updateEffectiveSize()
{
  // update effective world size rect
  _size.w = _view_width_px / _pixels_per_unit;
  _size.h = _view_height_px / _pixels_per_unit;

  // update actual pixel rect
  _output_rect.w = static_cast<int>(_size.w * _pixels_per_unit);
  _output_rect.h = static_cast<int>(_size.h * _pixels_per_unit);
}

/******************************************************************************
 *
 * Method: updateScreenSize()
 *
 *****************************************************************************/
void Camera::updateScreenSize(int screen_width, int screen_height)
{
  _view_width_px = static_cast<float>(screen_width) * _view_offset.width_factor;
  _view_height_px = static_cast<float>(screen_height) * _view_offset.height_factor;
  _view_offset.x_px = (1.f - _view_offset.width_factor) * screen_width;
  _view_offset.y_px = (1.f - _view_offset.height_factor) * screen_height;

  _render_rect = {
    _view_offset.x_px,
    _view_offset.y_px,
    _view_width_px,
    _view_height_px
  };

  updateEffectiveSize();
}

/******************************************************************************
 *
 * Method: toRect()
 *
 *****************************************************************************/
SDL_Rect Camera::toRect(WorldPosition p, WorldSize s) const
{
  SDL_Rect incoming {
    static_cast<int>((p.x - _pos.x) * _pixels_per_unit),
    static_cast<int>((p.y - _pos.y) * _pixels_per_unit),
    static_cast<int>(s.w * _pixels_per_unit),
    static_cast<int>(s.h * _pixels_per_unit)
  };
  return SDL_HasIntersection(&_output_rect, &incoming) ? incoming : empty_rect;
}

/******************************************************************************
 *
 * Method: toPixel()
 *
 *****************************************************************************/
PixelPosition Camera::toPixel(WorldPosition pos) const
{
  return {
    static_cast<int>((pos.x - _pos.x) * _pixels_per_unit),
    static_cast<int>((pos.y - _pos.y) * _pixels_per_unit)
  };
}

/******************************************************************************
 *
 * Method: toPixelSize()
 *
 *****************************************************************************/
PixelSize Camera::toPixelSize(WorldSize s) const
{
  return {
    static_cast<int>(s.w * _pixels_per_unit),
    static_cast<int>(s.h * _pixels_per_unit)
  };
}

/******************************************************************************
 *
 * Method: toWorld()
 *
 *****************************************************************************/
WorldPosition Camera::toWorld(PixelPosition pixel) const
{
  return {
    (static_cast<float>(pixel.x) / _pixels_per_unit) + _pos.x,
    (static_cast<float>(pixel.y) / _pixels_per_unit) + _pos.y
  };
}

/******************************************************************************
 *
 * Method: centerOn()
 *
 *****************************************************************************/
void Camera::centerOn(WorldPosition p, WorldSize s)
{
  _pos.x = (p.x + (s.w / 2.f)) - (_size.w / 2.f);
  _pos.y = (p.y + (s.h / 2.f)) - (_size.h / 2.f);

  if (bounds) {
    // restrict the cameras position such that it's rect is fully contained within the bounds 
    _pos.x = 
      std::clamp(_pos.x, 
        bounds->p.x, 
        std::max(bounds->p.x + bounds->s.w - _size.w, 0.f));

    _pos.y = 
      std::clamp(_pos.y, 
        bounds->p.y, 
        std::max(bounds->p.y + bounds->s.h - _size.h, 0.f));
  }
}

/******************************************************************************
 *
 * Method: zoomIn()
 *
 *****************************************************************************/
void Camera::zoomIn(float amount)
{
  _pixels_per_unit = std::min(_pixels_per_unit + amount, ZOOM_MAX);
  updateEffectiveSize();
}

/******************************************************************************
 *
 * Method: zoomOut()
 *
 *****************************************************************************/
void Camera::zoomOut(float amount)
{
  _pixels_per_unit = std::max(_pixels_per_unit - amount, ZOOM_MIN);
  updateEffectiveSize();
}
