#include "runtime_api.h"

Rose_RuntimeApiError rose_runtime_base_poke(Rose_RuntimeBase* r, uint_fast32_t idx, uint8_t val) {
    if (idx >= r->mem_size) {
        return ROSE_API_ERR_OUT_OF_BOUNDS;
    }
    r->mem[idx] = val;
    return ROSE_API_ERR_NONE;
}

Rose_RuntimeApiError rose_runtime_base_peek(Rose_RuntimeBase* r, uint_fast32_t idx, uint8_t* res) {
    if (idx >= r->mem_size) {
        return ROSE_API_ERR_OUT_OF_BOUNDS;
    }
    *res = r->mem[idx];
    return ROSE_API_ERR_NONE;
}

Rose_RuntimeApiError rose_runtime_base_pset(Rose_RuntimeBase* r, int32_t x, int32_t y, uint8_t val) {
    if (x >= 0 && x < ROSE_SCREEN_WIDTH && y >= 0 && y < ROSE_SCREEN_HEIGHT) {
        Rose_MemoryRange* screen = r->screen;
        *(screen->begin + (y * ROSE_SCREEN_WIDTH) + x) = val;
    }
    return ROSE_API_ERR_NONE;
}

Rose_RuntimeApiError rose_runtime_base_pset_default(Rose_RuntimeBase* r, int32_t x, int32_t y) {
    return rose_runtime_base_pset(r, x, y, 1);
}

Rose_RuntimeApiError rose_runtime_base_pget(Rose_RuntimeBase* r, int32_t x, int32_t y, uint8_t* res) {
    if (x >= 0 && x < ROSE_SCREEN_WIDTH && y >= 0 && y < ROSE_SCREEN_HEIGHT) {
        Rose_MemoryRange* screen = r->screen;
        *res = *(screen->begin + (y * ROSE_SCREEN_WIDTH) + x);
    } else {
        *res = 0;
    }
    return ROSE_API_ERR_NONE;
}

Rose_RuntimeApiError rose_runtime_base_palset(Rose_RuntimeBase* r, uint8_t idx, Rose_Color c) {
    Rose_MemoryIterator slot = r->palette->begin + (idx * 3);
    *slot = c.r;
    *(slot + 1) = c.g;
    *(slot + 2) = c.b;
    return ROSE_API_ERR_NONE;
}

Rose_RuntimeApiError rose_runtime_base_palget(Rose_RuntimeBase* r, uint8_t idx, Rose_Color* res) {
    Rose_MemoryIterator slot = r->palette->begin + (idx * 3);
    res->r = *slot;
    res->g = *(slot + 1);
    res->b = *(slot + 2);
    return ROSE_API_ERR_NONE;
}

Rose_RuntimeApiError rose_runtime_base_rect(Rose_RuntimeBase* r, uint_fast16_t x0, uint_fast16_t y0, uint_fast16_t x1, uint_fast16_t y1, uint8_t col) {

}

Rose_RuntimeApiError rose_runtime_base_rect_default(Rose_RuntimeBase* r, uint_fast16_t x0, uint_fast16_t y0, uint_fast16_t x1, uint_fast16_t y1) {

}

Rose_RuntimeApiError rose_runtime_base_rectfill(Rose_RuntimeBase* r, uint_fast16_t x0, uint_fast16_t y0, uint_fast16_t x1, uint_fast16_t y1, uint8_t col) {

}

Rose_RuntimeApiError rose_runtime_base_rectfill_default(Rose_RuntimeBase* r, uint_fast16_t x0, uint_fast16_t y0, uint_fast16_t x1, uint_fast16_t y1) {

}

Rose_RuntimeApiError rose_runtime_base_cls(Rose_RuntimeBase* r) {
    
}

void rose_runtime_register_api(lua_State* L, Rose_RuntimeBase* r) {
    rose_runtime_register_lua_function(L, r, rose_runtime_base_lua_poke, "poke");
    rose_runtime_register_lua_function(L, r, rose_runtime_base_lua_peek, "peek");
    rose_runtime_register_lua_function(L, r, rose_runtime_base_lua_pset, "pset");
    rose_runtime_register_lua_function(L, r, rose_runtime_base_lua_pget, "pget");
    rose_runtime_register_lua_function(L, r, rose_runtime_base_lua_palset, "palset");
    rose_runtime_register_lua_function(L, r, rose_runtime_base_lua_palget, "palget");
    rose_runtime_register_lua_function(L, r, rose_runtime_base_lua_rect, "rect");
    rose_runtime_register_lua_function(L, r, rose_runtime_base_lua_rectfill, "rectfill");
    rose_runtime_register_lua_function(L, r, rose_runtime_base_lua_cls, "cls");
}

