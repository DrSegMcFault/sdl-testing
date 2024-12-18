#pragma once

#include <SDL2/SDL.h>

#include "Texture.h"
#include "Util.h"

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

class Map {
public:

  Map() = default;

  Map(Sprite tileset_, int tile_width, int tile_height, size_t num_tiles) 
   : _tileset(tileset_)
   , _tile_width_px(tile_width)
   , _tile_height_px(tile_height)
   , _num_tiles(num_tiles)
  {
    _tileset.source.w = _tile_width_px;
    _tileset.source.h = _tile_height_px;

    float posx = 0.f; 
    float posy = 0.f;
    for (auto i : range(_num_tiles)) {
      _tiles[i] =
        Tile(
          posx, posy, 
          (rand() % 4) * 90,
          rand() % 2 == 0 ? SDL_RendererFlip::SDL_FLIP_VERTICAL : SDL_RendererFlip::SDL_FLIP_NONE);

      posy += Tile::size.h;
      if (i % 10 == 0) {
        posy = 0.f;
        posx += Tile::size.w;
      }
    }
  }

  // master tileset image
  Sprite _tileset;

  // where to sample the tileset from (x, y)
  int _tile_width_px = 0;
  int _tile_height_px = 0;
  size_t _num_tiles = 0;

  std::array<Tile, 500> _tiles = {{}};

  void draw(Screen& screen, Camera& c) 
  {
    SDL_Rect dest;

    for (const auto& i : range(_num_tiles)) {
      auto& tile = _tiles[i];
      dest = c.toRect(tile.pos, tile.size);

      _tileset.source.x = tile.source.x;
      _tileset.rotation_deg = tile.angle;
      _tileset.flip = tile.flip;

      screen.copyout(_tileset, &dest);
    }
  }
};
