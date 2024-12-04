#pragma once

#include <functional>

#include <SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Player.h"
#include "Screen.h"
#include "Texture.h"
#include "Camera.h"
#include "Map.h"

class App
{
private:
  enum class AppState {
    StatePlay,
    StateExit
  };

  Map _map;
  Screen _screen;
  Camera _camera;
  AppState _app_state = AppState::StatePlay;
  Player _player;

  TTF_Font* _font;

  // current 'program' being run, for example, the main menu will
  // need to have a different routine than when you are playing the game
  // delta_t_ms (ms since last frame)
  std::function<void(uint32_t)> mainRoutine;

  bool _initialized = false;

  template<typename... Args>
  void displayText(int pixel_x, int pixel_y, auto format, Args&&... args) {
    auto str = std::vformat(format, std::make_format_args(args...));
    SDL_Surface* surfaceMessage =
      TTF_RenderText_Solid(_font, str.data(), {255,255,255});

    SDL_Texture* Message = SDL_CreateTextureFromSurface(_screen.getRenderer(), surfaceMessage);

    SDL_Rect msg_rect = {pixel_x, pixel_y, 250, 35};
    SDL_RenderCopy(_screen.getRenderer(), Message, NULL, &msg_rect);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
  }

  bool initialize();
  void menuLoop(uint32_t delta_t_ms);
  void gameLoop(uint32_t delta_t_ms);
  void handleInput(const SDL_Event& ev);

public:
  App();
  ~App();
  int exec();

};
