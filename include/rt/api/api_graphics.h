#ifndef ROSE_API_GRAPHICS_H
#define ROSE_API_GRAPHICS_H

#include "config.h"
#include "rt/api/api_memory.h"
#include "rt/rt_base.h"

Rose_RuntimeApiError rose_api_graphics_pset(Rose_RuntimeBase* r, int16_t x, int16_t y, uint8_t val);

Rose_RuntimeApiError rose_api_graphics_pset_default(Rose_RuntimeBase* r, int16_t x, int16_t y);

Rose_RuntimeApiError rose_api_graphics_pget(Rose_RuntimeBase* r, int16_t x, int16_t y, uint8_t* res);

Rose_RuntimeApiError rose_api_graphics_palset(Rose_RuntimeBase* r, uint8_t idx, Rose_Color c);

Rose_RuntimeApiError rose_api_graphics_palget(Rose_RuntimeBase* r, uint8_t idx, Rose_Color* res);

Rose_RuntimeApiError rose_api_graphics_line(Rose_RuntimeBase* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t c);

Rose_RuntimeApiError rose_api_graphics_line_default(Rose_RuntimeBase* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1);

Rose_RuntimeApiError rose_api_graphics_rect(Rose_RuntimeBase* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t c);

Rose_RuntimeApiError rose_api_graphics_rect_default(Rose_RuntimeBase* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1);

Rose_RuntimeApiError rose_api_graphics_rectfill(Rose_RuntimeBase* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t c);

Rose_RuntimeApiError rose_api_graphics_rectfill_default(Rose_RuntimeBase* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1);

Rose_RuntimeApiError rose_api_graphics_circ(Rose_RuntimeBase* r, int16_t x0, int16_t y0, uint16_t radius, uint8_t c);

Rose_RuntimeApiError rose_api_graphics_circ_default(Rose_RuntimeBase* r, int16_t x0, int16_t y0, uint16_t radius);

Rose_RuntimeApiError rose_api_graphics_circfill(Rose_RuntimeBase* r, int16_t x0, int16_t y0, uint16_t radius, uint8_t c);

Rose_RuntimeApiError rose_api_graphics_circfill_default(Rose_RuntimeBase* r, int16_t x0, int16_t y0, uint16_t radius);

Rose_RuntimeApiError rose_api_graphics_cls(Rose_RuntimeBase* r);

#endif