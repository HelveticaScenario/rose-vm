#ifndef ROSEBUD_JS_INPUT_H
#define ROSEBUD_JS_INPUT_H

#include "config.h"
#include "rt/api/api_input.h"

void rose_js_input_mouse(const v8::FunctionCallbackInfo<v8::Value>& args);

void rose_js_input_btn(const v8::FunctionCallbackInfo<v8::Value>& args);

void rose_js_input_wheel(const v8::FunctionCallbackInfo<v8::Value>& args);

void rose_js_input_key(const v8::FunctionCallbackInfo<v8::Value>& args);

#endif //ROSEBUD_JS_INPUT_H
