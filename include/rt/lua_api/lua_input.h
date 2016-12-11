#ifndef ROSE_LUA_INPUT_H
#define ROSE_LUA_INPUT_H

#include "config.h"
#include "rt/lua_api/lua_base.h"
#include "rt/api/api_input.h"

int rose_lua_input_mouse(lua_State* L);

int rose_lua_input_btn(lua_State* L);

int rose_lua_input_wheel(lua_State* L);

#endif