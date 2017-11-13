#include "js.h"

int16_t clamp(int32_t v) {
    if (v > ROSE_MAX_INT16) {
        return ROSE_MAX_INT16;
    } else if (v < ROSE_MIN_INT16) {
        return ROSE_MIN_INT16;
    } else {
        return  (int16_t) v;
    }

}

void RosebudJS::Pset(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    if (info.Length() < 2) return;
    RosebudJS* obj = Nan::ObjectWrap::Unwrap<RosebudJS>(info.This());
    rose_vm* vm = obj->vm;

    rose_api_error err = ROSE_API_ERR_NONE;
    if (info.Length() == 2) {

        int16_t x = clamp(info[0]->Int32Value());
        int16_t y = clamp(info[1]->Int32Value());

        err = vm->pset_default(x, y);
    } else {
        int16_t x = clamp(info[0]->Int32Value());
        int16_t y = clamp(info[1]->Int32Value());
        uint8_t c = (uint8_t) info[2]->Uint32Value();

        err = vm->pset(x, y, c);
    }
    switch (err) {
        case ROSE_API_ERR_OUT_OF_BOUNDS: {
            Nan::ThrowRangeError("Bad Memory Access");;
            break;
        }
        case ROSE_API_ERR_NONE:
            break;
    }
}

