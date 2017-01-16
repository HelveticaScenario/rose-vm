#pragma once

#include "enums.h"

rose_api_error rose_api_graphics_pset(rose_rt* r, int16_t x, int16_t y, uint8_t val);

rose_api_error rose_api_graphics_pset_default(rose_rt* r, int16_t x, int16_t y);

rose_api_error rose_api_graphics_pget(rose_rt* r, int16_t x, int16_t y, uint8_t* res);

rose_api_error rose_api_graphics_palset(rose_rt* r, uint8_t idx, rose_color c);

rose_api_error rose_api_graphics_palget(rose_rt* r, uint8_t idx, rose_color* res);

rose_api_error rose_api_graphics_line(rose_rt* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t c);

rose_api_error rose_api_graphics_line_default(rose_rt* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1);

rose_api_error rose_api_graphics_rect(rose_rt* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t c);

rose_api_error rose_api_graphics_rect_default(rose_rt* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1);

rose_api_error rose_api_graphics_rectfill(rose_rt* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t c);

rose_api_error rose_api_graphics_rectfill_default(rose_rt* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1);

rose_api_error rose_api_graphics_circ(rose_rt* r, int16_t x0, int16_t y0, uint16_t radius, uint8_t c);

rose_api_error rose_api_graphics_circ_default(rose_rt* r, int16_t x0, int16_t y0, uint16_t radius);

rose_api_error rose_api_graphics_circfill(rose_rt* r, int16_t x0, int16_t y0, uint16_t radius, uint8_t c);

rose_api_error rose_api_graphics_circfill_default(rose_rt* r, int16_t x0, int16_t y0, uint16_t radius);

rose_api_error rose_api_graphics_tri(rose_rt* r, int16_t v1_x, int16_t v1_y, int16_t v2_x, int16_t v2_y, int16_t v3_x, int16_t v3_y, uint8_t c);

rose_api_error rose_api_graphics_tri_default(rose_rt* r, int16_t v0_x, int16_t v0_y, int16_t v1_x, int16_t v1_y, int16_t v2_x, int16_t v2_y);

rose_api_error rose_api_graphics_trifill(rose_rt* r, int16_t v1_x, int16_t v1_y, int16_t v2_x, int16_t v2_y, int16_t v3_x, int16_t v3_y, uint8_t c);

rose_api_error rose_api_graphics_trifill_default(rose_rt* r, int16_t v0_x, int16_t v0_y, int16_t v1_x, int16_t v1_y, int16_t v2_x, int16_t v2_y);

rose_api_error rose_api_graphics_cls(rose_rt* r);

rose_api_error rose_api_graphics_get_spritesheet_meta(rose_rt* r, uint32_t* addr, uint16_t* sheet_width, uint16_t* sheet_height, uint8_t* sprite_width_mult, uint8_t* sprite_height_mult);

rose_api_error rose_api_graphics_set_spritesheet_meta(rose_rt* r, uint32_t addr, uint16_t sheet_width, uint16_t sheet_height, uint8_t sprite_width_mult, uint8_t sprite_height_mult);

rose_api_error rose_api_graphics_spr(rose_rt* r, uint32_t n, int16_t x, int16_t y, uint8_t w, uint8_t h, bool flip_x, bool flip_y);
