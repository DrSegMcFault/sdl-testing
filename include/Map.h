#pragma once

#include <SDL2/SDL.h>

#include "Texture.h"
#include "Util.h"

struct Tile {
  static constexpr WorldSize size = { 15.f, 15.f };
  WorldPosition pos = { 0, 0 };
  // xy wh - the location within the tileset this tile is located
  SDL_Rect source = {0, 0, 0, 0 };
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

  // maybe add animiation stuff here
};

class Map {
  // does not own this ptr, _Screen does
  public:
  Map() = default;
  Map(SDL_Renderer* renderer) 
    : _renderer(renderer) 
  {
  }

  SDL_Renderer* _renderer;
  uint32_t tileCountX = 50;
  uint32_t tileCountY = 50;

  WorldSize size = { 50, 50 };

  // master tileset image
  Texture tileset;

  // where to sample the tileset from (x, y)
  int _tile_width_px = 32;
  int _tile_height_px = 32;

  std::array<Tile, 500> _tiles = {{}};

  bool load(const char* tilesetsource) 
  { 
    if (!_renderer) { return false; }

    tileset = Texture(_renderer, tilesetsource);
    if (!tileset) {
      return false;
    }

    // hardcoding this for now
    float posx = 0.f; float posy = 0.f;

    for (auto i : range(500)) {
      _tiles[i] = Tile(posx, posy, (rand() % 4) * 90,rand() % 2 == 0 ? SDL_RendererFlip::SDL_FLIP_VERTICAL : SDL_RendererFlip::SDL_FLIP_NONE );
      posy += Tile::size.h;
      if (i % 10 == 0) {
        posy = 0.f;
        posx += Tile::size.w;
      }
    }


    return true;
  }

  void update(Camera& c) 
  {
    // reusable var to hole destination pixelsj
    SDL_Rect dest;
    SDL_Rect src;
    for (const auto& tile : _tiles) {
      dest = c.toRect(tile.pos, tile.size);
    //   if (dest.x < 0 || dest.y < 0) {
    //     continue;
    //   }
      src.x = tile.source.x;
      src.y = 0;
      src.w = _tile_width_px;
      src.h = _tile_height_px;

      // move all this to tile, rand() here can be dont at initialization of the map
      SDL_RenderCopyEx(_renderer, tileset.get(), &src, &dest, 
        tile.angle, // random rotation in increments of 90 deg
        nullptr,
        tile.flip);
    }
  }
};
