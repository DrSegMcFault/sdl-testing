#include "cmdline.h"
#include <CLI/CLI.hpp>

void parseCommandLine(int argc, char *argv[])
{
  CLI::App cli_global{"sdl test executable"};
  cli_global.add_option("--server-hostname, --serverhost", cmdline::server_address, "hostname of server");
  cli_global.add_option("--server-port, --serverport, --port", cmdline::server_port, "port on the server you wish to connect to")
   ->check(CLI::PositiveNumber);

  cli_global.add_option("--max-fps,", cmdline::max_frame_rate, "maximum allowed frames to display per second")
    ->check(CLI::PositiveNumber);

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
