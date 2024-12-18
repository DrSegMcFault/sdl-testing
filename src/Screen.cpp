#include "Screen.h"
#include <SDL.h>

/******************************************************************************
 *
 * Method: Screen()
 *
 *****************************************************************************/
Screen::Screen()
{
}

/******************************************************************************
 *
 * Method: initialize()
 *
 *****************************************************************************/
bool Screen::initialize() 
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    return false;
  }

  if (TTF_Init() < 0) {
    return false;
  }

  _window =
      SDL_CreateWindow(
        "SDL Test App",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screenWidth,
        screenHeight,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

  if (!_window) {
    // failed to create window
    return false;
  }

  _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);

  if (!_renderer) {
    // failed to create renderer
    return false;
  }

  _font = TTF_OpenFont("resources/Roboto-Medium.ttf", 24);
  if (!_font) {
    return false;
  }

  return true;
}

/******************************************************************************
 *
 * Method: ~Screen()
 *
 *****************************************************************************/
Screen::~Screen()
{
  if (_font) {
    TTF_CloseFont(_font);
  }
  if (_renderer) {
    SDL_DestroyRenderer(_renderer);
  }
  if (_window) {
    SDL_DestroyWindow(_window);
  }
}
