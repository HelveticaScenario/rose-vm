#ifndef ROSEBUD_JS_MEMORY_H
#define ROSEBUD_JS_MEMORY_H

#include "config.h"
#include "rt/api/api_memory.h"

void rose_js_memory_poke(const v8::FunctionCallbackInfo<v8::Value>& args);

void rose_js_memory_peek(const v8::FunctionCallbackInfo<v8::Value>& args);

void rose_js_memory_memcpy(const v8::FunctionCallbackInfo<v8::Value>& args);

void rose_js_memory_memset(const v8::FunctionCallbackInfo<v8::Value>& args);

#endif //ROSEBUD_JS_MEMORY_H
