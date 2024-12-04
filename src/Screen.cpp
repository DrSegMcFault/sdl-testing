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
  SDL_Init(SDL_INIT_VIDEO);

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

  return true;
}

/******************************************************************************
 *
 * Method: render()
 *
 *****************************************************************************/
void Screen::render()
{
  SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
  SDL_RenderClear(_renderer);

  SDL_RenderPresent(_renderer);
}

/******************************************************************************
 *
 * Method: ~Screen()
 *
 *****************************************************************************/
Screen::~Screen()
{
  if (_renderer) {
    SDL_DestroyRenderer(_renderer);
  }
  if (_window) {
    SDL_DestroyWindow(_window);
  }
}
