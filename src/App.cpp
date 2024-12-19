#include "App.h"

#include <chrono>
#include <cmath>
#include <format>

#include "cmdline.h"

/******************************************************************************
 *
 * Method: App()
 *
 *****************************************************************************/
App::App()
  : _initialized(initialize())
{
}

/******************************************************************************
 *
 * Method: initialize()
 *
 *****************************************************************************/
bool App::initialize()
{
  if (!_screen.initialize()) {
    // failed to create window | renderer | font
    return false;
  }

  _camera = Camera();
  _camera.world = { 0, 0 };
  _camera.screen_height_px = _screen.height();
  _camera.screen_width_px = _screen.width();

  _player.pos = { 106.f, 49.f };
  _player.is_animated = true;

  _player.animator.add(
    Direction::EAST,
    _screen.makeTexture("resources/musky-swim-anim.png"),
    6,
    100
  );

  _player.animator.add(
    Direction::WEST,
    Sprite(
      _screen.makeTexture("resources/musky-swim-anim.png"),
      SDL_RendererFlip::SDL_FLIP_HORIZONTAL,
      180),
    6,
    100
  );

  _player.animator.add(
    Direction::NORTH,
    Sprite(
      _screen.makeTexture("resources/musky-swim-vertical.png"),
      SDL_RendererFlip::SDL_FLIP_NONE,
      270),
    6,
    100
  );

  _player.animator.add(
    Direction::NORTH,
    Sprite(
      _screen.makeTexture("resources/musky-swim-vertical.png"),
      SDL_RendererFlip::SDL_FLIP_NONE,
      270),
    6,
    100
  );

  _player.animator.add(
    Direction::SOUTH,
    Sprite(
      _screen.makeTexture("resources/musky-swim-vertical.png"),
      SDL_RendererFlip::SDL_FLIP_VERTICAL,
      90),
    6,
    100
  );

  _map =
    Map(
      Sprite(_screen.makeTexture("resources/water-tile-green.png")),
      32, 32,
      25,
      25);

  return true;
}

/******************************************************************************
 *
 * Method: gameLoop()
 *
 *****************************************************************************/
void App::gameLoop(uint32_t delta_t_ms)
{
  // draw the base white background
  _screen.clear();

  int mouse_pos_x = 0;
  int mouse_pos_y = 0;

  // find where the mouse is in the world space
  SDL_GetMouseState(&mouse_pos_x, &mouse_pos_y);
  WorldPosition mouse_world = _camera.toWorld({ mouse_pos_x, mouse_pos_y });

  _player.update(delta_t_ms, mouse_world);
  _camera.centerOn(_player.pos, _player.size);

  _map.draw(_screen, _camera);
  _player.draw(_screen, _camera);

  if (cmdline::debug_camera) {
    _screen.addText("Camera");
    _screen.addText("      world x, y: {}, {}", round(_camera.world.x), round(_camera.world.y));
    _screen.addText("      pixels_per_unit: {}", _camera.pixels_per_unit);
    _screen.addTextln();
  }

  if (cmdline::debug_player) {
    _screen.addText("Player");

    _screen.addText("     world x, y: {}, {}", 
      round(_player.pos.x), round(_player.pos.y));

    _screen.addText("     velocity_x, velocity_y: {}, {} ", 
      round(_player.vel_x), round(_player.vel_y));

    _screen.addText("     speed: {}", _player.speed_units_per_sec);
    _screen.addText("     heading: {}", round(_player.heading_deg));
    _screen.addTextln();
  }
  _screen.draw();
}

/******************************************************************************
 *
 * Method: processEvents()
 *
 *****************************************************************************/
void App::processEvents(const SDL_Event& ev)
{
  switch (ev.type) {
    case SDL_WINDOWEVENT:
    {
      _screen.onWindowEvent(ev.window);
      _camera.screen_width_px = _screen.width();
      _camera.screen_height_px = _screen.height();
      break;
    }

    case SDL_QUIT:
    {
      _app_state = AppState::StateExit;
      break;
    }
    case SDL_MOUSEWHEEL:
    {
      if (ev.wheel.y > 0) {
        _camera.zoomIn();
      } else if (ev.wheel.y < 0) {
        _camera.zoomOut();
      }
      break;
    }
    case SDL_KEYUP:
    {
      break;
    }
    case SDL_KEYDOWN:
    {
      switch (ev.key.keysym.sym)
      {
        case SDLK_UP:
        {
          _camera.zoomIn();
          break;
        }

        case SDLK_DOWN:
        {
          _camera.zoomOut();
          break;
        }

        case SDLK_LEFT:
        {
          break;
        }

        case SDLK_RIGHT: 
        {
          break;
        }

        case SDLK_w:
        {
          _player.increaseSpeed();
          break;
        }

        case SDLK_s:
        {
          _player.decreaseSpeed();
          break;
        }

        case SDLK_d:
        {
          break;
        }

        case SDLK_a:
        {
          break;
        }

        case SDLK_q:
        {
          break;
        }
      }
      break;
    }
    default:
    {
      break;
    }
  }
}

/******************************************************************************
 *
 * Method: exec()
 *
 *****************************************************************************/
int App::exec()
{
  if (!_initialized) {
    return 1;
  }

  SDL_Event ev;
  const uint32_t fps_max = cmdline::max_frame_rate;
  const uint32_t msec_per_frame = (1 / fps_max);

  uint32_t current_time_msec = 0;
  uint32_t prev_time_msec = SDL_GetTicks();

  uint32_t delta_t_msec = 0;

  WorldPosition mouse_world = { 0, 0 };

  while (_app_state != AppState::StateExit) {
    current_time_msec = SDL_GetTicks();
    delta_t_msec = current_time_msec - prev_time_msec;
    prev_time_msec = current_time_msec;

    while (SDL_PollEvent(&ev)) {
      processEvents(ev);
    }

    gameLoop(delta_t_msec);

    if (delta_t_msec < msec_per_frame) {
      SDL_Delay(msec_per_frame - delta_t_msec);
    }
  }

  return 0;
}

/******************************************************************************
 *
 * Method: ~App()
 *
 *****************************************************************************/
App::~App()
{
  SDL_Quit();
}
