#ifndef ROSE_RT_BASE_H
#define ROSE_RT_BASE_H

#include "config.h"
#include "rt/fs/fs.h"
#include "rt/js/js_common.h"
#include "rt/js/js.h"

const char* rose_keycode_to_string(rose_keycode key);

rose_memory_range* rose_memory_range_create(uint8_t begin[], uint32_t len);

rose_memory_iterator rose_memory_iterator_begin(uint8_t m[]);

rose_memory_iterator rose_memory_iterator_end(uint8_t m[], uint32_t len);

rose_memory_iterator rose_memory_iterator_next(rose_memory_iterator i);

rose_rt* rose_rt_create(rose_fs* fs);

bool rose_rt_clear(rose_rt* r);

bool rose_rt_load_run_main(rose_rt* r);

void rose_rt_free(rose_rt* r);

void rose_rt_save_input_frame(rose_rt* r);

void rose_rt_update_mousestate(rose_rt* r, const rose_mousestate* mousestate);

void rose_rt_update_keystate(rose_rt* r, rose_keycode keycode, bool pressed);

void rose_rt_reset_input(rose_rt* r, rose_mousestate* mousestate);

rose_rt_error rose_rt_init(rose_rt* r);

rose_rt_error rose_rt_update(rose_rt* r);

rose_rt_error rose_rt_draw(rose_rt* r);

rose_rt_error rose_rt_onmouse(rose_rt* r, int16_t x, int16_t y);

rose_rt_error rose_rt_onwheel(rose_rt* r, int16_t x, int16_t y, bool inverted);

rose_rt_error rose_rt_onbtn(rose_rt* r, uint8_t code, bool pressed);

rose_rt_error rose_rt_onkey(rose_rt* r, rose_keycode keycode, bool pressed, bool repeat);

rose_rt_error rose_rt_ontouch(rose_rt* r);

void rose_set_bit(uint8_t* trans, uint8_t addr, bool val);

bool rose_get_bit(uint8_t* trans, uint8_t addr);

void rose_init(const char* base_path);

void rose_deinit();

#endif