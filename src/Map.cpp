#include "Map.h"
#include <random>
#include <cstdlib>
#include "Util.h"

/******************************************************************************
 *
 * Method: Map()
 *
 *****************************************************************************/
Map::Map(
    Sprite tileset_, int tile_width_pixels, int tile_height_pixels, 
    size_t map_columns, size_t map_rows)
  : _tileset(tileset_)
  , _tile_width_px(tile_width_pixels)
  , _tile_height_px(tile_height_pixels)
  , _num_rows(map_rows)
  , _num_columns(map_columns)
  , _num_tiles(_num_rows * _num_columns)
  , _tiles(_num_tiles)
{
  _tileset.source.w = _tile_width_px;
  _tileset.source.h = _tile_height_px;

  float posx = 0.f;
  float posy = 0.f;

  srand(SDL_GetTicks());

  for (auto i : range(_num_rows)) {
    for (auto j : range(_num_columns)) {
      _tiles[i * _num_columns + j] =
        Tile(
          i * Tile::size.w, j * Tile::size.h,
          (rand() % 4) * 90,
          static_cast<SDL_RendererFlip>(rand() % 3));
    }
  }
}

/******************************************************************************
 *
 * Method: Map::draw()
 *
 *****************************************************************************/
void Map::draw(Screen& screen, Camera& c)
{
  SDL_Rect dest;

  for (const auto i : range(_num_tiles)) {
    auto& tile = _tiles[i];
    dest = c.toRect(tile.pos, tile.size);

    _tileset.source.x = tile.source.x;
    _tileset.rotation_deg = tile.angle;
    _tileset.flip = tile.flip;

    screen.copyout(_tileset, &dest);
  }
}
