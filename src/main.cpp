#include "sys/sys_sdl2.h"

int main(int argc, char* argv[]) {
    rose_system_sdl2 *system = new rose_system_sdl2();
    if (rose_sys_sdl2_init(system, argc, argv)) {
        rose_sys_sdl2_run(system);
    }

    rose_sys_sdl2_free(system);
    return 0;
}
