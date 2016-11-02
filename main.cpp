#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <random>
#include "system_interface.hpp"
#include "system_sdl2.hpp"
#include "sol.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
  SystemInterface *system = new SystemSdl2();
  if (system->init(argc, argv)) {
    system->run();
  }

  delete system;
  return 0;
}
