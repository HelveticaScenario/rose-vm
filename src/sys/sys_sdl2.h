#pragma once

#include <SDL.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include "rt/rose.h"
#include "sys_key_conversion.h"

struct rose_system_sdl2 {
    // The window we'll be rendering to
    SDL_Window* window;

    // The window renderer
    SDL_Renderer* renderer;

    // The actual hardware texture
    SDL_Texture* texture;
    SDL_Texture* hd_texture;

    void* pixels;
    int32_t pitch;
    int32_t window_width;
    int32_t window_height;
    // Image dimensions
    uint16_t width_mult;
    uint16_t height_mult;

    rose_fs* fs;

    rose_desktop_player* player;
};

bool rose_sys_sdl2_init(rose_system_sdl2* s, int argc, char* argv[]);

void rose_sys_sdl2_run(rose_system_sdl2* s);

void rose_sys_sdl2_free(rose_system_sdl2* s);
