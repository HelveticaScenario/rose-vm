#ifndef ROSE_RT_BASE_H
#define ROSE_RT_BASE_H

#include "config.h"
#include "rt/fs/fs_base.h"
#include "rt/js/js_common.h"
#include "rt/js/js_base.h"

const char* rose_keycode_to_string(rose_keycode key);

rose_memory_range* rose_memory_range_create(uint8_t begin[], uint32_t len);

rose_memory_iterator rose_memory_iterator_begin(uint8_t m[]);

rose_memory_iterator rose_memory_iterator_end(uint8_t m[], uint32_t len);

rose_memory_iterator rose_memory_iterator_next(rose_memory_iterator i);

rose_runtime_base* rose_runtime_base_create(rose_fs* fs);

bool rose_runtime_base_clear(rose_runtime_base* r);

bool rose_runtime_base_load_run_main(rose_runtime_base* r);

void rose_runtime_base_free(rose_runtime_base* r);

void rose_runtime_base_save_input_frame(rose_runtime_base* r);

void rose_runtime_base_update_mousestate(rose_runtime_base* r, const rose_mousestate* mousestate);

void rose_runtime_base_update_keystate(rose_runtime_base* r, rose_keycode keycode, bool pressed);

rose_runtime_base_error rose_runtime_base_init(rose_runtime_base* r);

rose_runtime_base_error rose_runtime_base_update(rose_runtime_base* r);

rose_runtime_base_error rose_runtime_base_draw(rose_runtime_base* r);

rose_runtime_base_error rose_runtime_base_onmouse(rose_runtime_base* r, int16_t x, int16_t y);

rose_runtime_base_error rose_runtime_base_onwheel(rose_runtime_base* r, int16_t x, int16_t y, bool inverted);

rose_runtime_base_error rose_runtime_base_onbtn(rose_runtime_base* r, uint8_t code, bool pressed);

rose_runtime_base_error rose_runtime_base_onkey(rose_runtime_base* r, rose_keycode keycode, bool pressed, bool repeat);

rose_runtime_base_error rose_runtime_base_ontouch(rose_runtime_base* r);

void rose_set_bit(uint8_t* trans, uint8_t addr, bool val);

bool rose_get_bit(uint8_t* trans, uint8_t addr);

void rose_init(const char* base_path);

void rose_deinit();

#endif