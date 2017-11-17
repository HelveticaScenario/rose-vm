#include "js.h"

void RosebudJS::UpdateMousePos(
    const Nan::FunctionCallbackInfo<v8::Value>& info) {
  RosebudJS* obj = Nan::ObjectWrap::Unwrap<RosebudJS>(info.This());
  rose_vm* vm = obj->vm;
  if (info.Length() >= 2) {
    int16_t x = (int16_t)info[0]->Int32Value();
    int16_t y = (int16_t)info[1]->Int32Value();
    vm->update_mouse_pos(x, y);
  }
}

void RosebudJS::UpdateBtnState(
    const Nan::FunctionCallbackInfo<v8::Value>& info) {
  RosebudJS* obj = Nan::ObjectWrap::Unwrap<RosebudJS>(info.This());
  rose_vm* vm = obj->vm;
  if (info.Length() >= 2) {
    uint8_t btn = (uint8_t)info[0]->Uint32Value();
    bool pressed = (bool)info[1]->BooleanValue();
    vm->update_btn_state(btn, pressed);
  }
}

void RosebudJS::UpdateWheelState(
    const Nan::FunctionCallbackInfo<v8::Value>& info) {
  RosebudJS* obj = Nan::ObjectWrap::Unwrap<RosebudJS>(info.This());
  rose_vm* vm = obj->vm;
  if (info.Length() >= 3) {
    int16_t delta_x = (int16_t)info[0]->Int32Value();
    int16_t delta_y = (int16_t)info[1]->Int32Value();
    bool inverted = (bool)info[2]->BooleanValue();
    vm->update_wheel_state(delta_x, delta_y, inverted);
  }
}

void RosebudJS::UpdateKeyState(
    const Nan::FunctionCallbackInfo<v8::Value>& info) {
  RosebudJS* obj = Nan::ObjectWrap::Unwrap<RosebudJS>(info.This());
  rose_vm* vm = obj->vm;
  if (info.Length() >= 3) {
    rose_keycode keycode = (rose_keycode)info[0]->Uint32Value();
    bool pressed = (bool)info[1]->BooleanValue();
    vm->update_keystate(keycode, pressed);
  }
}

void RosebudJS::ResetSystemMemory(
    const Nan::FunctionCallbackInfo<v8::Value>& info) {
  RosebudJS* obj = Nan::ObjectWrap::Unwrap<RosebudJS>(info.This());
  obj->vm->clear();
}