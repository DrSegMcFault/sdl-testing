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
  auto center_x = pos.x + size.w / 2;
  auto center_y = pos.y + size.h / 2;

  float angle_mouse_center_rad = std::atan2(mouse.y - center_y, mouse.x - center_x);

  auto norm = [](auto heading) {
    if (heading < 0) {
      return heading + 360;
    }
    return heading;
  };

  heading_deg = norm(angle_mouse_center_rad * 180 / M_PI);

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
  PixelSize size_pixels = cam.toPixelSize(size);

  SDL_Rect destination_rect = {
    cam.screen_width_px / 2 - size_pixels.w / 2,
    cam.screen_height_px / 2 - size_pixels.h / 2,
    size_pixels.w,
    size_pixels.h
  };

  screen.copyout(sprite, &destination_rect);
}
