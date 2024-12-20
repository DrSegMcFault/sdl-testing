#include "Map.h"
#include <random>
#include <cstdlib>
#include <chrono>

#include "Util.h"
#include "cmdline.h"

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

  auto now = std::chrono::steady_clock::now().time_since_epoch().count();
  srand(static_cast<unsigned int>(now));

  for (auto i : range(_num_rows)) {
    for (auto j : range(_num_columns)) {
      _tiles[i * _num_columns + j] =
        Tile(
          i * Tile::size.w, j * Tile::size.h,
          (rand() % 4) * 90,
          static_cast<SDL_RendererFlip>(rand() % 3));
    }
  }

  _position = WorldPosition(0.f, 0.f);
  _size = {
    .w = Tile::size.w * _num_rows,
    .h = Tile::size.h * _num_columns,
  };
}

/******************************************************************************
 *
 * Method: Map::draw()
 *
 *****************************************************************************/
void Map::draw(Screen& screen, Camera& c)
{
  SDL_Rect camera_rect = c.getRect();
  SDL_Rect dest;
  SDL_Rect temp;
  int visible_tiles = 0;

  for (const auto i : range(_num_tiles)) {
    auto& tile = _tiles[i];
    dest = c.toRect(tile.pos, tile.size);

    // if this tile is not within the view of the 
    // camera, dont render it
    if (!SDL_IntersectRect(&dest, &camera_rect, &temp)) {
      continue;
    }
    visible_tiles++;

    _tileset.source.x = tile.source.x;
    _tileset.rotation_deg = tile.angle;
    _tileset.flip = tile.flip;

    screen.copyout(_tileset, &dest);
  }

  if (cmdline::debug_tiles) {
    screen.addText("Tiles", visible_tiles, _num_tiles);
    screen.addText("      Visible: {}", visible_tiles);
    screen.addText("      Total: {}", _num_tiles);
    screen.addTextln();
  }
}
