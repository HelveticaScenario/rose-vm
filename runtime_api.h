#ifndef RUNTIME_API_H
#define RUNTIME_API_H

#include "config.h"
#include "runtime_base.h"

typedef enum {
    ROSE_API_ERR_NONE,
    ROSE_API_ERR_OUT_OF_BOUNDS
} Rose_RuntimeApiError;

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Rose_Color;

Rose_RuntimeApiError rose_runtime_base_poke(Rose_RuntimeBase* r, uint_fast32_t idx, uint8_t val);

Rose_RuntimeApiError rose_runtime_base_peek(Rose_RuntimeBase* r, uint_fast32_t idx, uint8_t* res);

Rose_RuntimeApiError rose_runtime_base_pset(Rose_RuntimeBase* r, int32_t x, int32_t y, uint8_t val);

Rose_RuntimeApiError rose_runtime_base_pset_default(Rose_RuntimeBase* r, int32_t x, int32_t y);

Rose_RuntimeApiError rose_runtime_base_pget(Rose_RuntimeBase* r, int32_t x, int32_t y, uint8_t* res);

Rose_RuntimeApiError rose_runtime_base_palset(Rose_RuntimeBase* r, uint8_t idx, Rose_Color c);

Rose_RuntimeApiError rose_runtime_base_palget(Rose_RuntimeBase* r, uint8_t idx, Rose_Color* res);

Rose_RuntimeApiError rose_runtime_base_rect(Rose_RuntimeBase* r, uint_fast16_t x0, uint_fast16_t y0, uint_fast16_t x1, uint_fast16_t y1, uint8_t col);

Rose_RuntimeApiError rose_runtime_base_rect_default(Rose_RuntimeBase* r, uint_fast16_t x0, uint_fast16_t y0, uint_fast16_t x1, uint_fast16_t y1);

Rose_RuntimeApiError rose_runtime_base_rectfill(Rose_RuntimeBase* r, uint_fast16_t x0, uint_fast16_t y0, uint_fast16_t x1, uint_fast16_t y1, uint8_t col);

Rose_RuntimeApiError rose_runtime_base_rectfill_default(Rose_RuntimeBase* r, uint_fast16_t x0, uint_fast16_t y0, uint_fast16_t x1, uint_fast16_t y1);

Rose_RuntimeApiError rose_runtime_base_cls(Rose_RuntimeBase* r);

void rose_runtime_register_api(lua_State* L, Rose_RuntimeBase* r);

void rose_runtime_register_lua_function(lua_State* L, Rose_RuntimeBase* r, lua_CFunction fun, const char* name);

int rose_runtime_base_lua_poke(lua_State* L);

int rose_runtime_base_lua_peek(lua_State* L);

int rose_runtime_base_lua_pset(lua_State* L);

int rose_runtime_base_lua_pget(lua_State* L);

int rose_runtime_base_lua_palset(lua_State* L);

int rose_runtime_base_lua_palget(lua_State* L);

int rose_runtime_base_lua_rect(lua_State* L);

int rose_runtime_base_lua_rectfill(lua_State* L);

int rose_runtime_base_lua_cls(lua_State* L);

#endif