#include "js.h"

using namespace Nan;
using namespace v8;

// void js_require(const v8::FunctionCallbackInfo<v8::Value>& args) {
//     Isolate* isolate = args.GetIsolate();
//     if (args.Length() < 1) {
//         isolate->ThrowException(String::NewFromUtf8(isolate, "missing path"));
//         return;
//     }
//     rose_vm* r = static_cast<rose_vm*>(Local<External>::Cast(args.Data())->Value());

//     HandleScope scope(args.GetIsolate());
//     v8::Isolate::Scope isolate_scope(isolate);
//     v8::TryCatch try_catch(isolate);
//     Local<Context> context = r->js->context.Get(isolate);
//     if (context.IsEmpty()) {
//         fprintf(stderr, "something went horribly wrong and I'm so, so sorry.\n");
//     }
//     v8::Context::Scope context_scope(context);

//     if (!args[0]->IsString()) {
//         isolate->ThrowException(String::NewFromUtf8(isolate, "path must be a string."));
//         return;
//     }
//     v8::String::Utf8Value argStr(args[0]->ToString());
//     string path(*argStr);
//     // ltrim
//     path.erase(path.begin(), std::find_if(path.begin(), path.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
//     // rtrim
//     path.erase(std::find_if(path.rbegin(), path.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), path.end());

//     if (path.length() == 0) {
//         isolate->ThrowException(String::NewFromUtf8(isolate, "missing path"));
//         return;
//     }

//     std::vector<std::string> elems;
//     split(path, '/', elems);
//     rose_file* file = elems[0].compare("") == 0 ? r->self_cart : r->js->include_path.back()->parent;
//     for (int i = 0; i < elems.size(); i++) {
//         if (elems[i].compare("") == 0) {
//             continue;
//         } if (elems[i].compare(".") == 0) {
//             continue;
//         } else if (elems[i].compare("..") == 0) {
//             file = file->parent;
//         } else {
//             auto new_file = rose_fs_fetch_child(file, elems[i].c_str());
//             if (new_file == NULL) {
//                 string with_suffix = elems[i] + ".js";
//                 new_file = rose_fs_fetch_child(file, with_suffix.c_str());
//             }
//             file = new_file;
//         }
//         if (file == NULL){
//             isolate->ThrowException(String::NewFromUtf8(isolate, "invalid path"));
//             return;
//         }
//     }

//     if (file == NULL){
//         isolate->ThrowException(String::NewFromUtf8(isolate, "invalid path"));
//         return;
//     }
//     auto exports_str = v8::String::NewFromUtf8(isolate, "exports", v8::NewStringType::kNormal).ToLocalChecked();

//     auto file_full_path_str = rose_construct_path(file);
//     auto file_full_path = v8::String::NewFromUtf8(isolate, file_full_path_str.c_str(), v8::NewStringType::kNormal).ToLocalChecked();
//     auto module_cache = r->js->module_cache.Get(isolate);
//     auto maybe_cached = module_cache->Get(context, file_full_path);
//     Local<Value> cached;
//     if (maybe_cached.ToLocal(&cached) && cached->IsObject()) {
//         args.GetReturnValue().Set(Handle<Object>::Cast(cached)->Get(exports_str));
//         return;
//     }

//     if (!file->in_mem) {
//         r->fs->read_file(file);
//     }

//     if (file->buffer[file->buffer_len-1] != '\0') {
//         file->buffer_len++;
//         file->buffer = (uint8_t*) realloc(file->buffer, file->buffer_len);
//         file->buffer[file->buffer_len-1] = '\0';
//         file->last_modification = time(NULL);
//     }

//     string file_buffer_string((const char*) file->buffer);
//     const char* header = "(function (exports, module, __filename) { ";
//     const char* footer = "\n})";
//     string wrapped_string = header + file_buffer_string + footer;

//     r->js->include_path.push_back(file);
//     v8::Local<v8::String> file_name = v8::String::NewFromUtf8(isolate, file->name.c_str(), v8::NewStringType::kNormal).ToLocalChecked();
//     v8::Local<v8::String> source;
//     if (!v8::String::NewFromUtf8(isolate, wrapped_string.c_str(), v8::NewStringType::kNormal).ToLocal(&source)) {
//         ReportException(isolate, &try_catch, r->error_cb);
//         return;
//     }
//     bool failed;
//     auto res = ExecuteString(isolate, source, file_name, true, &failed, r->error_cb);
//     if (failed) {
//         ReportException(isolate, &try_catch, r->error_cb);
//         isolate->ThrowException(try_catch.Exception());
//         return;
//     }
//     if (!res->IsFunction()) {
//         fprintf(stderr, "wrapped file return was not a function, which doesnt make sense.");
//         exit(1);
//     }

