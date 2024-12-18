#pragma once

#include <SDL2/SDL.h>

#include "Texture.h"

// an image to be rendered on the screen
// source is by default the same size as the texture,
// but it allows for sampling regions of the texture.
struct Sprite {

  Sprite() = default;

  Sprite(Texture txture)
    : texture(txture)
    , source({ 0/*x*/, 0/*y*/, texture.width, texture.height})
    , rotation_deg(0)
    , flip(SDL_RendererFlip::SDL_FLIP_NONE)
    , rotational_offset(0)
  {
  }

  Sprite(Texture txture, SDL_RendererFlip f, double angle_offset)
    : texture(txture)
    , source({ 0/*x*/, 0/*y*/, texture.width, texture.height})
    , rotation_deg(0)
    , flip(f)
    , rotational_offset(angle_offset)
  {
  }

  explicit operator bool() { return texture.operator bool(); }

  // image data
  Texture texture;

  // how to source the above texture
  SDL_Rect source;

  // actual rotaion applied to the image
  double rotation_deg;

  // flip/mirror of the image
  SDL_RendererFlip flip;

  // annoying, where is the sprite looking?
  /*
               here for north south animations -90 deg
               |
               v
       x x x x x x x x x
       x x x x x x x x x
       x x x x x x x x x
       x x x x x x x x x <- this point for east/west animation 0 deg
       x x x x x x x x x
       x x x x x x x x x
       x x x x x x x x x
               ^
               |
               90 deg
  */
  double rotational_offset;
};
