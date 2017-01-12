#pragma once

#include "enums.h"

struct rose_color{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

const char* rose_keycode_to_string(rose_keycode key);
