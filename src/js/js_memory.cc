#include "js.h"

void RosebudJS::Poke(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    RosebudJS* obj = Nan::ObjectWrap::Unwrap<RosebudJS>(info.This());
    rose_vm* vm = obj->vm;
    uint32_t idx = (uint32_t) info[0]->Uint32Value();
    uint8_t val = (uint8_t) info[1]->Int32Value();
    int err = vm->poke(idx, val);
    switch (err) {
        case ROSE_API_ERR_OUT_OF_BOUNDS: {
            Nan::ThrowRangeError("Bad Memory Access");
            break;
        }
        case ROSE_API_ERR_NONE:
        default: {
            break;
        }
    }
}

void RosebudJS::Peek(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    RosebudJS* obj = Nan::ObjectWrap::Unwrap<RosebudJS>(info.This());
    rose_vm* vm = obj->vm;
    if (info.Length() >= 1) {
        uint32_t idx = (uint32_t) info[0]->Uint32Value();
        uint8_t res;
        int err = vm->peek(idx, &res);
        switch (err) {
            case ROSE_API_ERR_OUT_OF_BOUNDS: {
                Nan::ThrowRangeError("Bad Memory Access");
                break;
            }
            case ROSE_API_ERR_NONE:
            default: {
                info.GetReturnValue().Set(res);
                break;
            }
        }
    } else {
        info.GetReturnValue().Set(0);
    }
}

void RosebudJS::Memcpy(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    RosebudJS* obj = Nan::ObjectWrap::Unwrap<RosebudJS>(info.This());
    rose_vm* vm = obj->vm;
    if (info.Length() >= 3) {
        uint32_t dest_addr = (uint32_t) info[0]->Uint32Value();
        uint32_t source_addr = (uint32_t) info[1]->Uint32Value();
        uint32_t len = (uint32_t) info[2]->Uint32Value();
        int err = vm->rose_memcpy(dest_addr, source_addr, len);
        switch (err) {
            case ROSE_API_ERR_OUT_OF_BOUNDS: {
                Nan::ThrowRangeError("Bad Memory Access");
                break;
            }
            case ROSE_API_ERR_NONE:
            default: {
                break;
            }
        }
    }
}

void RosebudJS::Memset(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    RosebudJS* obj = Nan::ObjectWrap::Unwrap<RosebudJS>(info.This());
    rose_vm* vm = obj->vm;
    if (info.Length() >= 3) {
        uint32_t dest_addr = (uint32_t) info[0]->Uint32Value();
        uint8_t val = (uint8_t) info[1]->Uint32Value();
        uint32_t len = (uint32_t) info[2]->Uint32Value();
        int err = vm->rose_memset(dest_addr, val, len);
        switch (err) {
            case ROSE_API_ERR_OUT_OF_BOUNDS: {
                Nan::ThrowRangeError("Bad Memory Access");
                break;
            }
            case ROSE_API_ERR_NONE:
            default: {
                break;
            }
        }
    }
}

// void RosebudJS::Cstore(const Nan::FunctionCallbackInfo<v8::Value>& info) {
//     printf("cstore probably works, but it is untested.\n");
//     HandleScope scope(info.GetIsolate());
//     rose_vm* r = static_cast<rose_vm*>(Local<External>::Cast(info.Data())->Value());
//     if (info.Length() >= 3) {
//         uint32_t dest_addr = (uint32_t) info[0]->Uint32Value();
//         uint32_t source_addr = (uint32_t) info[1]->Uint32Value();
//         uint32_t len = (uint32_t) info[2]->Uint32Value();
//         int err = vm->cstore(dest_addr, source_addr, len);
//         switch (err) {
//             case ROSE_API_ERR_OUT_OF_BOUNDS: {
//                 Isolate* isolate = Isolate::GetCurrent();
//                 isolate->ThrowException(String::NewFromUtf8(isolate, "Bad Memory Access"));
//                 break;
//             }
//             case ROSE_API_ERR_NONE:
//             default: {
//                 break;
//             }
//         }
//     }
// }


// void RosebudJS::Reload(const Nan::FunctionCallbackInfo<v8::Value>& info) {
//     printf("reload probably works, but it is untested.\n");
//     HandleScope scope(info.GetIsolate());
//     rose_vm* r = static_cast<rose_vm*>(Local<External>::Cast(info.Data())->Value());
//     if (info.Length() >= 3) {
//         uint32_t dest_addr = (uint32_t) info[0]->Uint32Value();
//         uint32_t source_addr = (uint32_t) info[1]->Uint32Value();
//         uint32_t len = (uint32_t) info[2]->Uint32Value();
//         int err = vm->reload(dest_addr, source_addr, len);
//         switch (err) {
//             case ROSE_API_ERR_OUT_OF_BOUNDS: {
//                 Isolate* isolate = Isolate::GetCurrent();
//                 isolate->ThrowException(String::NewFromUtf8(isolate, "Bad Memory Access"));
//                 break;
//             }
//             case ROSE_API_ERR_NONE:
//             default: {
//                 break;
//             }
//         }
//     }
// }
