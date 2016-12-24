#ifndef ROSE_API_INPUT_H
#define ROSE_API_INPUT_H

#include "config.h"
#include "rt/api/api_memory.h"
#include "rt/rt_base.h"

rose_runtime_api_error rose_api_input_mouse(rose_runtime_base* r, int16_t* x, int16_t* y);

rose_runtime_api_error rose_api_input_btn(rose_runtime_base* r, uint8_t idx, bool* res);

rose_runtime_api_error rose_api_input_wheel(rose_runtime_base* r, int16_t* x, int16_t* y);

rose_runtime_api_error rose_api_input_wheel_inverted(rose_runtime_base* r, bool* res);

rose_runtime_api_error rose_api_input_key(rose_runtime_base* r, rose_keycode keycode, bool* res);

#endif