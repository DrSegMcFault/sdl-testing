#include "Player.h"

#include <numbers>
#include <cmath>

/******************************************************************************
 *
 * Method: Player::update()
 *
 *****************************************************************************/
void Player::update(uint32_t delta_t_ms, WorldPosition mouse)
{
  auto center_x = pos.x + size.w / 2.f;
  auto center_y = pos.y + size.h / 2.f;

  float angle_mouse_center_rad = std::atan2(mouse.y - center_y, mouse.x - center_x);

  auto norm = [](auto heading) {
    if (heading < 0) {
      return heading + 360;
    }
    return heading;
  };

  heading_deg = [](auto angle) {
    if (angle < 0) {
      return angle + 360;
    }
    return angle;
  }(angle_mouse_center_rad * 180.f / std::numbers::pi_v<float>);

  vel_x = speed_units_per_sec * std::cos(angle_mouse_center_rad);
  vel_y = speed_units_per_sec * std::sin(angle_mouse_center_rad);

  pos.x += vel_x * (delta_t_ms / 1000.f);
  pos.y += vel_y * (delta_t_ms / 1000.f);

  if (heading_deg > 45 && heading_deg <= 135) {
    direction = Direction::SOUTH;
  } else if (heading_deg > 135 && heading_deg <= 225) {
    direction = Direction::WEST;
  } else if (heading_deg > 225 && heading_deg <= 315) {
    direction = Direction::NORTH;
  } else {
    direction = Direction::EAST;
  }

  if (is_animated) {
    animator.update(sprite, direction, heading_deg);
  }
}

/******************************************************************************
 *
 * Method: Player::draw()
 *
 *****************************************************************************/
void Player::draw(Screen& screen, Camera& cam)
{
  SDL_Rect junk;
  SDL_Rect camera_rect = cam.getRect();
  SDL_Rect dest_rect = cam.toRect(pos, size);

  if (SDL_IntersectRect(&dest_rect, &camera_rect, &junk)) {
    screen.copyout(sprite, &dest_rect);
  }
}
