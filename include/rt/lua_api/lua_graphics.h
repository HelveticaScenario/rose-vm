#ifndef ROSE_LUA_GRAPHICS_H
#define ROSE_LUA_GRAPHICS_H

#include "config.h"
#include "rt/lua_api/lua_base.h"
#include "rt/api/api_graphics.h"

int rose_lua_graphics_pset(lua_State* L);

int rose_lua_graphics_pget(lua_State* L);

int rose_lua_graphics_palset(lua_State* L);

int rose_lua_graphics_palget(lua_State* L);

int rose_lua_graphics_line(lua_State* L);

int rose_lua_graphics_rect(lua_State* L);

int rose_lua_graphics_rectfill(lua_State* L);

int rose_lua_graphics_circ(lua_State* L);

int rose_lua_graphics_circfill(lua_State* L);

int rose_lua_graphics_cls(lua_State* L);

#endif