void rose_runtime_register_lua_function(lua_State* L, Rose_RuntimeBase* r, lua_CFunction fun, const char* name) {
    lua_pushlightuserdata(L, r);
    lua_pushcclosure(L, fun, 1);
    lua_setglobal(L, name);
}

Rose_RuntimeBase* get_runtime_base(lua_State* L) {
    Rose_RuntimeBase* r = NULL;
    int uidx = lua_upvalueindex(1);
    if (lua_islightuserdata(L, uidx) == 1) {
        r = lua_touserdata(L, uidx);
    } else {
        luaL_error(L, "something went horribly wrong: upvalueindex 1 was not a lightuserdata");
    }
    return r;
}

int rose_runtime_base_lua_poke(lua_State* L) {
    Rose_RuntimeBase* r = get_runtime_base(L);
    if (r == NULL) { return 0; }
    uint_fast32_t idx = lua_tointeger(L, 1);
    uint8_t val = lua_tointeger(L, 2);
    int err = rose_runtime_base_poke(r, idx, val);
    switch (err) {
        case ROSE_API_ERR_OUT_OF_BOUNDS:
            return luaL_error(L, "Bad Memory Access");
        case ROSE_API_ERR_NONE:
        default:
            break;
    }
    return 0;
}

int rose_runtime_base_lua_peek(lua_State* L) {
    Rose_RuntimeBase* r = get_runtime_base(L);
    if (r == NULL) { return 0; }
    int nargs = lua_gettop(L);
    if (nargs >= 1) {
        uint_fast32_t idx = lua_tointeger(L, 1);
        uint8_t res;
        int err = rose_runtime_base_peek(r, idx, &res);
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



int rose_runtime_base_lua_pset(lua_State* L) {
    Rose_RuntimeBase* r = get_runtime_base(L);
    if (r == NULL) { return 0; }
    int nargs = lua_gettop(L);
    if (nargs == 2) {
        uint_fast16_t x = lua_tointeger(L, 1);
        uint_fast16_t y = lua_tointeger(L, 2);
        int err = rose_runtime_base_pset_default(r, x, y);
        switch (err) {
            case ROSE_API_ERR_OUT_OF_BOUNDS:
                return luaL_error(L, "Bad Memory Access");
            case ROSE_API_ERR_NONE:
            default:
                break;
        }
    } else if (nargs >= 3) {
        uint_fast16_t x = lua_tointeger(L, 1);
        uint_fast16_t y = lua_tointeger(L, 2);
        uint8_t val = lua_tointeger(L, 3);
        int err = rose_runtime_base_pset(r, x, y, val);
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

int rose_runtime_base_lua_pget(lua_State* L) {
    Rose_RuntimeBase* r = get_runtime_base(L);
    if (r == NULL) { return 0; }
    int nargs = lua_gettop(L);
    if (nargs >= 2) {
        uint_fast16_t x = lua_tointeger(L, 1);
        uint_fast16_t y = lua_tointeger(L, 2);
        uint8_t res;
        int err = rose_runtime_base_pget(r, x, y, &res);
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

int rose_runtime_base_lua_palset(lua_State* L) {
    Rose_RuntimeBase* r = get_runtime_base(L);
    if (r == NULL) { return 0; }
    int nargs = lua_gettop(L);
    if (nargs >= 4) {
        uint8_t idx = lua_tointeger(L, 1);
        uint8_t red = lua_tointeger(L, 2);
        uint8_t green = lua_tointeger(L, 3);
        uint8_t blue = lua_tointeger(L, 4);
        Rose_Color c = {.r = red, .g = green, .b = blue};
        int err = rose_runtime_base_palset(r, idx, c);
        switch (err) {
            case ROSE_API_ERR_OUT_OF_BOUNDS:
                return luaL_error(L, "Bad Memory Access");
                break;
            case ROSE_API_ERR_NONE:
            default:
                break;
        }
    }
    return 0;
}

int rose_runtime_base_lua_palget(lua_State* L) {
    Rose_RuntimeBase* r = get_runtime_base(L);
    if (r == NULL) { return 0; }
    int nargs = lua_gettop(L);
    if (nargs >= 1) {
        uint8_t idx = lua_tointeger(L, 1);
        Rose_Color res;
        int err = rose_runtime_base_palget(r, idx, &res);
        switch (err) {
            case ROSE_API_ERR_OUT_OF_BOUNDS:
                return luaL_error(L, "Bad Memory Access");
                break;
            case ROSE_API_ERR_NONE:
            default:
                lua_pushinteger(L, res.r);
                lua_pushinteger(L, res.g);
                lua_pushinteger(L, res.b);
                break;
        }
    }
    return 3;
}

int rose_runtime_base_lua_rect(lua_State* L) {

}

int rose_runtime_base_lua_rectfill(lua_State* L) {

}

int rose_runtime_base_lua_cls(lua_State* L) {

}