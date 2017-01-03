#ifndef ROSEBUD_JS_BASE_H
#define ROSEBUD_JS_BASE_H

#include "config.h"
#include "rt/js/js_common.h"
#include "rt/js/js_graphics.h"
#include "rt/js/js_input.h"
#include "rt/js/js_memory.h"


rose_js_base* rose_js_base_create(rose_runtime_base* r);

void rose_js_base_free(rose_js_base* js);


#endif //ROSEBUD_JS_BASE_H
