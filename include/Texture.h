#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <memory>
#include <iostream>

class Texture {
private:
  std::shared_ptr<SDL_Texture> _txture;

public:
  Texture() = default;
  Texture(SDL_Renderer* renderer, const char* file) {
    if (auto surface = IMG_Load(file)) {
      _txture = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer, surface), SDL_DestroyTexture);
    } else {
      std::cout<< SDL_GetError();
    }
  }
  auto get() { return _txture.get();}
  explicit operator bool() { return _txture.operator bool(); }
};
