#include "cmdline.h"
#include <CLI/CLI.hpp>

void parseCommandLine(int argc, char *argv[])
{
  CLI::App cli_global{"SDL Test Executable"};

  cli_global.add_option("--server-hostname, --serverhost", cmdline::server_address, "hostname of server");
  cli_global.add_option("--server-port, --serverport, --port", cmdline::server_port, "port on the server you wish to connect to")
   ->check(CLI::PositiveNumber);

  cli_global.add_option("--max-fps,", cmdline::max_frame_rate, "maximum allowed frames to display per second")
    ->check(CLI::PositiveNumber);

  cli_global.add_flag("--debug-unit-grid,", cmdline::draw_unit_grid, "Debugging - always draw the unit grid on the screen");
  cli_global.add_flag("--debug-player",     cmdline::debug_player,   "Debugging - output player values to the screen");
  cli_global.add_flag("--debug-camera",     cmdline::debug_camera,   "Debugging - output camera values to the screen");
  cli_global.add_flag("--debug-tiles",      cmdline::debug_tiles,    "Debugging - show tile stats on screen");

  try {
    cli_global.parse(argc, argv);
  }
  catch (const CLI::ParseError& e) {
    std::exit(cli_global.exit(e));
  }
  catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    std::exit(1);
  }
}
