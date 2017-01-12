#include "js_graphics.h"

int16_t clamp(int32_t v) {
    if (v > ROSE_MAX_INT16) {
        return ROSE_MAX_INT16;
    } else if (v < ROSE_MIN_INT16) {
        return ROSE_MIN_INT16;
    } else {
        return  (int16_t) v;
    }

}

void rose_js_graphics_pset(const v8::FunctionCallbackInfo<v8::Value>& args) {
    if (args.Length() < 2) return;
    HandleScope scope(args.GetIsolate());
    rose_rt* r = static_cast<rose_rt*>(Local<External>::Cast(args.Data())->Value());

    rose_api_error err = ROSE_API_ERR_NONE;
    if (args.Length() == 2) {

        int16_t x = clamp(args[0]->Int32Value());
        int16_t y = clamp(args[1]->Int32Value());

        err = rose_api_graphics_pset_default(r, x, y);
    } else {
        int16_t x = clamp(args[0]->Int32Value());
        int16_t y = clamp(args[1]->Int32Value());
        uint8_t c = (uint8_t) args[2]->Uint32Value();

        err = rose_api_graphics_pset(r, x, y, c);
    }
    switch (err) {
        case ROSE_API_ERR_OUT_OF_BOUNDS: {
            Isolate* isolate = Isolate::GetCurrent();
            isolate->ThrowException(String::NewFromUtf8(isolate, "Bad Memory Access"));;
            break;
        }
        case ROSE_API_ERR_NONE:
            break;
    }
}

