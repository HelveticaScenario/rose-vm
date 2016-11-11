#ifndef RUNTIME_GAME_H
#define RUNTIME_GAME_H

#include "config.h"
#include "cartridge.h"
#include "runtime_base.h"
#include "runtime_api.h"

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

#endif
