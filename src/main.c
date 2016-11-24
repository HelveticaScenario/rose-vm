#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include "sys/sys_sdl2.h"

int main(int argc, char *argv[]) {
  Rose_SystemSdl2 *system = malloc(sizeof(Rose_SystemSdl2));
  if (rose_sys_sdl2_init(system, argc, argv)) {
    rose_sys_sdl2_run(system);
  }

  // delete system;
  rose_sys_sdl2_free(system);
  return 0;
}