void RosebudJS::Pget(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    if (info.Length() < 2) return;
    RosebudJS* obj = Nan::ObjectWrap::Unwrap<RosebudJS>(info.This());
    rose_vm* vm = obj->vm;

    rose_api_error err = ROSE_API_ERR_NONE;
    uint8_t res = 0;
    int16_t x = clamp(info[0]->Int32Value());
    int16_t y = clamp(info[1]->Int32Value());

    err = vm->pget(x, y, &res);
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

void RosebudJS::Palset(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    if (info.Length() < 4) return;
    RosebudJS* obj = Nan::ObjectWrap::Unwrap<RosebudJS>(info.This());
    rose_vm* vm = obj->vm;

    rose_api_error err = ROSE_API_ERR_NONE;
    uint8_t idx = (uint8_t) info[0]->Int32Value();
    uint8_t red = (uint8_t) info[1]->Int32Value();
    uint8_t green = (uint8_t) info[2]->Int32Value();
    uint8_t blue = (uint8_t) info[3]->Int32Value();
    rose_color c = {.r = red, .g = green, .b = blue};
    err = vm->palset(idx, c);
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

void RosebudJS::Palget(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    if (info.Length() < 1) return;
    RosebudJS* obj = Nan::ObjectWrap::Unwrap<RosebudJS>(info.This());
    rose_vm* vm = obj->vm;

    rose_api_error err = ROSE_API_ERR_NONE;
    uint8_t idx = (uint8_t) info[0]->Int32Value();
    rose_color res;
    err = vm->palget(idx, &res);
    switch (err) {
        case ROSE_API_ERR_OUT_OF_BOUNDS: {
            Nan::ThrowRangeError("Bad Memory Access");
            break;
        }
        case ROSE_API_ERR_NONE:
        default: {
            v8::Local<v8::Array> array = Nan::New<v8::Array>(3);

            // Return an empty result if there was an error creating the array.
            if (array.IsEmpty()) {
                break;
            }

            // Fill out the values
            array->Set(0, Nan::New<v8::Integer>(res.r));
            array->Set(1, Nan::New<v8::Integer>(res.g));
            array->Set(2, Nan::New<v8::Integer>(res.b));
            info.GetReturnValue().Set(array);
            break;
        }
    }
}

void RosebudJS::Line(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    if (info.Length() < 4) return;
    RosebudJS* obj = Nan::ObjectWrap::Unwrap<RosebudJS>(info.This());
    rose_vm* vm = obj->vm;

    rose_api_error err = ROSE_API_ERR_NONE;
    int16_t x0 = (int16_t) info[0]->Int32Value();
    int16_t y0 = (int16_t) info[1]->Int32Value();
    int16_t x1 = (int16_t) info[2]->Int32Value();
    int16_t y1 = (int16_t) info[3]->Int32Value();
    if (info.Length() >= 5) {
        uint8_t col = (uint8_t) info[4]->Int32Value();
        err = vm->line(x0, y0, x1, y1, col);
    } else {
        err = vm->line_default(x0, y0, x1, y1);
    }

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

void RosebudJS::RectCommon(const Nan::FunctionCallbackInfo<v8::Value>& info, bool fill) {
    if (info.Length() < 4) return;
    RosebudJS* obj = Nan::ObjectWrap::Unwrap<RosebudJS>(info.This());
    rose_vm* vm = obj->vm;

    rose_api_error err = ROSE_API_ERR_NONE;
    if (info.Length() >= 4) {
        int16_t x0 = (int16_t) info[0]->Int32Value();
        int16_t y0 = (int16_t) info[1]->Int32Value();
        int16_t x1 = (int16_t) info[2]->Int32Value();
        int16_t y1 = (int16_t) info[3]->Int32Value();
        if (info.Length() >= 5) {
            uint8_t col = (uint8_t) info[4]->Int32Value();
            if (fill) {
                err = vm->rectfill(x0, y0, x1, y1, col);
            } else {
                err = vm->rect(x0, y0, x1, y1, col);
            }
        } else {
            if (fill) {
                err = vm->rectfill_default(x0, y0, x1, y1);
            } else {
                err = vm->rect_default(x0, y0, x1, y1);
            }
        }
    } else if (info.Length() == 3) {
        int16_t x0 = (int16_t) info[0]->Int32Value();
        int16_t y0 = (int16_t) info[1]->Int32Value();
        int16_t x1 = (int16_t) info[2]->Int32Value();
        if (fill) {
            err = vm->rectfill_default(x0, y0, x1, 0);
        } else {
            err = vm->rect_default(x0, y0, x1, 0);
        }
    } else if (info.Length() >= 1) {
        int16_t x1 = (int16_t) info[0]->Int32Value();
        int16_t y1 = 0;
        if (info.Length() == 2) {
            y1 = (int16_t) info[1]->Int32Value();
        }
        if (fill) {
            err = vm->rectfill_default(0, 0, x1, y1);
        } else {
            err = vm->rect_default(0, 0, x1, y1);
        }
    }

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

void RosebudJS::Rect(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    return RosebudJS::RectCommon(info, false);
}


void RosebudJS::Rectfill(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    return RosebudJS::RectCommon(info, true);
}

void RosebudJS::CircCommon(const Nan::FunctionCallbackInfo<v8::Value>& info, bool fill) {
    if (info.Length() < 4) return;
    RosebudJS* obj = Nan::ObjectWrap::Unwrap<RosebudJS>(info.This());
    rose_vm* vm = obj->vm;

    rose_api_error err = ROSE_API_ERR_NONE;
    if (info.Length() >= 2) {
        int16_t x0 = (int16_t) info[0]->Int32Value();
        int16_t y0 = (int16_t) info[1]->Int32Value();
        uint16_t radius;
        if (info.Length() >= 3) {
            radius = (uint16_t) info[2]->Int32Value();
        } else {
            radius = 4;
        }
        if (info.Length() >= 4) {
            uint8_t col = (uint8_t) info[3]->Int32Value();
            if (fill) {
                err = vm->circfill(x0, y0, radius, col);
            } else {
                err = vm->circ(x0, y0, radius, col);
            }
        } else {
            if (fill) {
                err = vm->circfill_default(x0, y0, radius);
            } else {
                err = vm->circ_default(x0, y0, radius);
            }
        }
    }

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

void RosebudJS::Circ(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    return RosebudJS::CircCommon(info, false);
}

void RosebudJS::Circfill(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    return RosebudJS::CircCommon(info, true);
}

void RosebudJS::Tri(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    if (info.Length() < 6) return;
    RosebudJS* obj = Nan::ObjectWrap::Unwrap<RosebudJS>(info.This());
    rose_vm* vm = obj->vm;

    rose_api_error err = ROSE_API_ERR_NONE;
    int16_t x0 = (int16_t) info[0]->Int32Value();
    int16_t y0 = (int16_t) info[1]->Int32Value();
    int16_t x1 = (int16_t) info[2]->Int32Value();
    int16_t y1 = (int16_t) info[3]->Int32Value();
    int16_t x2 = (int16_t) info[4]->Int32Value();
    int16_t y2 = (int16_t) info[5]->Int32Value();
    if (info.Length() >= 7) {
        uint8_t col = (uint8_t) info[6]->Int32Value();
        err = vm->tri(x0, y0, x1, y1, x2, y2, col);
    } else {
        err = vm->tri_default(x0, y0, x1, y1, x2, y2);
    }

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

void RosebudJS::Trifill(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    if (info.Length() < 6) return;
    RosebudJS* obj = Nan::ObjectWrap::Unwrap<RosebudJS>(info.This());
    rose_vm* vm = obj->vm;

    rose_api_error err = ROSE_API_ERR_NONE;
    int16_t x0 = (int16_t) info[0]->Int32Value();
    int16_t y0 = (int16_t) info[1]->Int32Value();
    int16_t x1 = (int16_t) info[2]->Int32Value();
    int16_t y1 = (int16_t) info[3]->Int32Value();
    int16_t x2 = (int16_t) info[4]->Int32Value();
    int16_t y2 = (int16_t) info[5]->Int32Value();
    if (info.Length() >= 7) {
        uint8_t col = (uint8_t) info[6]->Int32Value();
        err = vm->trifill(x0, y0, x1, y1, x2, y2, col);
    } else {
        err = vm->trifill_default(x0, y0, x1, y1, x2, y2);
    }

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

void RosebudJS::Cls(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    RosebudJS* obj = Nan::ObjectWrap::Unwrap<RosebudJS>(info.This());
    rose_vm* vm = obj->vm;

    rose_api_error err = ROSE_API_ERR_NONE;

    err = vm->cls();
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

void RosebudJS::Print(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    if (info.Length() < 1) return;
    RosebudJS* obj = Nan::ObjectWrap::Unwrap<RosebudJS>(info.This());
    rose_vm* vm = obj->vm;

    Nan::Utf8String strArg(info[0]->ToString());
    string str = ToCString(strArg);
    uint8_t color;
    int16_t x, y;
     if (info.Length() <= 2) {
        color = *(vm->pen_color_addr);
        int16_t* ptr = (int16_t*) vm->print_cursor.begin;
        x = ptr[0];
        y = ptr[1];
    } else if (info.Length() == 3) {
        color = *(vm->pen_color_addr);
        x = clamp(info[1]->Int32Value());
        y = clamp(info[2]->Int32Value());
    } else {
        color = (uint8_t) info[3]->Uint32Value();
        x = clamp(info[1]->Int32Value());
        y = clamp(info[2]->Int32Value());
    }
    rose_api_error err = vm->print(str, x, y, color);
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

void RosebudJS::Color(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    if (info.Length() < 1) return;
    RosebudJS* obj = Nan::ObjectWrap::Unwrap<RosebudJS>(info.This());
    rose_vm* vm = obj->vm;

    uint8_t color = (uint8_t) info[0]->Uint32Value();
    rose_api_error err = vm->color(color);
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


