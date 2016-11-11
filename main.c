#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include "system_interface.h"
#include "system_sdl2.h"

int main(int argc, char *argv[]) {
  Rose_SystemSdl2 *system = malloc(sizeof(Rose_SystemSdl2));
  if (rose_init(system, argc, argv)) {
    rose_run(system);
  }

  // delete system;
  rose_free(system);
  return 0;
}
