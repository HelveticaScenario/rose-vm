#include "rt/game.h"

rose_game* rose_game_create(rose_fs* fs) {
    if (fs == NULL) {
        fprintf(stderr, "tried to create runtime game with null fs\n");
        exit(1);
    }
    rose_game* r = (rose_game*) malloc(sizeof(rose_game));
    r->rt = rose_rt_create(fs);
    if (!rose_game_reload(r)) {
        fprintf(stderr, "reload could not finish\n");
    }
    return r;
}

void rose_game_free(rose_game* r) {
    rose_rt_free(r->rt);
    free(r);
}

bool rose_game_reload(rose_game* r) {
    rose_rt_clear(r->rt);
    // insert any game runtime specific init code here
    return rose_rt_load_run_main(r->rt);
}

rose_game_error rose_call(rose_game* r, const char* name, uint8_t nargs, Local<Value>* args) {
    auto isolate = r->rt->js->isolate;
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);
    v8::Local<Context> context = r->rt->js->context.Get(isolate);
    if (context.IsEmpty()) {
        return ROSE_GAME_CRITICAL_ERR;
    }
    v8::Context::Scope context_scope(context);
    v8::TryCatch try_catch(isolate);
    Local<Object> jsGlobal = context->Global();
    Handle<Value> value = jsGlobal->Get(String::NewFromUtf8(isolate, name));
    Handle<Function> jsUpdateFunc = Handle<Function>::Cast(value);
    if (!jsUpdateFunc->IsFunction()) {
        return ROSE_GAME_FUN_NOT_FOUND;
    }
    jsUpdateFunc->Call(jsGlobal, nargs, args);
    if (try_catch.HasCaught()) {
        v8::String::Utf8Value str(try_catch.Exception());
        fprintf(stderr, "Error: %s\n", *str);
        return ROSE_GAME_CRITICAL_ERR;
    }
    return ROSE_GAME_NO_ERR;
}



rose_game_error rose_game_init(rose_game* r) {
    return rose_call(r, "_init", 0, NULL);
}

rose_game_error rose_game_update(rose_game* r) {
    return rose_call(r, "_update", 0, NULL);
}

rose_game_error rose_game_draw(rose_game* r) {
    return rose_call(r, "_draw", 0, NULL);
}

rose_game_error rose_game_onmouse(rose_game* r, int16_t x, int16_t y) {
    auto isolate = r->rt->js->isolate;
    v8::HandleScope handle_scope(isolate);
    Local<Value> args[2] = {Int32::New(isolate,(int16_t) x), Int32::New(isolate,(int16_t) y)};
    return rose_call(r, "_onmouse", 2, args);
}

rose_game_error rose_game_onwheel(rose_game* r, int16_t x, int16_t y, bool inverted) {
    auto isolate = r->rt->js->isolate;
    v8::HandleScope handle_scope(isolate);
    Local<Value> args[3] = {Int32::New(isolate, x), Int32::New(isolate, y), Boolean::New(isolate, inverted)};
    return rose_call(r, "_onwheel", 3, args);
}

rose_game_error rose_game_onbtn(rose_game* r, uint8_t code, bool pressed) {
    auto isolate = r->rt->js->isolate;
    v8::HandleScope handle_scope(isolate);
    Local<Value> args[2] = {Int32::New(isolate, code), Boolean::New(isolate, pressed)};
    return rose_call(r, "_onbtn", 2, args);
}

rose_game_error rose_game_onkey(rose_game* r, rose_keycode keycode, bool pressed, bool repeat) {
    auto isolate = r->rt->js->isolate;
    v8::HandleScope handle_scope(isolate);
    Local<Value> args[3] = {Int32::New(isolate, keycode), Boolean::New(isolate, pressed), Boolean::New(isolate, repeat)};
    return rose_call(r, "_onkey", 2, args);
}

rose_game_error rose_game_ontouch(rose_game* r) {
    // TODO: make this actually do something
    return rose_call(r, "_ontouch", 0, NULL);
}