//     Handle<Object> module = Object::New(isolate);
//     Handle<Object> exports = Object::New(isolate);
//     module->Set(exports_str, exports);
//     r->js->module_cache.Reset(isolate, module_cache->Set(context, file_full_path, module).ToLocalChecked());

//     Handle<Function> func = Handle<Function>::Cast(res);
//     Local<Value> func_args[3] = {exports, module, file_name};
//     func->Call(context->Global(), 3, func_args);
//     if (try_catch.HasCaught()) {
//         ReportException(isolate, &try_catch, r->error_cb);
//         isolate->ThrowException(try_catch.Exception());
//         return;
//     }

//     args.GetReturnValue().Set(Handle<Object>::Cast(module->Get(exports_str)));
//     r->js->include_path.pop_back();

// }

Nan::Persistent<v8::Function> RosebudJS::constructor;

NAN_MODULE_INIT(RosebudJS::Init)
{
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("RosebudVM").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(tpl, "poke", Poke);
    Nan::SetPrototypeMethod(tpl, "peek", Peek);
    Nan::SetPrototypeMethod(tpl, "memcpy", Memcpy);
    Nan::SetPrototypeMethod(tpl, "memset", Memset);

    Nan::SetPrototypeMethod(tpl, "mouse", Mouse);
    Nan::SetPrototypeMethod(tpl, "btn", Btn);
    Nan::SetPrototypeMethod(tpl, "btnp", Btnp);
    Nan::SetPrototypeMethod(tpl, "wheel", Wheel);
    Nan::SetPrototypeMethod(tpl, "key", Key);
    Nan::SetPrototypeMethod(tpl, "keyp", Keyp);

    Nan::SetPrototypeMethod(tpl, "pset", Pset);
    Nan::SetPrototypeMethod(tpl, "pget", Pget);
    Nan::SetPrototypeMethod(tpl, "palset", Palset);
    Nan::SetPrototypeMethod(tpl, "palget", Palget);
    Nan::SetPrototypeMethod(tpl, "line", Line);
    Nan::SetPrototypeMethod(tpl, "rect", Rect);
    Nan::SetPrototypeMethod(tpl, "rectfill", Rectfill);
    Nan::SetPrototypeMethod(tpl, "circ", Circ);
    Nan::SetPrototypeMethod(tpl, "circfill", Circfill);
    Nan::SetPrototypeMethod(tpl, "tri", Tri);
    Nan::SetPrototypeMethod(tpl, "trifill", Trifill);
    Nan::SetPrototypeMethod(tpl, "cls", Cls);
    Nan::SetPrototypeMethod(tpl, "print", Print);
    Nan::SetPrototypeMethod(tpl, "color", Color);

    Nan::SetPrototypeMethod(tpl, "loadCartData", LoadCartData);

    constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
    Nan::Set(target, Nan::New("RosebudVM").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
    Nan::Set(target, Nan::New("MEMSIZE").ToLocalChecked(), Nan::New(ROSE_MEMORY_SIZE));
}

// Buffers free their memory when they are gc'd, we dont want that
void bufferFreeCallback(char *data, void *hint) {}

RosebudJS::RosebudJS(uint8_t *mem)
{
    vm = new rose_vm(mem);
}

RosebudJS::~RosebudJS()
{
    delete vm;
}

void RosebudJS::New(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    if (info.IsConstructCall())
    {
        // Invoked as constructor: `new RosebudJS(...)`
        Nan::TypedArrayContents<uint8_t> bufferObject(info[0]);
        RosebudJS *obj = new RosebudJS(*bufferObject);
        if (info.Length() > 1 && info[1]->IsTrue())
        {
            obj->vm->meta.hd = true;
            obj->vm->make_mem_ranges();
        }
        obj->Wrap(info.This());
        Nan::Set(info.This(), Nan::New("memory").ToLocalChecked(), info[0]);
        // v8::Local<v8::Object> screenBuffer = Nan::NewBuffer((char *)obj->vm->screen.begin,
        //                                                      obj->vm->screen.end - obj->vm->screen.begin,
        //                                                      bufferFreeCallback,
        //                                                      nullptr)
        //                                           .ToLocalChecked();
        Nan::Set(info.This(), Nan::New("screenOffset").ToLocalChecked(), Nan::New<v8::Integer, uint32_t>(obj->vm->screen.begin - obj->vm->mem));
        Nan::Set(info.This(), Nan::New("screenLength").ToLocalChecked(), Nan::New<v8::Integer, uint32_t>(obj->vm->screen.end - obj->vm->screen.begin));

        // v8::Local<v8::Object> paletteBuffer = Nan::NewBuffer((char *)obj->vm->palette.begin,
        //                                                      obj->vm->palette.end - obj->vm->palette.begin,
        //                                                      bufferFreeCallback,
        //                                                      nullptr)
        //                                           .ToLocalChecked();
        // Nan::Set(info.This(), Nan::New("palette").ToLocalChecked(), paletteBuffer);
        Nan::Set(info.This(), Nan::New("paletteOffset").ToLocalChecked(), Nan::New<v8::Integer, uint32_t>(obj->vm->palette.begin - obj->vm->mem));
        Nan::Set(info.This(), Nan::New("paletteLength").ToLocalChecked(), Nan::New<v8::Integer, uint32_t>(obj->vm->palette.end - obj->vm->palette.begin));
        const uint32_t requiredDataSize = ROSE_MEMORY_SIZE - (obj->vm->meta.hd ? ROSE_HD_RUNTIME_RESERVED_MEMORY_SIZE : ROSE_RUNTIME_RESERVED_MEMORY_SIZE);
        Nan::Set(info.This(), Nan::New("cartSize").ToLocalChecked(), Nan::New(requiredDataSize));
        Nan::Set(info.This(), Nan::New("screenWidth").ToLocalChecked(), Nan::New((obj->vm->meta.hd ? ROSE_HD_SCREEN_WIDTH : ROSE_SCREEN_WIDTH)));
        Nan::Set(info.This(), Nan::New("screenHeight").ToLocalChecked(), Nan::New((obj->vm->meta.hd ? ROSE_HD_SCREEN_HEIGHT : ROSE_SCREEN_HEIGHT)));

        info.GetReturnValue().Set(info.This());
    }
    else
    {
        // Invoked as plain function `RosebudJS(...)`, turn into construct call.
        v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
        info.GetReturnValue().Set(Nan::NewInstance(cons).ToLocalChecked());
    }
}

void RosebudJS::LoadCartData(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    RosebudJS *obj = Nan::ObjectWrap::Unwrap<RosebudJS>(info.This());
    rose_vm *vm = obj->vm;
    const uint32_t requiredDataSize = ROSE_MEMORY_SIZE - (vm->meta.hd ? ROSE_HD_RUNTIME_RESERVED_MEMORY_SIZE : ROSE_RUNTIME_RESERVED_MEMORY_SIZE);
    std::string errorMessage = "First argument must be a Buffer or TypedArray of length " + std::to_string(requiredDataSize);
    if (info.Length() == 0)
    {
        Nan::ThrowError(errorMessage.c_str());
        return;
    }
    Nan::TypedArrayContents<uint8_t> bufferObject(info[0]);
    if (*bufferObject == nullptr)
    {
        Nan::ThrowError(errorMessage.c_str());
        return;
    }

    if (bufferObject.length() != requiredDataSize)
    {
        Nan::ThrowError(errorMessage.c_str());
        return;
    }
    memcpy(vm->mem, *bufferObject, bufferObject.length());
}

// rose_js* rose_js_create(rose_vm* r) {
//     rose_js* js = new rose_js();

//     js->fs = r->fs;

//     js->create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
//     Isolate* isolate = Isolate::New(js->create_params);
//     js->isolate = isolate;

//     v8::Isolate::Scope isolate_scope(isolate);
//     v8::HandleScope handle_scope(isolate);

//     Local<External> r_ptr = External::New(isolate, r);

//     Local<ObjectTemplate> global = ObjectTemplate::New(isolate);

//     v8::Local<v8::FunctionTemplate> require = FunctionTemplate::New(isolate, js_require, r_ptr);
//     global->Set(String::NewFromUtf8(isolate, "require", NewStringType::kNormal).ToLocalChecked(), require);

//     v8::Local<v8::FunctionTemplate> extcmd = FunctionTemplate::New(isolate, rose_js_sys_extcmd, r_ptr);
//     global->Set(String::NewFromUtf8(isolate, "extcmd", NewStringType::kNormal).ToLocalChecked(), extcmd);

//     v8::Local<v8::FunctionTemplate> pset = FunctionTemplate::New(isolate, rose_js_graphics_pset, r_ptr);
//     global->Set(String::NewFromUtf8(isolate, "pset", NewStringType::kNormal).ToLocalChecked(), pset);

//     v8::Local<v8::FunctionTemplate> pget = FunctionTemplate::New(isolate, rose_js_graphics_pget, r_ptr);
//     global->Set(String::NewFromUtf8(isolate, "pget", NewStringType::kNormal).ToLocalChecked(), pget);

//     v8::Local<v8::FunctionTemplate> palset = FunctionTemplate::New(isolate, rose_js_graphics_palset, r_ptr);
//     global->Set(String::NewFromUtf8(isolate, "palset", NewStringType::kNormal).ToLocalChecked(), palset);

//     v8::Local<v8::FunctionTemplate> palget = FunctionTemplate::New(isolate, rose_js_graphics_palget, r_ptr);
//     global->Set(String::NewFromUtf8(isolate, "palget", NewStringType::kNormal).ToLocalChecked(), palget);

//     v8::Local<v8::FunctionTemplate> line = FunctionTemplate::New(isolate, rose_js_graphics_line, r_ptr);
//     global->Set(String::NewFromUtf8(isolate, "line", NewStringType::kNormal).ToLocalChecked(), line);

//     v8::Local<v8::FunctionTemplate> rect = FunctionTemplate::New(isolate, rose_js_graphics_rect, r_ptr);
//     global->Set(String::NewFromUtf8(isolate, "rect", NewStringType::kNormal).ToLocalChecked(), rect);

//     v8::Local<v8::FunctionTemplate> rectfill = FunctionTemplate::New(isolate, rose_js_graphics_rectfill, r_ptr);
//     global->Set(String::NewFromUtf8(isolate, "rectfill", NewStringType::kNormal).ToLocalChecked(), rectfill);

//     v8::Local<v8::FunctionTemplate> circ = FunctionTemplate::New(isolate, rose_js_graphics_circ, r_ptr);
//     global->Set(String::NewFromUtf8(isolate, "circ", NewStringType::kNormal).ToLocalChecked(), circ);

//     v8::Local<v8::FunctionTemplate> circfill = FunctionTemplate::New(isolate, rose_js_graphics_circfill, r_ptr);
//     global->Set(String::NewFromUtf8(isolate, "circfill", NewStringType::kNormal).ToLocalChecked(), circfill);

//     v8::Local<v8::FunctionTemplate> tri = FunctionTemplate::New(isolate, rose_js_graphics_tri, r_ptr);
//     global->Set(String::NewFromUtf8(isolate, "tri", NewStringType::kNormal).ToLocalChecked(), tri);

//     v8::Local<v8::FunctionTemplate> trifill = FunctionTemplate::New(isolate, rose_js_graphics_trifill, r_ptr);
//     global->Set(String::NewFromUtf8(isolate, "trifill", NewStringType::kNormal).ToLocalChecked(), trifill);

//     v8::Local<v8::FunctionTemplate> cls = FunctionTemplate::New(isolate, rose_js_graphics_cls, r_ptr);
//     global->Set(String::NewFromUtf8(isolate, "cls", NewStringType::kNormal).ToLocalChecked(), cls);

//     v8::Local<v8::FunctionTemplate> print = FunctionTemplate::New(isolate, rose_js_graphics_print, r_ptr);
//     global->Set(String::NewFromUtf8(isolate, "print", NewStringType::kNormal).ToLocalChecked(), print);

//     v8::Local<v8::FunctionTemplate> color = FunctionTemplate::New(isolate, rose_js_graphics_color, r_ptr);
//     global->Set(String::NewFromUtf8(isolate, "color", NewStringType::kNormal).ToLocalChecked(), color);

//     v8::Local<v8::FunctionTemplate> poke = FunctionTemplate::New(isolate, rose_js_memory_poke, r_ptr);
//     global->Set(String::NewFromUtf8(isolate, "poke", NewStringType::kNormal).ToLocalChecked(), poke);

//     v8::Local<v8::FunctionTemplate> peek = FunctionTemplate::New(isolate, rose_js_memory_peek, r_ptr);
//     global->Set(String::NewFromUtf8(isolate, "peek", NewStringType::kNormal).ToLocalChecked(), peek);

//     v8::Local<v8::FunctionTemplate> rt_memcpy = FunctionTemplate::New(isolate, rose_js_memory_memcpy, r_ptr);
//     global->Set(String::NewFromUtf8(isolate, "memcpy", NewStringType::kNormal).ToLocalChecked(), rt_memcpy);

//     v8::Local<v8::FunctionTemplate> rt_memset = FunctionTemplate::New(isolate, rose_js_memory_memset, r_ptr);
//     global->Set(String::NewFromUtf8(isolate, "memset", NewStringType::kNormal).ToLocalChecked(), rt_memset);

//     v8::Local<v8::FunctionTemplate> cstore = FunctionTemplate::New(isolate, rose_js_memory_cstore, r_ptr);
//     global->Set(String::NewFromUtf8(isolate, "cstore", NewStringType::kNormal).ToLocalChecked(), cstore);

//     v8::Local<v8::FunctionTemplate> reload = FunctionTemplate::New(isolate, rose_js_memory_reload, r_ptr);
//     global->Set(String::NewFromUtf8(isolate, "reload", NewStringType::kNormal).ToLocalChecked(), reload);

//     v8::Local<v8::FunctionTemplate> mouse = FunctionTemplate::New(isolate, rose_js_input_mouse, r_ptr);
//     global->Set(String::NewFromUtf8(isolate, "mouse", NewStringType::kNormal).ToLocalChecked(), mouse);

//     v8::Local<v8::FunctionTemplate> btn = FunctionTemplate::New(isolate, rose_js_input_btn, r_ptr);
//     global->Set(String::NewFromUtf8(isolate, "btn", NewStringType::kNormal).ToLocalChecked(), btn);

//     v8::Local<v8::FunctionTemplate> btnp = FunctionTemplate::New(isolate, rose_js_input_btnp, r_ptr);
//     global->Set(String::NewFromUtf8(isolate, "btnp", NewStringType::kNormal).ToLocalChecked(), btnp);

//     v8::Local<v8::FunctionTemplate> key = FunctionTemplate::New(isolate, rose_js_input_key, r_ptr);

//     for (int keycode = ROSE_KEYCODE_A; keycode < ROSE_KEYCODE_UNKNOWN; ++keycode) {
//         const char * name = rose_keycode_to_string(
//                 (rose_keycode) keycode);
//         key->Set(String::NewFromUtf8(isolate, name, NewStringType::kNormal).ToLocalChecked(), Uint32::New(isolate, keycode));
//     }

//     global->Set(String::NewFromUtf8(isolate, "key", NewStringType::kNormal).ToLocalChecked(), key);

//     v8::Local<v8::FunctionTemplate> keyp = FunctionTemplate::New(isolate, rose_js_input_keyp, r_ptr);

//     for (int keycode = ROSE_KEYCODE_A; keycode < ROSE_KEYCODE_UNKNOWN; ++keycode) {
//         const char * name = rose_keycode_to_string(
//                 (rose_keycode) keycode);
//         keyp->Set(String::NewFromUtf8(isolate, name, NewStringType::kNormal).ToLocalChecked(), Uint32::New(isolate, keycode));
//     }

//     global->Set(String::NewFromUtf8(isolate, "keyp", NewStringType::kNormal).ToLocalChecked(), keyp);

//     v8::Local<v8::FunctionTemplate> wheel = FunctionTemplate::New(isolate, rose_js_input_wheel, r_ptr);
//     global->Set(String::NewFromUtf8(isolate, "wheel", NewStringType::kNormal).ToLocalChecked(), wheel);

//     js->global_template.Reset(isolate, global);

//     return js;
// }

// void rose_js_free(rose_js* js) {
//     js->module_cache.Reset();
//     js->context.Reset();
//     js->global_template.Reset();
//     js->isolate->Dispose();
//     delete js->create_params.array_buffer_allocator;
//     delete js;
// }
