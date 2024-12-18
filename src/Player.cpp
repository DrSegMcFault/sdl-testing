#include "Player.h"

/******************************************************************************
 *
 * Method: Player::update()
 *
 *****************************************************************************/
void Player::update(uint32_t delta_t_ms, WorldPosition mouse)
{
  auto center_x = pos.x + size.w / 2;
  auto center_y = pos.y + size.h / 2;

  float angle_mouse_center_rad = atan2f(mouse.y - center_y, mouse.x - center_x);
  float angle_mouse_center_deg = (angle_mouse_center_rad * 180) / M_PI;

  heading_deg = angle_mouse_center_deg;

  if (angle_mouse_center_deg > -45 && angle_mouse_center_deg < 45) {
    // direction = Direction::Right;
    animator.active_animation = AnimationType::MOVING_EAST;
  } 
  else if (angle_mouse_center_deg <= -45 && angle_mouse_center_deg >= -135) {
    // direction = Direction::Up;
    animator.active_animation = AnimationType::MOVING_NORTH;
  }
   else if (angle_mouse_center_deg >= 45 && angle_mouse_center_deg <= 135) {
    // direction = Direction::Down;
    animator.active_animation = AnimationType::MOVING_SOUTH;
  } else {
    // direction = Direction::Left;
    animator.active_animation = AnimationType::MOVING_WEST;
  }

  vel_x = speed_units_per_sec * (delta_t_ms / 1000.f) * cosf(-angle_mouse_center_rad);
  vel_y = speed_units_per_sec * (delta_t_ms / 1000.f) * sinf(-angle_mouse_center_rad);

  pos.x -= vel_x * delta_t_ms;
  pos.y += vel_y * delta_t_ms;

  if (is_animated) {
    animator.update(sprite, angle_mouse_center_deg);
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
