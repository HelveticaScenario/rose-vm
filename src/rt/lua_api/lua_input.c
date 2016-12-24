#include "rt/lua_api/lua_input.h"

int rose_lua_input_mouse(lua_State* L) {
    rose_runtime_base* r = rose_lua_base_get_runtime_base(L);
    if (r == NULL) { return 0; }
    int16_t x = 0;
    int16_t y = 0;
    int err = rose_api_input_mouse(r, &x, &y);
    switch (err) {
        case ROSE_API_ERR_OUT_OF_BOUNDS:
            return luaL_error(L, "Bad Memory Access");
            break;
        case ROSE_API_ERR_NONE:
        default:
            lua_pushinteger(L, x);
            lua_pushinteger(L, y);
            break;
    }
    return 2;
}

int rose_lua_input_btn(lua_State* L) {
    rose_runtime_base* r = rose_lua_base_get_runtime_base(L);
    if (r == NULL) { return 0; }
    int nargs = lua_gettop(L);
    if (nargs >= 1) {
        uint32_t i;
        for (i = 0; i < nargs; i++) {
            uint8_t idx = (uint8_t) lua_tointeger(L, i + 1);
            if (idx > 31) {
                idx = 31;
            }
            bool res;
            int err = rose_api_input_btn(r, idx, &res);
            switch (err) {
                case ROSE_API_ERR_OUT_OF_BOUNDS:
                    return luaL_error(L, "Bad Memory Access");
                    break;
                case ROSE_API_ERR_NONE:
                default:
                    lua_pushboolean(L, res);
                    break;
            }
        }
        return nargs;
    } else {
        return 0;
    }
}

int rose_lua_input_wheel(lua_State* L) {
    rose_runtime_base* r = rose_lua_base_get_runtime_base(L);
    if (r == NULL) { return 0; }
    bool to_invert;
    rose_api_input_wheel_inverted(r, &to_invert);
    int nargs = lua_gettop(L);
    if (nargs > 0) {
        to_invert = (bool) lua_toboolean(L, 1);
    }
    int16_t x = 0;
    int16_t y = 0;
    int err = rose_api_input_wheel(r, &x, &y);
    if (to_invert) {
        x *= -1;
        y *= -1;
    }
    switch (err) {
        case ROSE_API_ERR_OUT_OF_BOUNDS:
            return luaL_error(L, "Bad Memory Access");
            break;
        case ROSE_API_ERR_NONE:
        default:
            lua_pushinteger(L, x);
            lua_pushinteger(L, y);
            break;
    }
    return 2;
}

int rose_lua_input_key(lua_State* L) {
    rose_runtime_base* r = rose_lua_base_get_runtime_base(L);
    if (r == NULL) { return 0; }
    int nargs = lua_gettop(L) - 1; // first arg will always be self
    if (nargs >= 1) {
        uint32_t i;
        for (i = 0; i < nargs; i++) {
            const char* type = luaL_typename(L, i + 2);
            uint8_t idx = (uint8_t) lua_tointeger(L, i + 2);
            if (idx >= ROSE_KEYCODE_UNKNOWN) {
                idx = ROSE_KEYCODE_UNKNOWN - 1;
            }
            bool res;
            int err = rose_api_input_key(r, (rose_keycode) idx, &res);
            switch (err) {
                case ROSE_API_ERR_OUT_OF_BOUNDS:
                    return luaL_error(L, "Bad Memory Access");
                    break;
                case ROSE_API_ERR_NONE:
                default:
                    lua_pushboolean(L, res);
                    break;
            }
        }
        return nargs;
    } else {
        return 0;
    }
}