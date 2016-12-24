#include "rt/lua_api.h"

void rose_lua_register_api(lua_State* L, rose_runtime_base* r) {
    rose_lua_register_function(L, r, rose_lua_memory_poke, "poke");
    rose_lua_register_function(L, r, rose_lua_memory_peek, "peek");
    rose_lua_register_function(L, r, rose_lua_memory_memcpy, "memcpy");
    rose_lua_register_function(L, r, rose_lua_memory_memset, "memset");

    rose_lua_register_function(L, r, rose_lua_graphics_pset, "pset");
    rose_lua_register_function(L, r, rose_lua_graphics_pget, "pget");
    rose_lua_register_function(L, r, rose_lua_graphics_palset, "palset");
    rose_lua_register_function(L, r, rose_lua_graphics_palget, "palget");
    rose_lua_register_function(L, r, rose_lua_graphics_line, "line");
    rose_lua_register_function(L, r, rose_lua_graphics_rect, "rect");
    rose_lua_register_function(L, r, rose_lua_graphics_rectfill, "rectfill");
    rose_lua_register_function(L, r, rose_lua_graphics_circ, "circ");
    rose_lua_register_function(L, r, rose_lua_graphics_circfill, "circfill");
    rose_lua_register_function(L, r, rose_lua_graphics_cls, "cls");

    rose_lua_register_function(L, r, rose_lua_input_mouse, "mouse");
    rose_lua_register_function(L, r, rose_lua_input_btn, "btn");
    rose_lua_register_function(L, r, rose_lua_input_wheel, "wheel");
    rose_lua_register_key_table(L, r);
}

void rose_lua_register_function(lua_State* L, rose_runtime_base* r, lua_CFunction fun, const char* name) {
    lua_pushlightuserdata(L, r);
    lua_pushcclosure(L, fun, 1);
    lua_setglobal(L, name);
}

void rose_lua_register_key_table(lua_State* L, rose_runtime_base* r) {
    lua_createtable (L, 0, ROSE_KEYCODE_UNKNOWN /* last keycode */);
    rose_keycode keycode;
    for (keycode = ROSE_KEYCODE_A; keycode < ROSE_KEYCODE_UNKNOWN; ++keycode) {
        const char * name = rose_keycode_to_string(keycode);
        lua_pushinteger(L, keycode);
        lua_setfield(L, 1, name);
    }
    lua_createtable (L, 0, 1);
    lua_pushlightuserdata(L, r);
    lua_pushcclosure(L, rose_lua_input_key, 1);
    lua_setfield(L, 2, "__call");
    lua_setmetatable(L, 1);
    lua_setglobal(L, "key");
}