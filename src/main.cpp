#include "cmdline.h"
#include "App.h"

int main(int argc, char* argv[]) {
  parseCommandLine(argc, argv);
  App a;
  return a.exec();
}
