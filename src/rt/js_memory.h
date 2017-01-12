#pragma once

void rose_js_memory_poke(const v8::FunctionCallbackInfo<v8::Value>& args);

void rose_js_memory_peek(const v8::FunctionCallbackInfo<v8::Value>& args);

void rose_js_memory_memcpy(const v8::FunctionCallbackInfo<v8::Value>& args);

void rose_js_memory_memset(const v8::FunctionCallbackInfo<v8::Value>& args);

void rose_js_memory_cstore(const v8::FunctionCallbackInfo<v8::Value>& args);

void rose_js_memory_reload(const v8::FunctionCallbackInfo<v8::Value>& args);

void rose_js_memory__writestr(const v8::FunctionCallbackInfo<v8::Value>& args);

void rose_js_memory__readstr(const v8::FunctionCallbackInfo<v8::Value>& args);

void rose_js_memory__savefile(const v8::FunctionCallbackInfo<v8::Value>& args);

void rose_js_memory__mkfile(const v8::FunctionCallbackInfo<v8::Value>& args);

void rose_js_memory__rmfile(const v8::FunctionCallbackInfo<v8::Value>& args);

