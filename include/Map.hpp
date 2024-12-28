#pragma once

#include <vector>

#include <SDL2/SDL.h>

#include "Sprite.hpp"
#include "Camera.hpp"
#include "Screen.hpp"

struct Tile {
  static constexpr WorldSize size = { 15.f, 15.f };
  WorldPosition pos = { 0, 0 };

  // the rectangle within the master tileset that is tile is referring to 
  SDL_Rect source = { 0, 0, 0, 0 };
  double angle = 0;
  SDL_RendererFlip flip;

  Tile() = default;

  Tile(float x, float y, SDL_Rect&& src, double angle, SDL_RendererFlip flip) 
    : pos(x,y)
    , source(std::move(src))
    , angle(angle)
    , flip(flip)
  {
  }
};

struct Map {
  Map() = default;
  Map(Sprite tileset_,
      int tile_width, int tile_height,
      size_t map_columns, size_t map_rows);
  
  // master tileset image
  Sprite _tileset;

  // how large in pixels is each tile
  int _tile_width_px;
  int _tile_height_px;

  size_t _num_rows;
  size_t _num_columns;
  size_t _num_tiles;

  WorldPosition _position;
  WorldSize _size;

  std::vector<Tile> _tiles;

  void draw(Screen& screen, Camera& c);
};
