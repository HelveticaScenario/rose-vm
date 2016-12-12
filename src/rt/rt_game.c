#include "rt/rt_game.h"

Rose_RuntimeGame* rose_runtime_game_create(Rose_Cartridge* cartridge) {
    Rose_RuntimeGame* r = (Rose_RuntimeGame*) malloc(sizeof(Rose_RuntimeGame));
    r->cartridge = cartridge;
    r->base = rose_runtime_base_create(cartridge);
    r->lua = NULL;
    rose_runtime_game_reload(r);
    return r;
}

void rose_runtime_game_free(Rose_RuntimeGame* r) {
    rose_runtime_base_free(r->base);
    lua_close(r->lua);
    free(r);
}

bool rose_runtime_game_reload(Rose_RuntimeGame* r) {
    memset(r->base->mem, 0, r->base->mem_size);
    if (r->cartridge->data_size > (r->base->mem_size - ROSE_RUNTIME_RESERVED_MEMORY_SIZE)) {
        fprintf(stderr, "ERROR: tried to reload runtime and cartridge memory size was bigger than available memory size\n");
        exit(1);
    }
    memcpy(r->base->mem, r->cartridge->data, r->cartridge->data_size);
    if (r->lua != NULL) lua_close(r->lua);
    r->lua = luaL_newstate();
    luaL_openlibs(r->lua);
    rose_lua_register_api(r->lua, r->base);
    int ret;
    ret = luaL_loadbuffer(r->lua, r->cartridge->code, r->cartridge->code_size, "main");
    if (ret != 0) {
        fprintf(stderr, "%s\r\n", luaL_checkstring(r->lua, -1));
        return false;
    }
    ret = lua_pcall(r->lua, 0, LUA_MULTRET, 0);
    if (ret != 0) {
        fprintf(stderr, "%s\r\n", luaL_checkstring(r->lua, -1));
        return false;
    }

    return true;
}

Rose_RuntimeGameError rose_pcall(Rose_RuntimeGame* r) {
    if (lua_isfunction(r->lua, -1) == 0) {
        return ROSE_RT_GAME_FUN_NOT_FOUND;
    }
    //TODO: add stop the world and output error functionality
    int res = lua_pcall(r->lua, 0, 0, 0);
    switch (res) {
        case LUA_ERRRUN:
        case LUA_ERRMEM:
        case LUA_ERRERR:
            printf("%s\n", lua_tostring(r->lua, -1));
            return ROSE_RT_GAME_CRITICAL_ERR;
            break;
        case 0:
        default:
            return ROSE_RT_GAME_NO_ERR;
    }
}

Rose_RuntimeGameError rose_runtime_game_init(Rose_RuntimeGame* r) {
    lua_getglobal(r->lua, "_init");
    return rose_pcall(r);
}

Rose_RuntimeGameError rose_runtime_game_update(Rose_RuntimeGame* r) {
    lua_getglobal(r->lua, "_update");
    return rose_pcall(r);
}

Rose_RuntimeGameError rose_runtime_game_draw(Rose_RuntimeGame* r) {
    lua_getglobal(r->lua, "_draw");
    return rose_pcall(r);
}

Rose_RuntimeGameError rose_runtime_game_onmouse(Rose_RuntimeGame* r, int16_t x, int16_t y) {
    lua_getglobal(r->lua, "_onmouse");
    return rose_pcall(r);
}

Rose_RuntimeGameError rose_runtime_game_onwheel(Rose_RuntimeGame* r, int16_t x, int16_t y, bool inverted) {
    lua_getglobal(r->lua, "_onwheel");
    return rose_pcall(r);
}

Rose_RuntimeGameError rose_runtime_game_onbtn(Rose_RuntimeGame* r, uint8_t code, bool pressed) {
    lua_getglobal(r->lua, "_onbtn");
    return rose_pcall(r);
}

Rose_RuntimeGameError rose_runtime_game_onkey(Rose_RuntimeGame* r, Rose_KeyCode keycode, bool pressed, bool repeat) {
    lua_getglobal(r->lua, "_onkey");
    return rose_pcall(r);
}

Rose_RuntimeGameError rose_runtime_game_ontouch(Rose_RuntimeGame* r) {
    lua_getglobal(r->lua, "_ontouch");
    return rose_pcall(r);
}