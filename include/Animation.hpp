#pragma once

#include <cstdint>
#include <array>
#include <SDL2/SDL.h>

#include "Sprite.hpp"
#include "util/types.hpp"

struct Animation {
  Animation() = default;
  Animation(Sprite sprite_sheet,
            uint32_t num_frames,
            uint32_t delay)
    : sprite(sprite_sheet)
    , frame_count(num_frames)
    , delay_ms(delay)
    , frame_width(sprite.texture.width / frame_count)
    , frame_height(frame_width)
    , sheet_step_size_x_px(frame_width)
  {
    sprite.source.w = frame_width;
    sprite.source.h = frame_height;
  }

  // the sprite sheet
  Sprite sprite;

  // how many frames are in the sheet/
  uint32_t frame_count = 0;

  // delay in ms between frames
  uint32_t delay_ms = 100;

  // width in pixels of each frame (usually 32px)
  uint32_t frame_width = 0;

  // height in pixels of each frame (usually 32px)
  uint32_t frame_height = 0;

  // how far away in horizontal pixels
  // is the next frame?
  // this is typically the same value as 'frame_width'
  uint32_t sheet_step_size_x_px = 0;

  // how far away in vertical pixels is the next
  // frame?
  // this is almost always going to be 0, i cannot
  // think of a reason to support staggered sheets
  uint32_t sheet_step_size_y_px = 0;

  void step() {
    // TODO: update this to be more extensible by taking into account 
    // the step size and frame width and height
    sprite.source.x = frame_width * static_cast<int>((SDL_GetTicks64() / delay_ms) % frame_count);
  }
};

struct AnimationController {
  std::array<Animation, 6> animations = {};

  void add(Direction d, Sprite s, uint32_t frame_count, uint32_t delayms) {
    animations[d] = Animation(s, frame_count, delayms);
  }

  void update(Sprite& s, Direction player_direction, const float heading) {
    auto& active = animations[player_direction];
    active.step();
    s = active.sprite;
    s.rotation_deg = heading + active.sprite.rotational_offset;
  }
};
