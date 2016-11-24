#include "rt/lua_api/lua_graphics.h"

int rose_lua_graphics_pset(lua_State* L) {
    Rose_RuntimeBase* r = rose_lua_base_get_runtime_base(L);
    if (r == NULL) { return 0; }
    int nargs = lua_gettop(L);
    if (nargs == 2) {
        uint16_t x = lua_tointeger(L, 1);
        uint16_t y = lua_tointeger(L, 2);
        int err = rose_api_graphics_pset_default(r, x, y);
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
        int err = rose_api_graphics_pset(r, x, y, val);
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

int rose_lua_graphics_pget(lua_State* L) {
    Rose_RuntimeBase* r = rose_lua_base_get_runtime_base(L);
    if (r == NULL) { return 0; }
    int nargs = lua_gettop(L);
    if (nargs >= 2) {
        uint16_t x = lua_tointeger(L, 1);
        uint16_t y = lua_tointeger(L, 2);
        uint8_t res;
        int err = rose_api_graphics_pget(r, x, y, &res);
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

int rose_lua_graphics_palset(lua_State* L) {
    Rose_RuntimeBase* r = rose_lua_base_get_runtime_base(L);
    if (r == NULL) { return 0; }
    int nargs = lua_gettop(L);
    if (nargs >= 4) {
        uint8_t idx = lua_tointeger(L, 1);
        uint8_t red = lua_tointeger(L, 2);
        uint8_t green = lua_tointeger(L, 3);
        uint8_t blue = lua_tointeger(L, 4);
        Rose_Color c = {.r = red, .g = green, .b = blue};
        int err = rose_api_graphics_palset(r, idx, c);
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

int rose_lua_graphics_palget(lua_State* L) {
    Rose_RuntimeBase* r = rose_lua_base_get_runtime_base(L);
    if (r == NULL) { return 0; }
    int nargs = lua_gettop(L);
    if (nargs >= 1) {
        uint8_t idx = lua_tointeger(L, 1);
        Rose_Color res;
        int err = rose_api_graphics_palget(r, idx, &res);
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

int rose_lua_graphics_line(lua_State* L) {
    Rose_RuntimeBase* r = rose_lua_base_get_runtime_base(L);
    if (r == NULL) { return 0; }
    int nargs = lua_gettop(L);
    int err;
    int16_t x0 = lua_tointeger(L, 1);
    int16_t y0 = lua_tointeger(L, 2);
    int16_t x1 = lua_tointeger(L, 3);
    int16_t y1 = lua_tointeger(L, 4);
    printf("%hd %hd %hd %hd\n", x0,y0,x1,y1);
    if (nargs >= 5) {
        uint8_t col = lua_tointeger(L, 5);
        err = rose_api_graphics_line(r, x0, y0, x1, y1, col);
    } else {
        err = rose_api_graphics_line_default(r, x0, y0, x1, y1);
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

typedef Rose_RuntimeApiError (*rect_fp)(Rose_RuntimeBase* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t col);
typedef Rose_RuntimeApiError (*rect_default_fp)(Rose_RuntimeBase* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1);

int rect_lua_common(lua_State* L, rect_fp rect, rect_default_fp rect_default) {
    Rose_RuntimeBase* r = rose_lua_base_get_runtime_base(L);
    if (r == NULL) { return 0; }
    int nargs = lua_gettop(L);
    int err = ROSE_API_ERR_NONE;
    if (nargs >= 4) {
        int16_t x0 = lua_tointeger(L, 1);
        int16_t y0 = lua_tointeger(L, 2);
        int16_t x1 = lua_tointeger(L, 3);
        int16_t y1 = lua_tointeger(L, 4);
        if (nargs >= 5) {
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

int rose_lua_graphics_rect(lua_State* L) {
    return rect_lua_common(L, &rose_api_graphics_rect, &rose_api_graphics_rect_default);
}

int rose_lua_graphics_rectfill(lua_State* L) {
    return rect_lua_common(L, &rose_api_graphics_rectfill, &rose_api_graphics_rectfill_default);
}

typedef Rose_RuntimeApiError (*circ_fp)(Rose_RuntimeBase* r, int16_t x0, int16_t y0, uint16_t radius, uint8_t col);
typedef Rose_RuntimeApiError (*circ_default_fp)(Rose_RuntimeBase* r, int16_t x0, int16_t y0, uint16_t radius);

int circ_lua_common(lua_State* L, circ_fp circ, circ_default_fp circ_default) {
    Rose_RuntimeBase* r = rose_lua_base_get_runtime_base(L);
    if (r == NULL) { return 0; }
    int nargs = lua_gettop(L);
    int err = ROSE_API_ERR_NONE;
    if (nargs >= 2) {
        int16_t x0 = lua_tointeger(L, 1);
        int16_t y0 = lua_tointeger(L, 2);
        uint16_t radius;
        if (nargs >= 3) {
            radius = lua_tointeger(L, 3);
        } else {
            radius = 4;
        }
        if (nargs >= 4) {
            uint8_t col = lua_tointeger(L, 4);
            err = circ(r, x0, y0, radius, col);
        } else {
            err = circ_default(r, x0, y0, radius);
        }
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

int rose_lua_graphics_circ(lua_State* L) {
    return circ_lua_common(L, &rose_api_graphics_circ, &rose_api_graphics_circ_default);
}

int rose_lua_graphics_circfill(lua_State* L) {
    return circ_lua_common(L, &rose_api_graphics_circfill, &rose_api_graphics_circfill_default);
}

int rose_lua_graphics_cls(lua_State* L) {
    Rose_RuntimeBase* r = rose_lua_base_get_runtime_base(L);
    if (r == NULL) { return 0; }
    int err = rose_api_graphics_cls(r);
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