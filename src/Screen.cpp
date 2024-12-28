#include "Screen.hpp"
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
      _screen_width_px,
      _screen_height_px,
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

  _font = TTF_OpenFont("resources/Roboto-Medium.ttf", 20);
  if (!_font) {
    return false;
  }

  return true;
}

/******************************************************************************
 *
 * Method: setDrawColor()
 *
 *****************************************************************************/
void Screen::setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
  SDL_SetRenderDrawColor(_renderer, r, g, b, a);
}

/******************************************************************************
 *
 * Method: draw()
 *
 *****************************************************************************/
void Screen::draw() 
{
  SDL_RenderPresent(_renderer);
  // reset the text position cursors
  // so that text can change between frames
  _text_pos_x = 0;
  _text_pos_y = 0;
}

/******************************************************************************
 *
 * Method: clear()
 *
 *****************************************************************************/
void Screen::clear() 
{
  SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
  SDL_RenderClear(_renderer);
}

/******************************************************************************
 *
 * Method: onWindowEvent()
 *
 *****************************************************************************/
void Screen::onWindowEvent(const SDL_WindowEvent& ev) 
{
  switch (ev.event) {
    case SDL_WINDOWEVENT_RESIZED: {
      _screen_width_px = ev.data1;
      _screen_height_px = ev.data2;
      break;
    }
  }
}

/******************************************************************************
 *
 * Method: makeTexture(const char*)
 *
 *****************************************************************************/
Texture Screen::makeTexture(const char* filepath) 
{
  return Texture(_renderer, filepath);
}

/******************************************************************************
 *
 * Method: copyout(Sprite&, SDL_Rect*)
 *
 *****************************************************************************/
void Screen::copyout(Sprite& sprite, SDL_Rect* dest) 
{
  SDL_RenderCopyEx(
    _renderer,
    sprite.texture.get(),
    &sprite.source,
    dest,
    sprite.rotation_deg, nullptr, sprite.flip);
}

/******************************************************************************
 *
 * Method: copyout(SDL_Texture*, SDL_Rect*, SDL_Rect*)
 *
 *****************************************************************************/
void Screen::copyout(SDL_Texture* texture, SDL_Rect* src, SDL_Rect* dest) 
{
  SDL_RenderCopy(
    _renderer,
    texture,
    src,
    dest);
}

/******************************************************************************
 *
 * Method: copyout(Texture&, SDL_Rect*, SDL_Rect*, double, SDL_RendererFlip)
 *
 *****************************************************************************/
void Screen::copyout(
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
