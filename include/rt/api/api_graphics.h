#ifndef ROSE_API_GRAPHICS_H
#define ROSE_API_GRAPHICS_H

#include "config.h"
#include "rt/api/api_memory.h"
#include "rt/rt_base.h"

rose_runtime_api_error rose_api_graphics_pset(rose_runtime_base* r, int16_t x, int16_t y, uint8_t val);

rose_runtime_api_error rose_api_graphics_pset_default(rose_runtime_base* r, int16_t x, int16_t y);

rose_runtime_api_error rose_api_graphics_pget(rose_runtime_base* r, int16_t x, int16_t y, uint8_t* res);

rose_runtime_api_error rose_api_graphics_palset(rose_runtime_base* r, uint8_t idx, rose_color c);

rose_runtime_api_error rose_api_graphics_palget(rose_runtime_base* r, uint8_t idx, rose_color* res);

rose_runtime_api_error rose_api_graphics_line(rose_runtime_base* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t c);

rose_runtime_api_error rose_api_graphics_line_default(rose_runtime_base* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1);

rose_runtime_api_error rose_api_graphics_rect(rose_runtime_base* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t c);

rose_runtime_api_error rose_api_graphics_rect_default(rose_runtime_base* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1);

rose_runtime_api_error rose_api_graphics_rectfill(rose_runtime_base* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t c);

rose_runtime_api_error rose_api_graphics_rectfill_default(rose_runtime_base* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1);

rose_runtime_api_error rose_api_graphics_circ(rose_runtime_base* r, int16_t x0, int16_t y0, uint16_t radius, uint8_t c);

rose_runtime_api_error rose_api_graphics_circ_default(rose_runtime_base* r, int16_t x0, int16_t y0, uint16_t radius);

rose_runtime_api_error rose_api_graphics_circfill(rose_runtime_base* r, int16_t x0, int16_t y0, uint16_t radius, uint8_t c);

rose_runtime_api_error rose_api_graphics_circfill_default(rose_runtime_base* r, int16_t x0, int16_t y0, uint16_t radius);

rose_runtime_api_error rose_api_graphics_cls(rose_runtime_base* r);

#endif