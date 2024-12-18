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

  bool _initialized = false;

  bool initialize();
  void gameLoop(uint32_t delta_t_ms);
  void handleInput(const SDL_Event& ev);

public:
  App();
  ~App();
  int exec();

};
