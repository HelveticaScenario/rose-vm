#include "rt/js/js_memory.h"

void rose_js_memory_poke(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HandleScope scope(args.GetIsolate());
    rose_runtime_base* r = static_cast<rose_runtime_base*>(Local<External>::Cast(args.Data())->Value());
    uint32_t idx = (uint32_t) args[0]->Uint32Value();
    uint8_t val = (uint8_t) args[1]->Int32Value();
    int err = rose_api_memory_poke(r, idx, val);
    switch (err) {
        case ROSE_API_ERR_OUT_OF_BOUNDS: {
            Isolate* isolate = Isolate::GetCurrent();
            isolate->ThrowException(String::NewFromUtf8(isolate, "Bad Memory Access"));
            break;
        }
        case ROSE_API_ERR_NONE:
        default: {
            break;
        }
    }
}

void rose_js_memory_peek(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HandleScope scope(args.GetIsolate());
    rose_runtime_base* r = static_cast<rose_runtime_base*>(Local<External>::Cast(args.Data())->Value());
    if (args.Length() >= 1) {
        uint32_t idx = (uint32_t) args[0]->Uint32Value();
        uint8_t res;
        int err = rose_api_memory_peek(r, idx, &res);
        switch (err) {
            case ROSE_API_ERR_OUT_OF_BOUNDS: {
                Isolate* isolate = Isolate::GetCurrent();
                isolate->ThrowException(String::NewFromUtf8(isolate, "Bad Memory Access"));
                break;
            }
            case ROSE_API_ERR_NONE:
            default: {
                args.GetReturnValue().Set(res);
                break;
            }
        }
    } else {
        args.GetReturnValue().Set(0);
    }
}

void rose_js_memory_memcpy(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HandleScope scope(args.GetIsolate());
    rose_runtime_base* r = static_cast<rose_runtime_base*>(Local<External>::Cast(args.Data())->Value());
    if (args.Length() >= 3) {
        uint32_t dest_addr = (uint32_t) args[0]->Uint32Value();
        uint32_t source_addr = (uint32_t) args[1]->Uint32Value();
        uint32_t len = (uint32_t) args[2]->Uint32Value();
        int err = rose_api_memory_memcpy(r, dest_addr, source_addr, len);
        switch (err) {
            case ROSE_API_ERR_OUT_OF_BOUNDS: {
                Isolate* isolate = Isolate::GetCurrent();
                isolate->ThrowException(String::NewFromUtf8(isolate, "Bad Memory Access"));
                break;
            }
            case ROSE_API_ERR_NONE:
            default: {
                break;
            }
        }
    }
}

void rose_js_memory_memset(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HandleScope scope(args.GetIsolate());
    rose_runtime_base* r = static_cast<rose_runtime_base*>(Local<External>::Cast(args.Data())->Value());
    if (args.Length() >= 3) {
        uint32_t dest_addr = (uint32_t) args[0]->Uint32Value();
        uint8_t val = (uint8_t) args[1]->Uint32Value();
        uint32_t len = (uint32_t) args[2]->Uint32Value();
        int err = rose_api_memory_memset(r, dest_addr, val, len);
        switch (err) {
            case ROSE_API_ERR_OUT_OF_BOUNDS: {
                Isolate* isolate = Isolate::GetCurrent();
                isolate->ThrowException(String::NewFromUtf8(isolate, "Bad Memory Access"));
                break;
            }
            case ROSE_API_ERR_NONE:
            default: {
                break;
            }
        }
    }
}