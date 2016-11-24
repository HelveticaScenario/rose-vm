#ifndef ROSE_LUA_MEMORY_H
#define ROSE_LUA_MEMORY_H

#include "config.h"
#include "rt/lua_api/lua_base.h"
#include "rt/api/api_memory.h"

int rose_lua_memory_poke(lua_State* L);

int rose_lua_memory_peek(lua_State* L);

int rose_lua_memory_memcpy(lua_State* L);

int rose_lua_memory_memset(lua_State* L);

#endif