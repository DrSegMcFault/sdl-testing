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

  // current 'program' being run, for example, the main menu will
  // need to have a different routine than when you are playing the game
  // delta_t_ms (ms since last frame)
  std::function<void(uint32_t)> mainRoutine;

  bool _initialized = false;

  bool initialize();
  void menuLoop(uint32_t delta_t_ms);
  void gameLoop(uint32_t delta_t_ms);
  void handleInput(const SDL_Event& ev);

public:
  App();
  ~App();
  int exec();

};
