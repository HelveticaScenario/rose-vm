#include "runtime_api.h"

Rose_RuntimeApiError rose_runtime_base_poke(Rose_RuntimeBase* r, uint32_t idx, uint8_t val) {
    if (idx >= r->mem_size) {
        return ROSE_API_ERR_OUT_OF_BOUNDS;
    }
    r->mem[idx] = val;
    return ROSE_API_ERR_NONE;
}

Rose_RuntimeApiError rose_runtime_base_peek(Rose_RuntimeBase* r, uint32_t idx, uint8_t* res) {
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
    return rose_runtime_base_pset(r, x, y, *r->pen_color_addr);
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

void rect_swap(int16_t* x0, int16_t* y0, int16_t* x1, int16_t* y1) {
    bool swap_x = *x0 > *x1;
    bool swap_y = *y0 > *y1;
    if (swap_x) {
        int16_t tmp = *x0;
        *x0 = *x1;
        *x1 = tmp;
    }
    if (swap_y) {
        int16_t tmp = *y0;
        *y0 = *y1;
        *y1 = tmp;
    }
}

void rect_cam_offset(Rose_RuntimeBase* r, int16_t* x0, int16_t* y0, int16_t* x1, int16_t* y1) {
    int16_t* camera_offset = (int16_t*) r->camera_offset->begin;
    int16_t x_cam = camera_offset[0];
    int16_t y_cam = camera_offset[1];
    
    // Offset
    *x0 -= x_cam;
    *x1 -= x_cam;
    *y0 -= y_cam;
    *y1 -= y_cam;
}

int rect_clip(int16_t* x0, int16_t* y0, int16_t* x1, int16_t* y1) {
    if ((*x0 < 0 && *x1 < 0) || (*x0 > (ROSE_SCREEN_WIDTH-1) && *x0 > (ROSE_SCREEN_WIDTH-1)) || (*y0 < 0 && *y1 < 0) || (*y0 > (ROSE_SCREEN_HEIGHT-1) && *y0 > (ROSE_SCREEN_HEIGHT-1))) {
        return 1;
    }
    if (*x0 < 0) {
        *x0 = 0;
    }
    if (*x1 > (ROSE_SCREEN_WIDTH-1)) {
        *x1 = (ROSE_SCREEN_WIDTH-1);
    }
    if (*y0 < 0) {
        *y0 = 0;
    }
    if (*y1 > (ROSE_SCREEN_HEIGHT-1)) {
        *y1 = (ROSE_SCREEN_HEIGHT-1);
    }
    return 0;
}

Rose_RuntimeApiError rose_runtime_base_rect(Rose_RuntimeBase* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t col) {
    rect_swap(&x0, &y0, &x1, &y1);
    rect_cam_offset(r, &x0, &y0, &x1, &y1);

    // Draw
    int i;
    if (x0 >= 0 && x0 < ROSE_SCREEN_WIDTH) {
        for (i = y0; i<=y1; ++i) {
            if (i >= 0 && i < ROSE_SCREEN_HEIGHT) {
                rose_runtime_base_pset(r, x0, i, col);
            }
        }
    }

    if (x1 >= 0 && x1 < ROSE_SCREEN_WIDTH) {
        for (i = y0; i<=y1; ++i) {
            if (i >= 0 && i < ROSE_SCREEN_HEIGHT) {
                rose_runtime_base_pset(r, x1, i, col);
            }
        }
    }

     if (y0 >= 0 && y0 < ROSE_SCREEN_HEIGHT) {
        for (i = x0; i<=x1; ++i) {
            if (i >= 0 && i < ROSE_SCREEN_WIDTH) {
                rose_runtime_base_pset(r, i, y0, col);
            }
        }
    }

    if (y1 >= 0 && y1 < ROSE_SCREEN_HEIGHT) {
        for (i = x0; i<=x1; ++i) {
            if (i >= 0 && i < ROSE_SCREEN_WIDTH) {
                rose_runtime_base_pset(r, i, y1, col);
            }
        }
    }
    
    return ROSE_API_ERR_NONE;
}

Rose_RuntimeApiError rose_runtime_base_rect_default(Rose_RuntimeBase* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
    return rose_runtime_base_rect(r, x0, y0, x1, y1, *r->pen_color_addr);
}

Rose_RuntimeApiError rose_runtime_base_rectfill(Rose_RuntimeBase* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t col) {
    rect_swap(&x0, &y0, &x1, &y1);
    rect_cam_offset(r, &x0, &y0, &x1, &y1);
    int res = rect_clip(&x0, &y0, &x1, &y1);
    if (res == 1) 
        return ROSE_API_ERR_NONE;
    // Draw
    int i;
    for (i = y0; i<=y1; ++i) {
        memset(r->screen->begin + (i * ROSE_SCREEN_WIDTH) + x0, col, x1-x0+1);
    }
    return ROSE_API_ERR_NONE;
}

Rose_RuntimeApiError rose_runtime_base_rectfill_default(Rose_RuntimeBase* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
    return rose_runtime_base_rectfill(r, x0, y0, x1, y1, *r->pen_color_addr);
}

Rose_RuntimeApiError rose_runtime_base_cls(Rose_RuntimeBase* r) {
    memset(r->screen->begin, 0, r->screen->end - r->screen->begin);
    return ROSE_API_ERR_NONE;
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
    uint32_t idx = lua_tointeger(L, 1);
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
        uint32_t idx = lua_tointeger(L, 1);
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
        uint16_t x = lua_tointeger(L, 1);
        uint16_t y = lua_tointeger(L, 2);
        int err = rose_runtime_base_pset_default(r, x, y);
        switch (err) {
            case ROSE_API_ERR_OUT_OF_BOUNDS:
                return luaL_error(L, "Bad Memory Access");
            case ROSE_API_ERR_NONE:
            default:
                break;
        }
    } else if (nargs >= 3) {
        uint16_t x = lua_tointeger(L, 1);
        uint16_t y = lua_tointeger(L, 2);
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
        uint16_t x = lua_tointeger(L, 1);
        uint16_t y = lua_tointeger(L, 2);
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

typedef Rose_RuntimeApiError (*rect_fp)(Rose_RuntimeBase* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t col);
typedef Rose_RuntimeApiError (*rect_default_fp)(Rose_RuntimeBase* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1);

int rect_common(lua_State* L, rect_fp rect, rect_default_fp rect_default) {
    Rose_RuntimeBase* r = get_runtime_base(L);
    if (r == NULL) { return 0; }
    int nargs = lua_gettop(L);
    int err = ROSE_API_ERR_NONE;
    if (nargs >= 4) {
        int16_t x0 = lua_tointeger(L, 1);
        int16_t y0 = lua_tointeger(L, 2);
        int16_t x1 = lua_tointeger(L, 3);
        int16_t y1 = lua_tointeger(L, 4);
        if (nargs == 5) {
            uint8_t col = lua_tointeger(L, 5);
            err = rect(r, x0, y0, x1, y1, col);
        } else {
            err = rect_default(r, x0, y0, x1, y1);
        }
        
    } else if (nargs == 3) {
        int16_t x0 = lua_tointeger(L, 1);
        int16_t y0 = lua_tointeger(L, 2);
        int16_t x1 = lua_tointeger(L, 3);
        err = rect_default(r, x0, y0, x1, 0);
    } else if (nargs >= 1) {
        int16_t x1 = lua_tointeger(L, 1);
        int16_t y1 = 0;
        if (nargs == 2) {
            y1 = lua_tointeger(L, 2);
        }
        err = rect_default(r, 0, 0, x1, y1);
    }

    switch (err) {
        case ROSE_API_ERR_OUT_OF_BOUNDS:
            return luaL_error(L, "Bad Memory Access");
            break;
        case ROSE_API_ERR_NONE:
        default:
            break;
    }
    return 0;
}

int rose_runtime_base_lua_rect(lua_State* L) {
    return rect_common(L, &rose_runtime_base_rect, &rose_runtime_base_rect_default);
}

int rose_runtime_base_lua_rectfill(lua_State* L) {
    return rect_common(L, &rose_runtime_base_rectfill, &rose_runtime_base_rectfill_default);
}

int rose_runtime_base_lua_cls(lua_State* L) {
    Rose_RuntimeBase* r = get_runtime_base(L);
    if (r == NULL) { return 0; }
    int err = rose_runtime_base_cls(r);
    switch (err) {
        case ROSE_API_ERR_OUT_OF_BOUNDS:
            return luaL_error(L, "Bad Memory Access");
            break;
        case ROSE_API_ERR_NONE:
        default:
            break;
    }
    return 0;
}