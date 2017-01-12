#ifndef ROSEBUD_JS_BASE_H
#define ROSEBUD_JS_BASE_H

#include "config.h"
#include "rt/js/js_common.h"
#include "rt/js/js_graphics.h"
#include "rt/js/js_input.h"
#include "rt/js/js_memory.h"


rose_js* rose_js_create(rose_rt* r);

void rose_js_free(rose_js* js);


#endif //ROSEBUD_JS_BASE_H
