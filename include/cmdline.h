#pragma once

#include <cstdint>
#include <string>

namespace cmdline {
  inline int server_port = 61550;
  inline std::string server_address = "127.0.0.1";
  inline uint32_t max_frame_rate = 240;
}

void parseCommandLine(int argc, char* argv[]);
