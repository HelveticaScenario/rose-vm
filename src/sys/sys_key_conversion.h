#ifndef ROSEBUD_SYS_KEY_CONVERSION_H
#define ROSEBUD_SYS_KEY_CONVERSION_H


#include <SDL_scancode.h>
#include "../config.h"

SDL_Scancode rose_keycode_to_sdl_scancode(rose_keycode key);

rose_keycode sdl_scancode_to_rose_keycode(SDL_Scancode scan);

#endif //ROSEBUD_SYS_KEY_CONVERSION_H
