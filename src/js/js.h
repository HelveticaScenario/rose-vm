#pragma once

#include <nan.h>
#include "../vm/rose.h"
#include "js_common.h"

class RosebudJS : public Nan::ObjectWrap {
public:
    static NAN_MODULE_INIT(Init);

private:
    explicit RosebudJS();
    ~RosebudJS();

    static NAN_METHOD(New);

    // Memory
    static NAN_METHOD(Poke);
    static NAN_METHOD(Peek);
    static NAN_METHOD(Memcpy);
    static NAN_METHOD(Memset);

    // Input
    static NAN_METHOD(Mouse);
    static NAN_METHOD(Btn);
    static NAN_METHOD(Btnp);
    static NAN_METHOD(Wheel);
    static NAN_METHOD(Key);
    static NAN_METHOD(Keyp);

    // Graphics
    static NAN_METHOD(Pset);
    static NAN_METHOD(Pget);
    static NAN_METHOD(Palset);
    static NAN_METHOD(Palget);
    static NAN_METHOD(Line);
    static NAN_METHOD(Rect);
    static NAN_METHOD(Rectfill);
    static NAN_METHOD(Circ);
    static NAN_METHOD(Circfill);
    static NAN_METHOD(Tri);
    static NAN_METHOD(Trifill);
    static NAN_METHOD(Cls);
    static NAN_METHOD(Print);
    static NAN_METHOD(Color);

    static NAN_METHOD(LoadCartData);

    static void RectCommon(const Nan::FunctionCallbackInfo<v8::Value>& info, bool fill);
    static void CircCommon(const Nan::FunctionCallbackInfo<v8::Value>& info, bool fill);


    static Nan::Persistent<v8::Function> constructor;
    rose_vm* vm;
};

