#ifndef ROSE_API_INPUT_H
#define ROSE_API_INPUT_H

#include "config.h"
#include "rt/api/api_memory.h"
#include "rt/rt_base.h"

Rose_RuntimeApiError rose_api_input_mouse(Rose_RuntimeBase* r, int16_t* x, int16_t* y);

Rose_RuntimeApiError rose_api_input_btn(Rose_RuntimeBase* r, uint8_t idx, bool* res);

Rose_RuntimeApiError rose_api_input_wheel(Rose_RuntimeBase* r, int16_t* x, int16_t* y);

Rose_RuntimeApiError rose_api_input_wheel_inverted(Rose_RuntimeBase* r, bool* res);

Rose_RuntimeApiError rose_api_input_key(Rose_RuntimeBase* r, uint16_t* keycode);

#endif