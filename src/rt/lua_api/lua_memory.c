#include "rt/lua_api/lua_memory.h"

int rose_lua_memory_poke(lua_State* L) {
    Rose_RuntimeBase* r = rose_lua_base_get_runtime_base(L);
    if (r == NULL) { return 0; }
    uint32_t idx = lua_tointeger(L, 1);
    uint8_t val = lua_tointeger(L, 2);
    int err = rose_api_memory_poke(r, idx, val);
    switch (err) {
        case ROSE_API_ERR_OUT_OF_BOUNDS:
            return luaL_error(L, "Bad Memory Access");
        case ROSE_API_ERR_NONE:
        default:
            break;
    }
    return 0;
}

int rose_lua_memory_peek(lua_State* L) {
    Rose_RuntimeBase* r = rose_lua_base_get_runtime_base(L);
    if (r == NULL) { return 0; }
    int nargs = lua_gettop(L);
    if (nargs >= 1) {
        uint32_t idx = lua_tointeger(L, 1);
        uint8_t res;
        int err = rose_api_memory_peek(r, idx, &res);
        switch (err) {
            case ROSE_API_ERR_OUT_OF_BOUNDS:
                return luaL_error(L, "Bad Memory Access");
                break;
            case ROSE_API_ERR_NONE:
            default:
                lua_pushinteger(L, res);
                break;
        }
    } else {
        lua_pushinteger(L, 0);
    }
    return 1;
}

int rose_lua_memory_memcpy(lua_State* L) {
    Rose_RuntimeBase* r = rose_lua_base_get_runtime_base(L);
    if (r == NULL) { return 0; }
    int nargs = lua_gettop(L);
    if (nargs >= 3) {
        uint32_t dest_addr = lua_tointeger(L, 1);
        uint32_t source_addr = lua_tointeger(L, 2);
        uint32_t len = lua_tointeger(L, 3);
        int err = rose_api_memory_memcpy(r, dest_addr, source_addr, len);
        switch (err) {
            case ROSE_API_ERR_OUT_OF_BOUNDS:
                return luaL_error(L, "Bad Memory Access");
            case ROSE_API_ERR_NONE:
            default:
                break;
        }
    }
    
    return 0;
}

int rose_lua_memory_memset(lua_State* L) {
    Rose_RuntimeBase* r = rose_lua_base_get_runtime_base(L);
    if (r == NULL) { return 0; }
    int nargs = lua_gettop(L);
    if (nargs >= 3) {
        uint32_t dest_addr = lua_tointeger(L, 1);
        uint8_t val = lua_tointeger(L, 2);
        uint32_t len = lua_tointeger(L, 3);
        int err = rose_api_memory_memset(r, dest_addr, val, len);
        switch (err) {
            case ROSE_API_ERR_OUT_OF_BOUNDS:
                return luaL_error(L, "Bad Memory Access");
            case ROSE_API_ERR_NONE:
            default:
                break;
        }
    }
    
    return 0;
}
