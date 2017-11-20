#include "js.h"

void RosebudJS::Mouse(const Nan::FunctionCallbackInfo<v8::Value>& info){
    RosebudJS* obj = Nan::ObjectWrap::Unwrap<RosebudJS>(info.This());
    rose_vm* vm = obj->vm;
    int16_t x = 0;
    int16_t y = 0;
    rose_api_error err = vm->mouse(&x, &y);
    switch (err) {
        case ROSE_API_ERR_OUT_OF_BOUNDS: {
            Nan::ThrowRangeError("Bad Memory Access");
            break;
        }
        case ROSE_API_ERR_NONE: {
            v8::Local<v8::Array> array = Nan::New<v8::Array>(2);

            // Return an empty result if there was an error creating the array.
            if (array.IsEmpty()) {
                break;
            }

            // Fill out the values
            array->Set(0, Nan::New<v8::Integer>(x));
            array->Set(1, Nan::New<v8::Integer>(y));
            info.GetReturnValue().Set(array);
            break;
        }

    }
}

void RosebudJS::Btn(const Nan::FunctionCallbackInfo<v8::Value>& info){
    if (info.Length() < 1) return;
    RosebudJS* obj = Nan::ObjectWrap::Unwrap<RosebudJS>(info.This());
    rose_vm* vm = obj->vm;
    uint32_t i;
    for (i = 0; i < info.Length(); i++) {
        uint8_t idx = (uint8_t) info[i]->Int32Value();
        if (idx > 31) {
            idx = 31;
        }
        bool res;
        int err = vm->btn(idx, &res);
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
    }
}

void RosebudJS::Btnp(const Nan::FunctionCallbackInfo<v8::Value>& info){
    if (info.Length() < 1) return;
    RosebudJS* obj = Nan::ObjectWrap::Unwrap<RosebudJS>(info.This());
    rose_vm* vm = obj->vm;
    uint32_t i;
    for (i = 0; i < info.Length(); i++) {
        uint8_t idx = (uint8_t) info[i]->Int32Value();
        if (idx > 31) {
            idx = 31;
        }
        bool res;
        int err = vm->btnp(idx, &res);
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
    }
}

void RosebudJS::Wheel(const Nan::FunctionCallbackInfo<v8::Value>& info){
    RosebudJS* obj = Nan::ObjectWrap::Unwrap<RosebudJS>(info.This());
    rose_vm* vm = obj->vm;
    bool to_invert;
    vm->wheel_inverted(&to_invert);
    int ninfo = info.Length();
    if (ninfo > 0) {
        to_invert = info[0]->BooleanValue();
    }
    int16_t x = 0;
    int16_t y = 0;
    int err = vm->wheel(&x, &y);
    if (to_invert) {
        x *= -1;
        y *= -1;
    }
    switch (err) {
        case ROSE_API_ERR_OUT_OF_BOUNDS: {
            Nan::ThrowRangeError("Bad Memory Access");
            break;
        }
        case ROSE_API_ERR_NONE:
        default: {
            v8::Local<v8::Array> array = Nan::New<v8::Array>(2);

            // Return an empty result if there was an error creating the array.
            if (array.IsEmpty()) {
                break;
            }

            // Fill out the values
            array->Set(0, Nan::New<v8::Integer>(x));
            array->Set(1, Nan::New<v8::Integer>(y));
            info.GetReturnValue().Set(array);
            break;
        }
    }
}

void RosebudJS::Key(const Nan::FunctionCallbackInfo<v8::Value>& info){
    RosebudJS* obj = Nan::ObjectWrap::Unwrap<RosebudJS>(info.This());
    rose_vm* vm = obj->vm;
    uint32_t i;
    for (i = 0; i < info.Length(); i++) {
        uint8_t idx = (uint8_t) info[i]->Int32Value();
        if (idx >= ROSE_KEYCODE_MAX_VALUE) {
            idx = ROSE_KEYCODE_MAX_VALUE - 1;
        }
        bool res;
        int err = vm->key((rose_keycode) idx, &res);
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
    }
}

void RosebudJS::Keyp(const Nan::FunctionCallbackInfo<v8::Value>& info){
    RosebudJS* obj = Nan::ObjectWrap::Unwrap<RosebudJS>(info.This());
    rose_vm* vm = obj->vm;
    uint32_t i;
    for (i = 0; i < info.Length(); i++) {
        uint8_t idx = (uint8_t) info[i]->Int32Value();
        if (idx >= ROSE_KEYCODE_MAX_VALUE) {
            idx = ROSE_KEYCODE_MAX_VALUE - 1;
        }
        bool res;
        int err = vm->keyp((rose_keycode) idx, &res);
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
    }
}