void rose_js_graphics_pget(const v8::FunctionCallbackInfo<v8::Value>& args) {
    if (args.Length() < 2) return;
    HandleScope scope(args.GetIsolate());
    rose_rt* r = static_cast<rose_rt*>(Local<External>::Cast(args.Data())->Value());

    rose_api_error err = ROSE_API_ERR_NONE;
    uint8_t res = 0;
    int16_t x = clamp(args[0]->Int32Value());
    int16_t y = clamp(args[1]->Int32Value());

    err = rose_api_graphics_pget(r, x, y, &res);
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

void rose_js_graphics_palset(const v8::FunctionCallbackInfo<v8::Value>& args) {
    if (args.Length() < 4) return;
    HandleScope scope(args.GetIsolate());
    rose_rt* r = static_cast<rose_rt*>(Local<External>::Cast(args.Data())->Value());

    rose_api_error err = ROSE_API_ERR_NONE;
    uint8_t idx = (uint8_t) args[0]->Int32Value();
    uint8_t red = (uint8_t) args[1]->Int32Value();
    uint8_t green = (uint8_t) args[2]->Int32Value();
    uint8_t blue = (uint8_t) args[3]->Int32Value();
    rose_color c = {.r = red, .g = green, .b = blue};
    err = rose_api_graphics_palset(r, idx, c);
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

void rose_js_graphics_palget(const v8::FunctionCallbackInfo<v8::Value>& args) {
    if (args.Length() < 1) return;
    HandleScope scope(args.GetIsolate());
    rose_rt* r = static_cast<rose_rt*>(Local<External>::Cast(args.Data())->Value());

    rose_api_error err = ROSE_API_ERR_NONE;
    uint8_t idx = (uint8_t) args[0]->Int32Value();
    rose_color res;
    err = rose_api_graphics_palget(r, idx, &res);
    switch (err) {
        case ROSE_API_ERR_OUT_OF_BOUNDS: {
            Isolate* isolate = Isolate::GetCurrent();
            isolate->ThrowException(String::NewFromUtf8(isolate, "Bad Memory Access"));
            break;
        }
        case ROSE_API_ERR_NONE:
        default: {
            auto isolate = Isolate::GetCurrent();
            Local<Array> array = Array::New(isolate, 3);

            // Return an empty result if there was an error creating the array.
            if (array.IsEmpty()) {
                break;
            }

            // Fill out the values
            array->Set(0, Integer::New(isolate, res.r));
            array->Set(1, Integer::New(isolate, res.g));
            array->Set(2, Integer::New(isolate, res.b));
            args.GetReturnValue().Set(array);
            break;
        }
    }
}

void rose_js_graphics_line(const v8::FunctionCallbackInfo<v8::Value>& args) {
    if (args.Length() < 4) return;
    HandleScope scope(args.GetIsolate());
    rose_rt* r = static_cast<rose_rt*>(Local<External>::Cast(args.Data())->Value());

    rose_api_error err = ROSE_API_ERR_NONE;
    int16_t x0 = (int16_t) args[0]->Int32Value();
    int16_t y0 = (int16_t) args[1]->Int32Value();
    int16_t x1 = (int16_t) args[2]->Int32Value();
    int16_t y1 = (int16_t) args[3]->Int32Value();
    if (args.Length() >= 5) {
        uint8_t col = (uint8_t) args[4]->Int32Value();
        err = rose_api_graphics_line(r, x0, y0, x1, y1, col);
    } else {
        err = rose_api_graphics_line_default(r, x0, y0, x1, y1);
    }

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

typedef rose_api_error (*rect_fp)(rose_rt* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t col);
typedef rose_api_error (*rect_default_fp)(rose_rt* r, int16_t x0, int16_t y0, int16_t x1, int16_t y1);

void rect_js_common(const v8::FunctionCallbackInfo<v8::Value>& args, rect_fp rect, rect_default_fp rect_default) {
    if (args.Length() < 4) return;
    HandleScope scope(args.GetIsolate());
    rose_rt* r = static_cast<rose_rt*>(Local<External>::Cast(args.Data())->Value());

    rose_api_error err = ROSE_API_ERR_NONE;
    if (args.Length() >= 4) {
        int16_t x0 = (int16_t) args[0]->Int32Value();
        int16_t y0 = (int16_t) args[1]->Int32Value();
        int16_t x1 = (int16_t) args[2]->Int32Value();
        int16_t y1 = (int16_t) args[3]->Int32Value();
        if (args.Length() >= 5) {
            uint8_t col = (uint8_t) args[4]->Int32Value();
            err = rect(r, x0, y0, x1, y1, col);
        } else {
            err = rect_default(r, x0, y0, x1, y1);
        }
    } else if (args.Length() == 3) {
        int16_t x0 = (int16_t) args[0]->Int32Value();
        int16_t y0 = (int16_t) args[1]->Int32Value();
        int16_t x1 = (int16_t) args[2]->Int32Value();
        err = rect_default(r, x0, y0, x1, 0);
    } else if (args.Length() >= 1) {
        int16_t x1 = (int16_t) args[0]->Int32Value();
        int16_t y1 = 0;
        if (args.Length() == 2) {
            y1 = (int16_t) args[1]->Int32Value();
        }
        err = rect_default(r, 0, 0, x1, y1);
    }

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

void rose_js_graphics_rect(const v8::FunctionCallbackInfo<v8::Value>& args) {
    return rect_js_common(args, &rose_api_graphics_rect, &rose_api_graphics_rect_default);
}


void rose_js_graphics_rectfill(const v8::FunctionCallbackInfo<v8::Value>& args) {
    return rect_js_common(args, &rose_api_graphics_rectfill, &rose_api_graphics_rectfill_default);
}

typedef rose_api_error (*circ_fp)(rose_rt* r, int16_t x0, int16_t y0, uint16_t radius, uint8_t col);
typedef rose_api_error (*circ_default_fp)(rose_rt* r, int16_t x0, int16_t y0, uint16_t radius);

void circ_js_common(const v8::FunctionCallbackInfo<v8::Value>& args, circ_fp circ, circ_default_fp circ_default) {
    if (args.Length() < 4) return;
    HandleScope scope(args.GetIsolate());
    rose_rt* r = static_cast<rose_rt*>(Local<External>::Cast(args.Data())->Value());

    rose_api_error err = ROSE_API_ERR_NONE;
    if (args.Length() >= 2) {
        int16_t x0 = (int16_t) args[0]->Int32Value();
        int16_t y0 = (int16_t) args[1]->Int32Value();
        uint16_t radius;
        if (args.Length() >= 3) {
            radius = (uint16_t) args[2]->Int32Value();
        } else {
            radius = 4;
        }
        if (args.Length() >= 4) {
            uint8_t col = (uint8_t) args[3]->Int32Value();
            err = circ(r, x0, y0, radius, col);
        } else {
            err = circ_default(r, x0, y0, radius);
        }
    }

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

void rose_js_graphics_circ(const v8::FunctionCallbackInfo<v8::Value>& args) {
    return circ_js_common(args, &rose_api_graphics_circ, &rose_api_graphics_circ_default);
}

void rose_js_graphics_circfill(const v8::FunctionCallbackInfo<v8::Value>& args) {
    return circ_js_common(args, &rose_api_graphics_circfill, &rose_api_graphics_circfill_default);
}

void rose_js_graphics_tri(const v8::FunctionCallbackInfo<v8::Value>& args) {
    if (args.Length() < 6) return;
    HandleScope scope(args.GetIsolate());
    rose_rt* r = static_cast<rose_rt*>(Local<External>::Cast(args.Data())->Value());

    rose_api_error err = ROSE_API_ERR_NONE;
    int16_t x0 = (int16_t) args[0]->Int32Value();
    int16_t y0 = (int16_t) args[1]->Int32Value();
    int16_t x1 = (int16_t) args[2]->Int32Value();
    int16_t y1 = (int16_t) args[3]->Int32Value();
    int16_t x2 = (int16_t) args[4]->Int32Value();
    int16_t y2 = (int16_t) args[5]->Int32Value();
    if (args.Length() >= 7) {
        uint8_t col = (uint8_t) args[6]->Int32Value();
        err = rose_api_graphics_tri(r, x0, y0, x1, y1, x2, y2, col);
    } else {
        err = rose_api_graphics_tri_default(r, x0, y0, x1, y1, x2, y2);
    }

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

void rose_js_graphics_trifill(const v8::FunctionCallbackInfo<v8::Value>& args) {
    if (args.Length() < 6) return;
    HandleScope scope(args.GetIsolate());
    rose_rt* r = static_cast<rose_rt*>(Local<External>::Cast(args.Data())->Value());

    rose_api_error err = ROSE_API_ERR_NONE;
    int16_t x0 = (int16_t) args[0]->Int32Value();
    int16_t y0 = (int16_t) args[1]->Int32Value();
    int16_t x1 = (int16_t) args[2]->Int32Value();
    int16_t y1 = (int16_t) args[3]->Int32Value();
    int16_t x2 = (int16_t) args[4]->Int32Value();
    int16_t y2 = (int16_t) args[5]->Int32Value();
    if (args.Length() >= 7) {
        uint8_t col = (uint8_t) args[6]->Int32Value();
        err = rose_api_graphics_trifill(r, x0, y0, x1, y1, x2, y2, col);
    } else {
        err = rose_api_graphics_trifill_default(r, x0, y0, x1, y1, x2, y2);
    }

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

void rose_js_graphics_cls(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HandleScope scope(args.GetIsolate());
    rose_rt* r = static_cast<rose_rt*>(Local<External>::Cast(args.Data())->Value());

    rose_api_error err = ROSE_API_ERR_NONE;

    err = rose_api_graphics_cls(r);
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


