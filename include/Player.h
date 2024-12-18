#pragma once

#include <SDL2/SDL.h>

#include "Util.h"
#include "Animation.h"
#include "Camera.h"
#include "Sprite.h"
#include "Screen.h"

struct Player {
  // top left x,y of player, not center
  WorldPosition pos = { 0, 0 };

  // width and height of player, in world units
  WorldSize size = { 5.f, 5.f };

  static constexpr float MAX_SPEED = 20.f;
  float speed_units_per_sec = 0.f;
  float vel_x = 0;
  float vel_y = 0;
  float heading_deg = 0;

  bool is_animated = false;
  Sprite sprite;

  AnimationController animator;

  void increaseSpeed() {
    speed_units_per_sec = std::clamp(speed_units_per_sec + 1.5f, -1.5f, MAX_SPEED);
  }

  void decreaseSpeed() {
    speed_units_per_sec = std::clamp(speed_units_per_sec - 1.5f, -1.5f, MAX_SPEED);
  }

  void update(uint32_t delta_t_ms, WorldPosition mouse);
  void draw(Screen& s, Camera& cam);
};
