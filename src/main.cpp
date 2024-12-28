#include "cmdline.hpp"
#include "App.hpp"

int main(int argc, char* argv[]) {
  parseCommandLine(argc, argv);
  App a;
  return a.exec();
}
