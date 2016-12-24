#include <types.h>
#include "rt/rt_game.h"

rose_runtime_game* rose_runtime_game_create(rose_fs* fs) {
    if (fs == NULL) {
        fprintf(stderr, "tried to create runtime game with null fs\n");
        exit(1);
    }
    rose_runtime_game* r = (rose_runtime_game*) malloc(sizeof(rose_runtime_game));
    r->base = rose_runtime_base_create(fs);
    rose_runtime_game_reload(r);
    return r;
}

void rose_runtime_game_free(rose_runtime_game* r) {
    rose_runtime_base_free(r->base);
    free(r);
}

bool rose_runtime_game_reload(rose_runtime_game* r) {
    rose_runtime_base_clear(r->base);
    // insert any game runtime specific init code here
    return rose_runtime_base_load_run_main(r->base);
}

rose_runtime_game_error rose_pcall(rose_runtime_game* r, uint8_t nargs) {
    if (lua_isfunction(r->base->lua, -1) == 0) {
        lua_settop(r->base->lua, 0);
        return ROSE_RT_GAME_FUN_NOT_FOUND;
    }
    //TODO: add stop the world and output error functionality
    int res = lua_pcall(r->base->lua, nargs, 0, 0);
    switch (res) {
        case LUA_ERRRUN:
        case LUA_ERRMEM:
        case LUA_ERRERR:
            printf("%s\n", lua_tostring(r->base->lua, -1));
            lua_settop(r->base->lua, 0);
            return ROSE_RT_GAME_CRITICAL_ERR;
            break;
        case 0:
        default:
            lua_settop(r->base->lua, 0);
            return ROSE_RT_GAME_NO_ERR;
    }
}

rose_runtime_game_error rose_runtime_game_init(rose_runtime_game* r) {
    lua_getglobal(r->base->lua, "_init");
    return rose_pcall(r, 0);
}

rose_runtime_game_error rose_runtime_game_update(rose_runtime_game* r) {
    lua_getglobal(r->base->lua, "_update");
    return rose_pcall(r, 0);
}

rose_runtime_game_error rose_runtime_game_draw(rose_runtime_game* r) {
    lua_getglobal(r->base->lua, "_draw");
    return rose_pcall(r, 0);
}

rose_runtime_game_error rose_runtime_game_onmouse(rose_runtime_game* r, int16_t x, int16_t y) {
    lua_getglobal(r->base->lua, "_onmouse");
    lua_pushinteger(r->base->lua, x);
    lua_pushinteger(r->base->lua, y);
    return rose_pcall(r, 2);
}

rose_runtime_game_error rose_runtime_game_onwheel(rose_runtime_game* r, int16_t x, int16_t y, bool inverted) {
    lua_getglobal(r->base->lua, "_onwheel");
    lua_pushinteger(r->base->lua, x);
    lua_pushinteger(r->base->lua, y);
    lua_pushboolean(r->base->lua, inverted);
    return rose_pcall(r, 3);
}

rose_runtime_game_error rose_runtime_game_onbtn(rose_runtime_game* r, uint8_t code, bool pressed) {
    lua_getglobal(r->base->lua, "_onbtn");
    lua_pushinteger(r->base->lua, code);
    lua_pushboolean(r->base->lua, pressed);
    return rose_pcall(r, 2);
}

rose_runtime_game_error rose_runtime_game_onkey(rose_runtime_game* r, rose_keycode keycode, bool pressed, bool repeat) {
    lua_getglobal(r->base->lua, "_onkey");
    lua_pushinteger(r->base->lua, keycode);
    lua_pushboolean(r->base->lua, pressed);
    lua_pushboolean(r->base->lua, repeat);
    return rose_pcall(r, 3);
}

rose_runtime_game_error rose_runtime_game_ontouch(rose_runtime_game* r) {
    // TODO: make this actually do something
    lua_getglobal(r->base->lua, "_ontouch");
    return rose_pcall(r, 0);
}
