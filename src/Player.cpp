#include "Player.h"

#include <numbers>
#include <cmath>

#include "cmdline.h"

/******************************************************************************
 *
 * Method: Player::update()
 *
 *****************************************************************************/
void Player::update(uint32_t delta_t_ms, WorldPosition mouse)
{
  auto center_x = _pos.x + _size.w / 2.f;
  auto center_y = _pos.y + _size.h / 2.f;

  float angle_mouse_center_rad =
    std::atan2(mouse.y - center_y, mouse.x - center_x);

  _heading_deg = [](auto angle) {
    if (angle < 0) {
      return angle + 360;
    }
    return angle;
  }(angle_mouse_center_rad * 180.f / std::numbers::pi_v<float>);

  _vel_x = _speed_units_per_sec * std::cos(angle_mouse_center_rad);
  _vel_y = _speed_units_per_sec * std::sin(angle_mouse_center_rad);

  _pos.x += _vel_x * (delta_t_ms / 1000.f);
  _pos.y += _vel_y * (delta_t_ms / 1000.f);

  if (_heading_deg > 45 && _heading_deg <= 135) {
    _direction = Direction::SOUTH;
  } else if (_heading_deg > 135 && _heading_deg <= 225) {
    _direction = Direction::WEST;
  } else if (_heading_deg > 225 && _heading_deg <= 315) {
    _direction = Direction::NORTH;
  } else {
    _direction = Direction::EAST;
  }

  if (_is_animated) {
    _animator.update(_sprite, _direction, _heading_deg);
  }
}

/******************************************************************************
 *
 * Method: Player::draw()
 *
 *****************************************************************************/
void Player::draw(Screen& screen, Camera& cam)
{
  SDL_Rect dest_rect = cam.toRect(_pos, _size);

  if (!SDL_RectEmpty(&dest_rect)) {
    screen.copyout(_sprite, &dest_rect);
  }

  if (cmdline::debug_player) {
    screen.addText("Player");

    screen.addText("     world x, y: {}, {}", 
      round(_pos.x), round(_pos.y));

    screen.addText("     velocity_x, velocity_y: {}, {} ", 
      round(_vel_x), round(_vel_y));

    screen.addText("     speed: {}", _speed_units_per_sec);
    screen.addText("     heading: {}", round(_heading_deg));
    screen.addTextln();
  }
}
