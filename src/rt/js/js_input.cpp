#include "rt/js/js_input.h"

void rose_js_input_mouse(const v8::FunctionCallbackInfo<v8::Value>& args){
    HandleScope scope(args.GetIsolate());
    rose_rt* r = static_cast<rose_rt*>(Local<External>::Cast(args.Data())->Value());
    int16_t x = 0;
    int16_t y = 0;
    rose_api_error err = rose_api_input_mouse(r, &x, &y);
    switch (err) {
        case ROSE_API_ERR_OUT_OF_BOUNDS: {
            Isolate* isolate = Isolate::GetCurrent();
            isolate->ThrowException(String::NewFromUtf8(isolate, "Bad Memory Access"));;
            break;
        }
        case ROSE_API_ERR_NONE: {
            auto isolate = Isolate::GetCurrent();
            Local<Array> array = Array::New(isolate, 2);

            // Return an empty result if there was an error creating the array.
            if (array.IsEmpty()) {
                break;
            }

            // Fill out the values
            array->Set(0, Integer::New(isolate, x));
            array->Set(1, Integer::New(isolate, y));
            args.GetReturnValue().Set(array);
            break;
        }

    }
}

void rose_js_input_btn(const v8::FunctionCallbackInfo<v8::Value>& args){
    if (args.Length() < 1) return;
    HandleScope scope(args.GetIsolate());
    rose_rt* r = static_cast<rose_rt*>(Local<External>::Cast(args.Data())->Value());
    uint32_t i;
    for (i = 0; i < args.Length(); i++) {
        uint8_t idx = (uint8_t) args[i]->Int32Value();
        if (idx > 31) {
            idx = 31;
        }
        bool res;
        int err = rose_api_input_btn(r, idx, &res);
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
    }
}

void rose_js_input_btnp(const v8::FunctionCallbackInfo<v8::Value>& args){
    if (args.Length() < 1) return;
    HandleScope scope(args.GetIsolate());
    rose_rt* r = static_cast<rose_rt*>(Local<External>::Cast(args.Data())->Value());
    uint32_t i;
    for (i = 0; i < args.Length(); i++) {
        uint8_t idx = (uint8_t) args[i]->Int32Value();
        if (idx > 31) {
            idx = 31;
        }
        bool res;
        int err = rose_api_input_btnp(r, idx, &res);
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
    }
}

void rose_js_input_wheel(const v8::FunctionCallbackInfo<v8::Value>& args){
    HandleScope scope(args.GetIsolate());
    rose_rt* r = static_cast<rose_rt*>(Local<External>::Cast(args.Data())->Value());
    bool to_invert;
    rose_api_input_wheel_inverted(r, &to_invert);
    int nargs = args.Length();
    if (nargs > 0) {
        to_invert = args[0]->BooleanValue();
    }
    int16_t x = 0;
    int16_t y = 0;
    int err = rose_api_input_wheel(r, &x, &y);
    if (to_invert) {
        x *= -1;
        y *= -1;
    }
    switch (err) {
        case ROSE_API_ERR_OUT_OF_BOUNDS: {
            Isolate* isolate = Isolate::GetCurrent();
            isolate->ThrowException(String::NewFromUtf8(isolate, "Bad Memory Access"));
            break;
        }
        case ROSE_API_ERR_NONE:
        default: {
            auto isolate = Isolate::GetCurrent();
            Local<Array> array = Array::New(isolate, 2);

            // Return an empty result if there was an error creating the array.
            if (array.IsEmpty()) {
                break;
            }

            // Fill out the values
            array->Set(0, Integer::New(isolate, x));
            array->Set(1, Integer::New(isolate, y));
            args.GetReturnValue().Set(array);
            break;
        }
    }
}

void rose_js_input_key(const v8::FunctionCallbackInfo<v8::Value>& args){
    HandleScope scope(args.GetIsolate());
    rose_rt* r = static_cast<rose_rt*>(Local<External>::Cast(args.Data())->Value());
    uint32_t i;
    for (i = 0; i < args.Length(); i++) {
        uint8_t idx = (uint8_t) args[i]->Int32Value();
        if (idx >= ROSE_KEYCODE_UNKNOWN) {
            idx = ROSE_KEYCODE_UNKNOWN - 1;
        }
        bool res;
        int err = rose_api_input_key(r, (rose_keycode) idx, &res);
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
    }
}

void rose_js_input_keyp(const v8::FunctionCallbackInfo<v8::Value>& args){
    HandleScope scope(args.GetIsolate());
    rose_rt* r = static_cast<rose_rt*>(Local<External>::Cast(args.Data())->Value());
    uint32_t i;
    for (i = 0; i < args.Length(); i++) {
        uint8_t idx = (uint8_t) args[i]->Int32Value();
        if (idx >= ROSE_KEYCODE_UNKNOWN) {
            idx = ROSE_KEYCODE_UNKNOWN - 1;
        }
        bool res;
        int err = rose_api_input_keyp(r, (rose_keycode) idx, &res);
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
    }
}