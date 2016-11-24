#ifndef ROSE_SYS_SDL2_H
#define ROSE_SYS_SDL2_H

#include <SDL.h>
#include <zlib.h>
#include <archive.h>
#include "config.h"
#include "rt/rt_base.h"
#include "rt/rt_game.h"
#include "cartridge.h"

typedef struct {
    // The window we'll be rendering to
    SDL_Window *window;

    // The window renderer
    SDL_Renderer *renderer;

    // The actual hardware texture
    SDL_Texture *texture;

    void *pixels;
    int32_t pitch;
    int32_t windowWidth;
    int32_t windowHeight;
    // Image dimensions
    uint16_t widthMult;
    uint16_t heightMult;

    Rose_Cartridge *cartridge;

    // RuntimeEditor *editor;

    Rose_RuntimeGame *game;

    Rose_ScreenMode screenMode;
} Rose_SystemSdl2;

bool rose_sys_sdl2_init(Rose_SystemSdl2* s, int argc, char* argv[]);

void rose_sys_sdl2_run(Rose_SystemSdl2* s);

void rose_sys_sdl2_free(Rose_SystemSdl2* s);

#endif