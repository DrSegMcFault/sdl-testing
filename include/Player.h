#pragma once

#include <SDL2/SDL.h>

#include "Util.h"
#include "Animation.h"
#include "Camera.h"
#include "Sprite.h"
#include "Screen.h"

// what is the player doing
enum Action {
  NONE,
  IDLE,
  MOVING,
  ATTACKING,
  // SLEEPING
  // ... anything else
};

struct Player {
  // top left x,y of player, not center
  WorldPosition _pos = { 0, 0 };

  // width and height of player, in world units
  WorldSize _size = { 5.f, 5.f };

  static constexpr float SPEED_MAX = 30.f;
  static constexpr float SPEED_MIN = 0.f;
  float _speed_units_per_sec = 0.f;

  // velocity
  float _vel_x = 0;
  float _vel_y = 0;

  // north is 270
  // east is 0
  // south is 90
  // west is 180
  float _heading_deg = 0;

  bool _is_animated = false;
  Sprite _sprite;

  AnimationController _animator;
  Direction _direction = Direction::EAST;

  void increaseSpeed() {
    _speed_units_per_sec = std::min(_speed_units_per_sec + 1.5f, SPEED_MAX);
  }

  void decreaseSpeed() {
    _speed_units_per_sec = std::max(_speed_units_per_sec - 1.5f, SPEED_MIN);
  }

  void update(uint32_t delta_t_ms, WorldPosition mouse);
  void draw(Screen& s, Camera& cam);
};
