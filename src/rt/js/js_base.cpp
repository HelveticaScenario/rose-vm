#include "rt/js/js_base.h"


void js_print(const v8::FunctionCallbackInfo<v8::Value>& args) {
    if (args.Length() < 1) return;
    HandleScope scope(args.GetIsolate());
    rose_runtime_base* r = static_cast<rose_runtime_base*>(Local<External>::Cast(args.Data())->Value());
    for (int i = 0; i < args.Length(); i++){
        const v8::String::Utf8Value value(args[i]);
        printf("%s", *value);
        if (i < (args.Length()-1))
            printf(", ");
    }
    printf("\n");
}

rose_js_base* rose_js_base_create(rose_runtime_base* r) {
    rose_js_base* js = new rose_js_base();

    js->fs = r->fs;

    js->create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    Isolate* isolate = Isolate::New(js->create_params);
    js->isolate = isolate;

    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);

    Local<External> r_ptr = External::New(isolate, r);

    Local<ObjectTemplate> global = ObjectTemplate::New(isolate);
    v8::Local<v8::FunctionTemplate> print = FunctionTemplate::New(isolate, js_print, r_ptr);
    global->Set(String::NewFromUtf8(isolate, "print", NewStringType::kNormal).ToLocalChecked(), print);

    v8::Local<v8::FunctionTemplate> pset = FunctionTemplate::New(isolate, rose_js_graphics_pset, r_ptr);
    global->Set(String::NewFromUtf8(isolate, "pset", NewStringType::kNormal).ToLocalChecked(), pset);

    v8::Local<v8::FunctionTemplate> pget = FunctionTemplate::New(isolate, rose_js_graphics_pget, r_ptr);
    global->Set(String::NewFromUtf8(isolate, "pget", NewStringType::kNormal).ToLocalChecked(), pget);

    v8::Local<v8::FunctionTemplate> palset = FunctionTemplate::New(isolate, rose_js_graphics_palset, r_ptr);
    global->Set(String::NewFromUtf8(isolate, "palset", NewStringType::kNormal).ToLocalChecked(), palset);

    v8::Local<v8::FunctionTemplate> palget = FunctionTemplate::New(isolate, rose_js_graphics_palget, r_ptr);
    global->Set(String::NewFromUtf8(isolate, "palget", NewStringType::kNormal).ToLocalChecked(), palget);

    v8::Local<v8::FunctionTemplate> line = FunctionTemplate::New(isolate, rose_js_graphics_line, r_ptr);
    global->Set(String::NewFromUtf8(isolate, "line", NewStringType::kNormal).ToLocalChecked(), line);

    v8::Local<v8::FunctionTemplate> rect = FunctionTemplate::New(isolate, rose_js_graphics_rect, r_ptr);
    global->Set(String::NewFromUtf8(isolate, "rect", NewStringType::kNormal).ToLocalChecked(), rect);

    v8::Local<v8::FunctionTemplate> rectfill = FunctionTemplate::New(isolate, rose_js_graphics_rectfill, r_ptr);
    global->Set(String::NewFromUtf8(isolate, "rectfill", NewStringType::kNormal).ToLocalChecked(), rectfill);

    v8::Local<v8::FunctionTemplate> circ = FunctionTemplate::New(isolate, rose_js_graphics_circ, r_ptr);
    global->Set(String::NewFromUtf8(isolate, "circ", NewStringType::kNormal).ToLocalChecked(), circ);

    v8::Local<v8::FunctionTemplate> circfill = FunctionTemplate::New(isolate, rose_js_graphics_circfill, r_ptr);
    global->Set(String::NewFromUtf8(isolate, "circfill", NewStringType::kNormal).ToLocalChecked(), circfill);

    v8::Local<v8::FunctionTemplate> tri = FunctionTemplate::New(isolate, rose_js_graphics_tri, r_ptr);
    global->Set(String::NewFromUtf8(isolate, "tri", NewStringType::kNormal).ToLocalChecked(), tri);

    v8::Local<v8::FunctionTemplate> trifill = FunctionTemplate::New(isolate, rose_js_graphics_trifill, r_ptr);
    global->Set(String::NewFromUtf8(isolate, "trifill", NewStringType::kNormal).ToLocalChecked(), trifill);

    v8::Local<v8::FunctionTemplate> cls = FunctionTemplate::New(isolate, rose_js_graphics_cls, r_ptr);
    global->Set(String::NewFromUtf8(isolate, "cls", NewStringType::kNormal).ToLocalChecked(), cls);



    v8::Local<v8::FunctionTemplate> poke = FunctionTemplate::New(isolate, rose_js_memory_poke, r_ptr);
    global->Set(String::NewFromUtf8(isolate, "poke", NewStringType::kNormal).ToLocalChecked(), poke);

    v8::Local<v8::FunctionTemplate> peek = FunctionTemplate::New(isolate, rose_js_memory_peek, r_ptr);
    global->Set(String::NewFromUtf8(isolate, "peek", NewStringType::kNormal).ToLocalChecked(), peek);

    v8::Local<v8::FunctionTemplate> memcpy = FunctionTemplate::New(isolate, rose_js_memory_memcpy, r_ptr);
    global->Set(String::NewFromUtf8(isolate, "memcpy", NewStringType::kNormal).ToLocalChecked(), memcpy);

    v8::Local<v8::FunctionTemplate> memset = FunctionTemplate::New(isolate, rose_js_memory_memset, r_ptr);
    global->Set(String::NewFromUtf8(isolate, "memset", NewStringType::kNormal).ToLocalChecked(), memset);



    v8::Local<v8::FunctionTemplate> mouse = FunctionTemplate::New(isolate, rose_js_input_mouse, r_ptr);
    global->Set(String::NewFromUtf8(isolate, "mouse", NewStringType::kNormal).ToLocalChecked(), mouse);

    v8::Local<v8::FunctionTemplate> btn = FunctionTemplate::New(isolate, rose_js_input_btn, r_ptr);
    global->Set(String::NewFromUtf8(isolate, "btn", NewStringType::kNormal).ToLocalChecked(), btn);

    v8::Local<v8::FunctionTemplate> key = FunctionTemplate::New(isolate, rose_js_input_key, r_ptr);

    for (int keycode = ROSE_KEYCODE_A; keycode < ROSE_KEYCODE_UNKNOWN; ++keycode) {
        const char * name = rose_keycode_to_string(
                (rose_keycode) keycode);
        key->Set(String::NewFromUtf8(isolate, name, NewStringType::kNormal).ToLocalChecked(), Uint32::New(isolate, keycode));
    }

    global->Set(String::NewFromUtf8(isolate, "key", NewStringType::kNormal).ToLocalChecked(), key);

    v8::Local<v8::FunctionTemplate> wheel = FunctionTemplate::New(isolate, rose_js_input_wheel, r_ptr);
    global->Set(String::NewFromUtf8(isolate, "wheel", NewStringType::kNormal).ToLocalChecked(), wheel);

    js->global_template.Reset(isolate, global);

    return js;
}

void rose_js_base_free(rose_js_base* js) {
    js->context.Reset();
    js->global_template.Reset();
    js->isolate->Dispose();
    delete js->create_params.array_buffer_allocator;
    delete js;
}

