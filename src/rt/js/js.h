#ifndef ROSEBUD_JS_BASE_H
#define ROSEBUD_JS_BASE_H

#include "../../config.h"
#include "js_common.h"
#include "js_graphics.h"
#include "js_input.h"
#include "js_memory.h"


rose_js* rose_js_create(rose_rt* r);

void rose_js_free(rose_js* js);


#endif //ROSEBUD_JS_BASE_H
