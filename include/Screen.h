#pragma once

#include <cstdint>
#include <format>

#include <SDL_ttf.h>

#include "Sprite.h"

class Screen {
private:
  SDL_Window* _window;
  SDL_Renderer* _renderer;
  TTF_Font* _font;

  // make a text controller class?
  int _text_pos_y = 0;
  int _text_pos_x = 0;
  int _text_vertical_spacing_px = 5;
  int _text_indentation_px = 5;
  int _last_text_height = 0;

  int _screen_width_px = 1280;
  int _screen_height_px = 960;

public:
  Screen();
  ~Screen();

  bool initialize();

  int width() { return _screen_width_px; } 
  int height() { return _screen_height_px; }

  void setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
  void draw(); 
  void clear();

  void onWindowEvent(const SDL_WindowEvent& event);

  Texture makeTexture(const char* filepath);

  // The SDL documentation does not say this,
  // but after you call this, 0,0 is now the xy of the rect you passed here
  void setViewPort(int x, int y, int w, int h) {
    SDL_Rect r = {
      x, y, w, h
    };
    SDL_RenderSetViewport(_renderer, &r);
  }

  // The SDL documentation does not say this,
  // but after you call this, 0,0 is now the xy of the rect you passed here
  void setViewPort(SDL_Rect* c) {
    SDL_RenderSetViewport(_renderer, c);
  }

  void filledRect(SDL_Rect* r) {
    SDL_RenderFillRect(_renderer, r);
  }

  void filledRectBackground(SDL_Rect* foreground, SDL_Color bg, SDL_Color fg) {
    if (!foreground) {
      return;
    }
    setDrawColor(bg.r, bg.g, bg.b, bg.a);
    SDL_Rect background = {
      foreground->x - 1,
      foreground->y - 1,
      foreground->w + 2,
      foreground->h + 2
    };
    SDL_RenderFillRect(_renderer, &background);

    setDrawColor(fg.r, fg.g, fg.b, fg.a);
    SDL_RenderFillRect(_renderer, foreground);
  }

  void copyout(Sprite& sprite, SDL_Rect* dest);

  void copyout(
    SDL_Texture* texture,
    SDL_Rect* src, SDL_Rect* dest);
  
  void copyout(
    Texture& texture,
    SDL_Rect* src, SDL_Rect* dest, 
    double angle, SDL_RendererFlip flip);

  // make an empy new line of text on the screen
  void addTextln() 
  {
    _text_pos_y += _last_text_height;
  }

  // create a text texture and copy it to the renderer,
  // this function does not show the text,
  // you must call draw() after
  template<typename... Args>
  void addText(auto format, Args&&... args)
  {
    int dest_w = 0;
    int dest_h = 0;

    std::string str = std::vformat(format, std::make_format_args(args...));
    if (TTF_SizeText(_font, str.c_str(), &dest_w, &dest_h) != 0) {
      return;
    }

    SDL_Surface* surface = TTF_RenderText_Blended(_font, str.c_str(), { 255, 255, 255 });
    SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect dest_rect = {
      _text_pos_x + _text_indentation_px,
      _text_pos_y,
      dest_w,
      dest_h,
    };

    copyout(texture, nullptr, &dest_rect);
    SDL_DestroyTexture(texture);

    // prepare the next position where text will be drawn
    // this may need to involve x, but probably in a specialization
    // of this function as having only y move is a good default for now
    _text_pos_y = std::min(_text_pos_y + dest_h, _screen_height_px - dest_h);
    _last_text_height = dest_h;
  }
};
