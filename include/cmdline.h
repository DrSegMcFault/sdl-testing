#pragma once

#include <cstdint>
#include <string>

namespace cmdline {
  inline int server_port = 61550;
  inline std::string server_address = "127.0.0.1";
  inline uint32_t max_frame_rate = 240;
  inline bool draw_unit_grid = false;
  inline bool debug_player = false;
  inline bool debug_camera = false;
  inline bool debug_tiles = false;
}

void parseCommandLine(int argc, char* argv[]);
