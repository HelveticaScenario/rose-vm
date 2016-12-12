#include "rt/lua_api.h"

void rose_lua_register_api(lua_State* L, Rose_RuntimeBase* r) {
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
}

void rose_lua_register_function(lua_State* L, Rose_RuntimeBase* r, lua_CFunction fun, const char* name) {
    lua_pushlightuserdata(L, r);
    lua_pushcclosure(L, fun, 1);
    lua_setglobal(L, name);
}
