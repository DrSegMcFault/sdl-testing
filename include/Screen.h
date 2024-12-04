#pragma once
#include <cstdint>

struct SDL_Window;
struct SDL_Renderer;

class Screen {
private:
  // can't use unique_ptr, and shared_ptr is too slow
  SDL_Window* _window;
  SDL_Renderer* _renderer;

public:
  uint32_t screenWidth = 1280;
  uint32_t screenHeight = 960;

  Screen();
  ~Screen();

  bool initialize();
  SDL_Window* getWindow() { return _window; }
  SDL_Renderer* getRenderer() { return _renderer; }
  inline void render();
};
