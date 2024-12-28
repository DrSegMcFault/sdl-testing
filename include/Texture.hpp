#pragma once

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_render.h>
#include <iostream>

class Texture {
private:
  std::shared_ptr<SDL_Texture> _texture;

public:
  Texture() = default;

  Texture(SDL_Renderer* renderer, const char* file) {
    if (auto surface = IMG_Load(file)) {
      width = surface->w;
      height = surface->h;

      _texture = 
        std::shared_ptr<SDL_Texture>(
          SDL_CreateTextureFromSurface(renderer, surface),
          SDL_DestroyTexture);
      SDL_FreeSurface(surface);
    } else {
      std::cout<< SDL_GetError();
    }
  }

  int width = 0;
  int height = 0;

  SDL_Texture* get() { return _texture.get(); }

  explicit operator bool() { return _texture != nullptr; }
};
