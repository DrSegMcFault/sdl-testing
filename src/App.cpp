#include "App.h"
#include <chrono>
#include <cmath>
#include <format>

#include <SDL2/SDL_ttf.h>
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
  TTF_Init();

  if (!_screen.initialize()) {
    // failed to create window/renderer
    return false;
  }
  _player.pos = {1, 1};
  _camera = Camera();
  _camera.world = {0, 0};
  _camera.screen_height_px = _screen.screenHeight;
  _camera.screen_width_px = _screen.screenWidth;

  {
    auto& this_anim = _player.animations.emplace_back();
    this_anim.texture = Texture(_screen.getRenderer(), "resources/musky-idle-anim.png");
    this_anim.frame_count = 3;
    this_anim.sheet_step_size_x_px = 32;
    this_anim.sheet_step_size_y_px = 0;
    this_anim.delay_ms = 150;
  }

  {
    auto& this_anim = _player.animations.emplace_back();
    this_anim.texture = Texture(_screen.getRenderer(), "resources/musky-swim-anim.png");
    this_anim.frame_count = 6;
    this_anim.sheet_step_size_x_px = 32;
    this_anim.sheet_step_size_y_px = 0;
    this_anim.delay_ms = 100;
  }

  {
    auto& this_anim = _player.animations.emplace_back();
    this_anim.texture = Texture(_screen.getRenderer(), "resources/musky-swim-vertical.png");
    this_anim.frame_count = 6;
    this_anim.sheet_step_size_x_px = 32;
    this_anim.sheet_step_size_y_px = 0;
    this_anim.delay_ms = 100;
  }

  return true;
}

/******************************************************************************
 *
 * Method: handleInput()
 *
 *****************************************************************************/
void App::handleInput(const SDL_Event& ev)
{
  switch (ev.type) {
    case SDL_WINDOWEVENT:
    {
      switch (ev.window.event) {
        case SDL_WINDOWEVENT_RESIZED:
        {
          _camera.screen_width_px = ev.window.data1;
          _camera.screen_height_px = ev.window.data2;
          break;
        }
      }
      break;
    }

    case SDL_QUIT:
    {
      _app_state = AppState::StateExit;
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
        case SDLK_j:
        {
          // _camera.move({ -.5f, 0.f });
          break;
        }
        case SDLK_k:
        {
          // _camera.move({ 0.f, -.5f });
          break;
        }
        case SDLK_i:
        {
          // _camera.move({ 0.f, .5f });
          break;
        }
        case SDLK_l:
        {
          // _camera.move({ .5f, 0.f });
          break;
        }

        case SDLK_LEFT:
        {
          break;
        }
        case SDLK_RIGHT: {
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

  TTF_Font* font = TTF_OpenFont("resources/Roboto-Medium.ttf", 24);

  auto instantText = [&]<typename... Args>(int x, int y, auto format, Args&&... args) {
    auto str = std::vformat(format, std::make_format_args(args...));
    SDL_Surface* surfaceMessage =
      TTF_RenderText_Solid(font, str.data(), {255,255,255});

    SDL_Texture* Message = SDL_CreateTextureFromSurface(_screen.getRenderer(), surfaceMessage);

    SDL_Rect msg_rect = {x, y, 250, 35};
    SDL_RenderCopy(_screen.getRenderer(), Message, NULL, &msg_rect);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
  };

  SDL_Event ev;
  const uint32_t fps_max = cmdline::max_frame_rate;
  const uint32_t msec_per_frame = (1 / fps_max);

  uint32_t current_time_msec = 0;
  uint32_t prev_time_msec = SDL_GetTicks();

  uint32_t delta_t_msec = 0;

  int mouse_pos_x = 0;
  int mouse_pos_y = 0;
  WorldPosition mouse_world = { 0, 0 };

  while (_app_state != AppState::StateExit) {
    current_time_msec = SDL_GetTicks();
    delta_t_msec = current_time_msec - prev_time_msec;
    prev_time_msec = current_time_msec;

    while (SDL_PollEvent(&ev)) {
      handleInput(ev);
    }

    // update the camera
    SDL_GetMouseState(&mouse_pos_x, &mouse_pos_y);
    mouse_world = _camera.toWorld({mouse_pos_x, mouse_pos_y});

    _player.update(delta_t_msec, mouse_world);
    _camera.centerOn(_player.pos, _player.size);

    PixelSize player_sz_px = _camera.toPixelSize(_player.size);

    SDL_Rect d = {
      static_cast<int>(_camera.screen_width_px / 2) - (player_sz_px.w / 2),
      static_cast<int>(_camera.screen_height_px / 2) - (player_sz_px.h / 2),
      player_sz_px.w,
      player_sz_px.h
    };

    SDL_SetRenderDrawColor(_screen.getRenderer(), 83, 132, 172, 255);
    SDL_RenderClear(_screen.getRenderer());

    SDL_RenderCopyEx(
      _screen.getRenderer(), _player.texture.get(),
      &_player.src, &d,
      _player.visual_rotation, nullptr,
      _player.anim_dir);

    instantText(0, 0, "Player world xy: {},{} mousewrld pos: {},{}", 
      round(_player.pos.x), round(_player.pos.y), mouse_world.x, mouse_world.y);

    instantText(0, 40, "Player velx, vely: {},{}", 
      round(_player.vel_x), round(_player.vel_y));
    instantText(0, 80, "Camera pos: {}, {}", _camera.world.x, _camera.world.y);
    instantText(0, 120, "Camera pixels_per_unit: {}", _camera.pixels_per_unit);

    instantText(_camera.screen_width_px - 250, 0, "Player speed: {}", _player.speed_units_per_sec);

    SDL_RenderPresent(_screen.getRenderer());
    if (delta_t_msec < msec_per_frame) {
      SDL_Delay(msec_per_frame - delta_t_msec);
    }
  }

  TTF_CloseFont(font);

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
