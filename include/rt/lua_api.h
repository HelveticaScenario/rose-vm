#ifndef ROSE_LUA_API_H
#define ROSE_LUA_API_H

#include "config.h"
#include "rt/rt_base.h"
#include "rt/lua_api/lua_base.h"
#include "rt/lua_api/lua_memory.h"
#include "rt/lua_api/lua_graphics.h"
#include "rt/lua_api/lua_input.h"

void rose_lua_register_api(lua_State* L, rose_runtime_base* r);

void rose_lua_register_function(lua_State* L, rose_runtime_base* r, lua_CFunction fun, const char* name);

void rose_lua_register_key_table(lua_State* L, rose_runtime_base* r);

#endif