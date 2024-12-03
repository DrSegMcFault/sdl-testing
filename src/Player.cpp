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

  float angle_direction = atan2f(mouse.y - center_y, mouse.x - center_x);
  float other = -atan2f(mouse.y - center_y, mouse.x - center_x);

  float angle_dir_deg = (angle_direction * 180) / M_PI;

  updateVisualRotation(angle_dir_deg, center_x, center_y, mouse);

  vel_x = speed_units_per_sec * (delta_t_ms / 1000.f) * cosf(other);
  vel_y = speed_units_per_sec * (delta_t_ms / 1000.f) * sinf(other);

  pos.x -= vel_x * delta_t_ms;
  pos.y += vel_y * delta_t_ms;

  if (animations.size()) {
    auto& active = animations[active_anim];
    texture = active.texture;
    src.x = src.w * static_cast<int>((SDL_GetTicks64() / active.delay_ms) % active.frame_count);
  }
}

/******************************************************************************
 *
 * Method: Player::updateVisualRotation()
 *
 *****************************************************************************/
void Player::updateVisualRotation(
  const double& mouse_angle_deg,
  const int& center_x, const int& center_y,
  WorldPosition mouse)
{
  // this is horseshit code, but i want it to work right now.
  // trying to see where the sprite is actually looking, given where the mouse is at
  // then calculate how much we need to rotate the sprite so that it looks like the sprite
  // is looking at the mouse
  // Determine shifted cardinal direction (shifted by 45 degrees)

  if (mouse_angle_deg > -45 && mouse_angle_deg < 45) {
    // direction = Direction::Right;
    active_anim = 1;
    anim_dir = SDL_RendererFlip::SDL_FLIP_NONE;
    eff_x = pos.x + size.w;
    eff_y = pos.y + size.h / 2;

  } else if (mouse_angle_deg <= -45 && mouse_angle_deg >= -135) {
    // direction = Direction::Up;
    active_anim = 2;
    anim_dir = SDL_RendererFlip::SDL_FLIP_VERTICAL;
    eff_x = pos.x + size.w / 2;
    eff_y = pos.y;
  }
   else if (mouse_angle_deg >= 45 && mouse_angle_deg <= 135) {
    // direction = Direction::Down;
    active_anim = 2;
    anim_dir = SDL_RendererFlip::SDL_FLIP_NONE;
    eff_x = pos.x + size.w / 2;
    eff_y = pos.y + size.h;
  } else {
    // direction = Direction::Left;
    anim_dir = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
    active_anim = 1;
    eff_x = pos.x;
    eff_y = pos.y + size.h / 2;
  }

  visual_rotation = 360 - (-atan2f(
    (eff_x - center_x) * (mouse.y - center_y) - (eff_y - center_y) * (mouse.x - center_x),
    (eff_x - center_x) * (mouse.x - center_x) + (eff_y - center_y) * (mouse.y - center_y)) * 180 / M_PI);
}
