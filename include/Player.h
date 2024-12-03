#pragma once
#include <vector>

#include "Texture.h"
#include "Util.h"

struct Player {
  // top left x,y of player, not center
  WorldPosition pos = { 0, 0 };

  // width and height of player, in world units
  WorldSize size = { 5.f, 5.f };

  // sprite sheet sourcing, need to move to a class
  SDL_Rect src = { 0, 0, 32, 32 };

  // animation related
  SDL_RendererFlip anim_dir = SDL_RendererFlip::SDL_FLIP_NONE;
  double visual_rotation = 0;

  int eff_x;
  int eff_y;
  // end animation related

  static constexpr float MAX_SPEED = 20.f;
  float speed_units_per_sec = 0.f;
  float vel_x = 0;
  float vel_y = 0;

  Texture texture;

  struct anim {
    uint32_t delay_ms = 100;
    uint32_t frame_count = 0;
    uint32_t sheet_step_size_x_px = 32;
    uint32_t sheet_step_size_y_px = 32;
    Texture texture;
  };

  std::vector<anim> animations;
  uint32_t active_anim = 0;

  void increaseSpeed() {
    speed_units_per_sec = std::clamp(speed_units_per_sec + 0.5f, 0.f, MAX_SPEED);
  }

  void decreaseSpeed() {
    speed_units_per_sec = std::clamp(speed_units_per_sec - 0.5f, 0.f, MAX_SPEED);
  }

  /*
    rotates the sprite so that it looks like it is looking at the mouse
  */
  void updateVisualRotation(
    const double& mouse_angle_deg,
    const int& center_x, const int& center_y,
    WorldPosition mouse);

  void update(uint32_t delta_t_ms, WorldPosition mouse);
};
