#ifndef ROSE_RT_GAME_H
#define ROSE_RT_GAME_H

#include "config.h"
#include "rt/fs/fs_cartridge.h"
#include "rt/rt_base.h"
#include "rt/lua_api.h"

typedef enum {
    ROSE_RT_GAME_FUN_NOT_FOUND,
    ROSE_RT_GAME_NO_ERR,
    ROSE_RT_GAME_CRITICAL_ERR
} Rose_RuntimeGameError;

typedef struct {
    Rose_Cartridge* cartridge;
    lua_State* lua;
    Rose_RuntimeBase* base;
} Rose_RuntimeGame;

Rose_RuntimeGame* rose_runtime_game_create(Rose_Cartridge* cartridge);

void rose_runtime_game_free(Rose_RuntimeGame* r);

bool rose_runtime_game_reload(Rose_RuntimeGame* r);

Rose_RuntimeGameError rose_runtime_game_init(Rose_RuntimeGame* r);

Rose_RuntimeGameError rose_runtime_game_update(Rose_RuntimeGame* r);

Rose_RuntimeGameError rose_runtime_game_draw(Rose_RuntimeGame* r);

Rose_RuntimeGameError rose_runtime_game_onmouse(Rose_RuntimeGame* r, int16_t x, int16_t y);

Rose_RuntimeGameError rose_runtime_game_onwheel(Rose_RuntimeGame* r, int16_t x, int16_t y, bool inverted);

Rose_RuntimeGameError rose_runtime_game_onbtn(Rose_RuntimeGame* r, uint8_t code, bool pressed);

Rose_RuntimeGameError rose_runtime_game_onkey(Rose_RuntimeGame* r, Rose_KeyCode keycode, bool pressed, bool repeat);

Rose_RuntimeGameError rose_runtime_game_ontouch(Rose_RuntimeGame* r);

#endif
