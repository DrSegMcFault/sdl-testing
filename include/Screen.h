#pragma once

#include <cstdint>
#include <format>

#include <SDL_ttf.h>

#include "Sprite.h"

class Screen {
private:
  // can't use unique_ptr, and shared_ptr is too slow
  SDL_Window* _window;
  SDL_Renderer* _renderer;
  TTF_Font* _font;

public:
  uint32_t screenWidth = 1280;
  uint32_t screenHeight = 960;

  Screen();
  ~Screen();

  bool initialize();
  SDL_Window* getWindow() { return _window; }
  SDL_Renderer* getRenderer() { return _renderer; }

  // copyout a sprite to render
  inline void copyout(Sprite& sprite, SDL_Rect* dest) {
    SDL_RenderCopyEx(
      _renderer,
      sprite.texture.get(),
      &sprite.source,
      dest,
      sprite.rotation_deg, nullptr, sprite.flip);
  }

  // copyout a texture to render
  inline void copyout(
    Texture& texture,
    SDL_Rect* src, SDL_Rect* dest, 
    double angle, SDL_RendererFlip flip) 
  {
    SDL_RenderCopyEx(
      _renderer,
      texture.get(),
      src,
      dest,
      angle, nullptr, flip);
  }

  inline void clear() {
    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
    SDL_RenderClear(_renderer);
  }

  inline void draw() { SDL_RenderPresent(_renderer); }

  // create a text texture and copy it to the renderer,
  // this function does not show the text,
  // you must call SDL_RenderPresent after
  template<typename... Args>
  void enqueueText(int pixel_x, int pixel_y, auto format, Args&&... args)
  {
    auto str = std::vformat(format, std::make_format_args(args...));

    SDL_Surface* surfaceMessage =
      TTF_RenderText_Solid(_font, str.data(), { 255, 255, 255 });

    SDL_Texture* message = SDL_CreateTextureFromSurface(_renderer, surfaceMessage);

    SDL_Rect msg_rect = { pixel_x, pixel_y, 350, 35 };

    SDL_RenderCopy(_renderer, message, NULL, &msg_rect);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(message);
  }

  inline Texture makeTexture(const char* filepath)
  {
    return Texture(_renderer, filepath);
  }
};
