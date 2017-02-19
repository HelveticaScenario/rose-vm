#include "rose.h"

void rose_js_sys_extcmd(const v8::FunctionCallbackInfo<v8::Value>& args) {
    if (args.Length() < 1) return;
    HandleScope scope(args.GetIsolate());
    rose_rt* r = static_cast<rose_rt*>(Local<External>::Cast(args.Data())->Value());
    if (r->extcmd_cb != NULL) {
        r->extcmd_cb(args);
    }
}
