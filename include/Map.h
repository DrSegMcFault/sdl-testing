#pragma once

#include <vector>

#include <SDL2/SDL.h>

#include "Sprite.h"
#include "Camera.h"
#include "Screen.h"

struct Tile {
  static constexpr WorldSize size = { 15.f, 15.f };
  WorldPosition pos = { 0, 0 };
  // xy wh - the location within the tileset this tile is located
  SDL_Rect source = { 0, 0, 0, 0 };
  double angle = 0;
  SDL_RendererFlip flip;

  Tile() = default;

  Tile(float x, float y, double angle, SDL_RendererFlip flip) 
    : pos(x,y)
    , angle(angle)
    , flip(flip)
  {
    source.x = rand() % 2;
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
