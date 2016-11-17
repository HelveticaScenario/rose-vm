#ifndef SYSTEM_SDL2_HPP
#define SYSTEM_SDL2_HPP

#include <SDL.h>
#include <zlib.h>
#include <archive.h>
#include "config.h"
#include "system_interface.h"
#include "runtime_base.h"
#include "runtime_api.h"
#include "runtime_game.h"
// #include "runtime_editor.hpp"
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

bool rose_init(Rose_SystemSdl2* s, int argc, char* argv[]);

void rose_run(Rose_SystemSdl2* s);

void rose_free(Rose_SystemSdl2* s);

#endif