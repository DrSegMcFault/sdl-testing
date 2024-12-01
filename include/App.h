#pragma once

#include <SDL.h>

#include "Player.h"
#include "Screen.h"
#include "Texture.h"
#include "Camera.h"

class App
{
private:
  enum class AppState {
    StatePlay,
    StateExit
  };

  Screen _screen;
  Camera _camera;
  AppState _app_state = AppState::StatePlay;
  Player _player;
  bool _initialized = false;

  bool initialize();
  void handleInput(const SDL_Event& ev);

public:
  App();
  ~App();
  int exec();

};
