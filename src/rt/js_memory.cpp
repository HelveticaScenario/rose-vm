#include "rose.h"

void rose_js_memory_poke(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HandleScope scope(args.GetIsolate());
    rose_rt* r = static_cast<rose_rt*>(Local<External>::Cast(args.Data())->Value());
    uint32_t idx = (uint32_t) args[0]->Uint32Value();
    uint8_t val = (uint8_t) args[1]->Int32Value();
    int err = r->poke(idx, val);
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
    rose_rt* r = static_cast<rose_rt*>(Local<External>::Cast(args.Data())->Value());
    if (args.Length() >= 1) {
        uint32_t idx = (uint32_t) args[0]->Uint32Value();
        uint8_t res;
        int err = r->peek(idx, &res);
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
    rose_rt* r = static_cast<rose_rt*>(Local<External>::Cast(args.Data())->Value());
    if (args.Length() >= 3) {
        uint32_t dest_addr = (uint32_t) args[0]->Uint32Value();
        uint32_t source_addr = (uint32_t) args[1]->Uint32Value();
        uint32_t len = (uint32_t) args[2]->Uint32Value();
        int err = r->rt_memcpy(dest_addr, source_addr, len);
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
    rose_rt* r = static_cast<rose_rt*>(Local<External>::Cast(args.Data())->Value());
    if (args.Length() >= 3) {
        uint32_t dest_addr = (uint32_t) args[0]->Uint32Value();
        uint8_t val = (uint8_t) args[1]->Uint32Value();
        uint32_t len = (uint32_t) args[2]->Uint32Value();
        int err = r->rt_memset(dest_addr, val, len);
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

void rose_js_memory_cstore(const v8::FunctionCallbackInfo<v8::Value>& args) {
    printf("cstore probably works, but it is untested.\n");
    HandleScope scope(args.GetIsolate());
    rose_rt* r = static_cast<rose_rt*>(Local<External>::Cast(args.Data())->Value());
    if (args.Length() >= 3) {
        uint32_t dest_addr = (uint32_t) args[0]->Uint32Value();
        uint32_t source_addr = (uint32_t) args[1]->Uint32Value();
        uint32_t len = (uint32_t) args[2]->Uint32Value();
        int err = r->cstore(dest_addr, source_addr, len);
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


void rose_js_memory_reload(const v8::FunctionCallbackInfo<v8::Value>& args) {
    printf("reload probably works, but it is untested.\n");
    HandleScope scope(args.GetIsolate());
    rose_rt* r = static_cast<rose_rt*>(Local<External>::Cast(args.Data())->Value());
    if (args.Length() >= 3) {
        uint32_t dest_addr = (uint32_t) args[0]->Uint32Value();
        uint32_t source_addr = (uint32_t) args[1]->Uint32Value();
        uint32_t len = (uint32_t) args[2]->Uint32Value();
        int err = r->reload(dest_addr, source_addr, len);
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
