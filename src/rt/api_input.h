#pragma once

#include "enums.h"

rose_api_error rose_api_input_mouse(rose_rt* r, int16_t* x, int16_t* y);

rose_api_error rose_api_input_btn(rose_rt* r, uint8_t idx, bool* res);

rose_api_error rose_api_input_btnp(rose_rt* r, uint8_t idx, bool* res);

rose_api_error rose_api_input_wheel(rose_rt* r, int16_t* x, int16_t* y);

rose_api_error rose_api_input_wheel_inverted(rose_rt* r, bool* res);

rose_api_error rose_api_input_key(rose_rt* r, rose_keycode keycode, bool* res);

rose_api_error rose_api_input_keyp(rose_rt* r, rose_keycode keycode, bool* res);
