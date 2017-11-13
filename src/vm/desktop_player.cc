#include "rose.h"

void rose_desktop_player::call_update() {
    auto rt = this->get_active_rt();
    if (rt != NULL) {
        rt->call_update();
    }

}

void rose_desktop_player::call_draw() {
    auto rt = this->get_active_rt();
    if (rt != NULL) {
        rt->call_draw();
    }
}

void rose_desktop_player::call_onmouse(int16_t x, int16_t y) {
    auto rt = this->get_active_rt();
    if (rt != NULL) {
        rt->update_mouse_pos(x, y);
        rt->call_onmouse(x, y);
    }
}

void rose_desktop_player::call_onwheel(int16_t x, int16_t y, bool inverted) {
    if (x != 0 && y != 0) {
        auto rt = this->get_active_rt();
        if (rt != NULL) {
            rt->update_wheel_state(x, y, inverted);
            rt->call_onwheel(x, y, inverted);
        }
    } else {
        return;
    }
}

void rose_desktop_player::call_onbtn(uint8_t code, bool pressed) {
    auto rt = this->get_active_rt();
    if (rt != NULL) {
        rt->update_btn_state(code, pressed);
        rt->call_onbtn(code, pressed);
    }
}

bool rose_desktop_player::is_reload_shortcut(rose_keycode keycode, bool pressed) {
    bool res = keycode == ROSE_KEYCODE_R;
    res &= pressed;
    bool is_control = rose_get_bit(this->game_rt->key_states.begin, ROSE_KEYCODE_LCTRL) ||
                   rose_get_bit(this->game_rt->key_states.begin, ROSE_KEYCODE_RCTRL);

    bool is_gui = rose_get_bit(this->game_rt->key_states.begin, ROSE_KEYCODE_RGUI) ||
                  rose_get_bit(this->game_rt->key_states.begin, ROSE_KEYCODE_LGUI);
    res &= (is_control || is_gui);
    return res;
}

void rose_desktop_player::call_onkey(rose_keycode keycode, bool pressed, bool repeat) {
    if (this->is_reload_shortcut(keycode, pressed))
    {
        this->screenmode = ROSE_GAMEMODE;
        this->reload();
        return;
    }

    if (keycode == ROSE_KEYCODE_ESCAPE && pressed) {
        switch (this->screenmode) {
            case ROSE_GAMEMODE: {
                this->screenmode = ROSE_CONSOLEMODE;
                break;
            }
            case ROSE_CONSOLEMODE: {
                this->screenmode = ROSE_EDITORMODE;
                break;
            }
            case ROSE_EDITORMODE: {
                this->screenmode = ROSE_CONSOLEMODE;
                break;
            }
            default:break;
        }
        return;
    }

    this->update_keystate(keycode, pressed);
    auto rt = this->get_active_rt();
    if (rt != NULL) {
        rt->call_onkey(keycode, pressed, repeat);
    }
}

void rose_desktop_player::call_ontouch() {
    auto rt = this->get_active_rt();
    if (rt != NULL) {
        rt->call_ontouch();
    }
}

rose_desktop_player::rose_desktop_player(rose_fs* fs) {
    if (fs == NULL) {
        fprintf(stderr, "tried to create runtime game with null fs\n");
        exit(1);
    }
    this->fs = fs;
    auto cb = [this] (string exception_string) {
        this->error_cb(exception_string);
    };

    this->shell_extcmd_cb = [this] (const v8::FunctionCallbackInfo<v8::Value>& args) {
        v8::String::Utf8Value argStr(args[0]->ToString());
        string path(ToCString(argStr));
        if (path == "writestr") {
            this->writestr(args);
        } else if (path == "readstr") {
            this->readstr(args);
        } else if (path == "savefile") {
            this->savefile(args);
        } else if (path == "mkfile") {
            this->mkfile(args);
        } else if (path == "rmfile") {
            this->rmfile(args);
        } else if (path == "load") {
            this->load(args);
        } else if (path == "run") {
            this->run(args);
        } else if (path == "log") {
            this->log(args);
        }
    };

    this->shell_rt = new rose_vm(fs);
    this->shell_rt->error_cb = cb;
    this->shell_rt->extcmd_cb = this->shell_extcmd_cb;
    rose_file* shell_cart = this->fetch_sys_cart(ROSE_SHELL_CART_NAME);
    if (shell_cart == NULL) {
        fprintf(stderr, "shell cart not found\n");
        exit(1);
    }
    this->shell_rt->retarget(shell_cart, shell_cart);
    this->shell_rt->load_run_main();
    this->shell_rt->call_init();
    this->game_rt = new rose_vm(fs);
    this->game_rt->error_cb = cb;
    this->cart = NULL;
    this->screenmode = rose_screenmode::ROSE_CONSOLEMODE;
}

rose_desktop_player::~rose_desktop_player() {
    delete this->game_rt;
}

rose_file* rose_desktop_player::fetch_sys_cart(string cart_name) {
    rose_file* file =  rose_fs_fetch_child(this->fs->root, ROSE_SYS_DIR_NAME);
    if (file == NULL) {
        file = new rose_file();
        file->parent = this->fs->root;
        file->name = ROSE_CART_DIR_NAME;
        file->type = ROSE_CART_DIRECTORY;
        this->fs->write_file(file);
        return NULL;
    }
    file = rose_fs_fetch_child(file, cart_name);
    if (file == NULL) {
        return NULL;
    }
    if (file->type != ROSE_CART_DIRECTORY) {
        return NULL;
    }
    return file;
}


bool rose_desktop_player::reload() {
    if (this->cart == NULL) {
        return false;
    }
    if (!this->game_rt->clear()) {
        return false;
    }
    this->game_rt->retarget(cart, cart);
    if (!this->game_rt->load_run_main()) {
        return false;
    }

    // insert any game runtime specific init code here

    return this->game_rt->call_init();
}

bool rose_desktop_player::load_cart_path(string cart_path) {
    rose_file* cart = rose_fs_fetch_child(this->fs->root, cart_path);
    if (cart == NULL) {
        return false;
    }
    if (this->cart != NULL && !this->cart->on_disk) {
        rose_file_free_recurse(this->cart);
    }
    this->cart = cart;
    return reload();
}

rose_vm* rose_desktop_player::get_active_rt() {
    switch (this->screenmode) {
        case ROSE_GAMEMODE: {
            return this->game_rt;
            break;
        }
        case ROSE_CONSOLEMODE: {
            return this->shell_rt;
            break;
        }
        case ROSE_EDITORMODE: {
            return this->shell_rt;
            break;
        }
        default: {
            return NULL;
        };
    }
}

void rose_desktop_player::save_input_frame() {
    auto rt = this->get_active_rt();
    if (rt != NULL) {
        rt->save_input_frame();
    }
}

void rose_desktop_player::update_keystate(rose_keycode keycode, bool pressed) {
    auto rt = this->get_active_rt();
    if (rt != NULL) {
        rt->update_keystate(keycode, pressed);
    }
}

bool rose_desktop_player::is_hd() {
    auto rt = this->get_active_rt();
    if (rt != NULL) {
        return rt->meta.hd;
    } else {
        return false;
    }
}

uint32_t rose_desktop_player::get_screen_length() {
    if (is_hd()) {
        return ROSE_HD_SCREEN_SIZE;
    } else {
        return ROSE_SCREEN_SIZE;
    }
}

uint8_t rose_desktop_player::get_color_index(uint32_t i) {
    switch (this->screenmode) {
        case ROSE_CONSOLEMODE: {
            return *(shell_rt->screen.begin + i);
        }
        case ROSE_GAMEMODE: {
            return *(game_rt->screen.begin + i);
        }
        case ROSE_EDITORMODE: {
            return 2;
        }
        case ROSE_STARTUPMODE: {
            return 3;
        }
    }

}

void rose_desktop_player::get_color(rose_color& color, uint8_t color_index) {
    auto rt = get_active_rt();
    if (rt == NULL) {
        return;
    }
    auto c = (rt->palette.begin + (color_index * 3));
    color.r = *(c++);
    color.g = *(c++);
    color.b = *c;
}

void rose_desktop_player::error_cb(string exception_string) {
    this->screenmode = ROSE_CONSOLEMODE;
    fprintf(stderr, "%s", exception_string.c_str());
}



void rose_desktop_player::writestr(const v8::FunctionCallbackInfo<v8::Value>& args) {
    printf("_writestr probably works, but it is untested.\n");
    Isolate* isolate = args.GetIsolate();
    if (args.Length() < 3) {
        isolate->ThrowException(String::NewFromUtf8(isolate, "writestr: missing path"));
        return;
    }
    rose_vm* r = static_cast<rose_vm*>(Local<External>::Cast(args.Data())->Value());

    HandleScope scope(args.GetIsolate());
    v8::Isolate::Scope isolate_scope(isolate);
    v8::TryCatch try_catch(isolate);
    Local<Context> context = r->js->context.Get(isolate);
    if (context.IsEmpty()) {
        fprintf(stderr, "something went horribly wrong and I'm so, so sorry.\n");
    }
    v8::Context::Scope context_scope(context);

    if (!args[1]->IsString()) {
        isolate->ThrowException(String::NewFromUtf8(isolate, "writestr: path must be a string."));
        return;
    }

    if (!args[2]->IsString()) {
        isolate->ThrowException(String::NewFromUtf8(isolate, "writestr: content must be a string."));
        return;
    }
    v8::String::Utf8Value argStr(args[1]->ToString());
    string path(*argStr);
    // ltrim
    path.erase(path.begin(), std::find_if(path.begin(), path.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    // rtrim
    path.erase(std::find_if(path.rbegin(), path.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), path.end());


    if (path.length() == 0) {
        isolate->ThrowException(String::NewFromUtf8(isolate, "writestr: missing path"));
        return;
    }

    std::vector<std::string> elems;
    split(path, '/', elems);
    rose_file* file =  r->target_cart;
    for (int i = 0; i < elems.size(); i++) {
        file = rose_fs_fetch_child(file, elems[i].c_str());
        if (file == NULL){
            isolate->ThrowException(String::NewFromUtf8(isolate, "writestr: invalid path"));
            return;
        }
    }
    String::Utf8Value content(args[2]);
    size_t length = (size_t) content.length();

    if (file->buffer == NULL) {
        file->buffer = (uint8_t*) malloc(sizeof(char) * (length + 1));
    } else {
        file->buffer = (uint8_t*) realloc(file->buffer, sizeof(char) * (length + 1));
    }
    file->buffer_len = (length + 1);
    file->buffer[length] = '\0';
    if (*content != NULL) {
        memcpy(file->buffer, *content, length);
        file->in_mem = true;
    }
}

void rose_desktop_player::readstr(const v8::FunctionCallbackInfo<v8::Value>& args) {
    printf("_readstr probably works, but it is untested.\n");
    Isolate* isolate = args.GetIsolate();
    if (args.Length() < 2) {
        isolate->ThrowException(String::NewFromUtf8(isolate, "missing path"));
        return;
    }
    rose_vm* r = static_cast<rose_vm*>(Local<External>::Cast(args.Data())->Value());

    HandleScope scope(args.GetIsolate());
    v8::Isolate::Scope isolate_scope(isolate);
    v8::TryCatch try_catch(isolate);
    Local<Context> context = r->js->context.Get(isolate);
    if (context.IsEmpty()) {
        fprintf(stderr, "something went horribly wrong and I'm so, so sorry.\n");
    }
    v8::Context::Scope context_scope(context);

    if (!args[1]->IsString()) {
        isolate->ThrowException(String::NewFromUtf8(isolate, "path must be a string."));
        return;
    }
    v8::String::Utf8Value argStr(args[1]->ToString());
    string path(*argStr);
    // ltrim
    path.erase(path.begin(), std::find_if(path.begin(), path.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    // rtrim
    path.erase(std::find_if(path.rbegin(), path.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), path.end());


    if (path.length() == 0) {
        isolate->ThrowException(String::NewFromUtf8(isolate, "missing path"));
        return;
    }

    std::vector<std::string> elems;
    split(path, '/', elems);
    rose_file* file =  r->target_cart;
    for (int i = 0; i < elems.size(); i++) {
        file = rose_fs_fetch_child(file, elems[i].c_str());
        if (file == NULL){
            isolate->ThrowException(String::NewFromUtf8(isolate, "invalid path"));
            return;
        }
    }
    if (!file->in_mem) {
        r->fs->read_file(file);
    }

    if (file->buffer[file->buffer_len-1] != '\0') {
        file->buffer_len++;
        file->buffer = (uint8_t*) realloc(file->buffer, file->buffer_len);
        file->buffer[file->buffer_len-1] = '\0';
        file->last_modification = time(NULL);
    }

    args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, (const char*) file->buffer, v8::NewStringType::kNormal).ToLocalChecked());
}


void rose_desktop_player::savefile(const v8::FunctionCallbackInfo<v8::Value>& args) {
    printf("_savefile probably works, but it is untested.\n");
    Isolate* isolate = args.GetIsolate();
    if (args.Length() < 2) {
        isolate->ThrowException(String::NewFromUtf8(isolate, "missing path"));
        return;
    }
    rose_vm* r = static_cast<rose_vm*>(Local<External>::Cast(args.Data())->Value());

    HandleScope scope(args.GetIsolate());
    v8::Isolate::Scope isolate_scope(isolate);
    v8::TryCatch try_catch(isolate);
    Local<Context> context = r->js->context.Get(isolate);
    if (context.IsEmpty()) {
        fprintf(stderr, "something went horribly wrong and I'm so, so sorry.\n");
    }
    v8::Context::Scope context_scope(context);

    if (!args[1]->IsString()) {
        isolate->ThrowException(String::NewFromUtf8(isolate, "path must be a string."));
        return;
    }
    v8::String::Utf8Value argStr(args[1]->ToString());
    string path(*argStr);
    // ltrim
    path.erase(path.begin(), std::find_if(path.begin(), path.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    // rtrim
    path.erase(std::find_if(path.rbegin(), path.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), path.end());


    if (path.length() == 0) {
        isolate->ThrowException(String::NewFromUtf8(isolate, "missing path"));
        return;
    }

    std::vector<std::string> elems;
    split(path, '/', elems);
    rose_file* file =  r->target_cart;
    for (int i = 0; i < elems.size(); i++) {
        file = rose_fs_fetch_child(file, elems[i].c_str());
        if (file == NULL){
            isolate->ThrowException(String::NewFromUtf8(isolate, "invalid path"));
            return;
        }
    }
    r->fs->write_file(file);

}

void rose_desktop_player::mkfile(const v8::FunctionCallbackInfo<v8::Value>& args) {
    printf("_mkfile probably works, but it is untested.\n");
    Isolate* isolate = args.GetIsolate();
    if (args.Length() < 2) {
        isolate->ThrowException(String::NewFromUtf8(isolate, "missing path"));
        return;
    }
    rose_vm* r = static_cast<rose_vm*>(Local<External>::Cast(args.Data())->Value());

    HandleScope scope(args.GetIsolate());
    v8::Isolate::Scope isolate_scope(isolate);
    v8::TryCatch try_catch(isolate);
    Local<Context> context = r->js->context.Get(isolate);
    if (context.IsEmpty()) {
        fprintf(stderr, "something went horribly wrong and I'm so, so sorry.\n");
    }
    v8::Context::Scope context_scope(context);

    if (!args[1]->IsString()) {
        isolate->ThrowException(String::NewFromUtf8(isolate, "path must be a string."));
        return;
    }
    v8::String::Utf8Value argStr(args[1]->ToString());
    string path(*argStr);
    // ltrim
    path.erase(path.begin(), std::find_if(path.begin(), path.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    // rtrim
    path.erase(std::find_if(path.rbegin(), path.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), path.end());


    if (path.length() == 0) {
        isolate->ThrowException(String::NewFromUtf8(isolate, "missing path"));
        return;
    }

    std::vector<std::string> elems;
    split(path, '/', elems);
    rose_file* file =  r->target_cart;
    for (int i = 0; i < elems.size(); i++) {
        auto new_file = rose_fs_fetch_child(file, elems[i].c_str());
        if (new_file != NULL && new_file->type == ROSE_CART_DIRECTORY) {
            isolate->ThrowException(String::NewFromUtf8(isolate, "Error: path traverses into sub cartridge."));
            return;
        }
        if (new_file != NULL && new_file->type != ROSE_DIRECTORY && i < (elems.size()-1)) {
            isolate->ThrowException(String::NewFromUtf8(isolate, "Error: attempted to make a file a child of another file"));
            return;
        }
        if (new_file == NULL){
            new_file = new rose_file();
            if (i < (elems.size()-1)) {
                new_file->type = ROSE_DIRECTORY;
            } else {
                new_file->type = ROSE_CODE_FILE;
            }
            new_file->name = elems[i];
            new_file->on_disk = false;
            new_file->last_modification = time(NULL);
            new_file->in_mem = true;
            new_file->buffer_len = 0;
            new_file->buffer = NULL;
            new_file->parent = file;
            new_file->contents = {};
            rose_fs_add_child(file, new_file);
            r->fs->write_file(new_file);
        }
        file = new_file;
    }

}

void rose_desktop_player::rmfile(const v8::FunctionCallbackInfo<v8::Value>& args) {
    printf("_rmfile probably works, but it is untested.\n");
    Isolate* isolate = args.GetIsolate();
    if (args.Length() < 2) {
        isolate->ThrowException(String::NewFromUtf8(isolate, "missing path"));
        return;
    }
    rose_vm* r = static_cast<rose_vm*>(Local<External>::Cast(args.Data())->Value());

    HandleScope scope(args.GetIsolate());
    v8::Isolate::Scope isolate_scope(isolate);
    v8::TryCatch try_catch(isolate);
    Local<Context> context = r->js->context.Get(isolate);
    if (context.IsEmpty()) {
        fprintf(stderr, "something went horribly wrong and I'm so, so sorry.\n");
    }
    v8::Context::Scope context_scope(context);

    if (!args[1]->IsString()) {
        isolate->ThrowException(String::NewFromUtf8(isolate, "path must be a string."));
        return;
    }
    v8::String::Utf8Value argStr(args[1]->ToString());
    string path(*argStr);
    // ltrim
    path.erase(path.begin(), std::find_if(path.begin(), path.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    // rtrim
    path.erase(std::find_if(path.rbegin(), path.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), path.end());


    if (path.length() == 0) {
        isolate->ThrowException(String::NewFromUtf8(isolate, "missing path"));
        return;
    }

    std::vector<std::string> elems;
    split(path, '/', elems);
    rose_file* file =  r->target_cart;
    for (int i = 0; i < elems.size(); i++) {
        file = rose_fs_fetch_child(file, elems[i].c_str());
        if (file == NULL){
            isolate->ThrowException(String::NewFromUtf8(isolate, "invalid path"));
            return;
        }
    }
    if (file == r->target_cart) {
        isolate->ThrowException(String::NewFromUtf8(isolate, "cannot remove loaded cart"));
        return;
    }
    file->removed = true;
    rose_fs_error err = r->fs->write_file(file);
    if (err == ROSE_FS_CRITICAL_ERR) {
        isolate->ThrowException(String::NewFromUtf8(isolate, "critical filesystem error"));
        return;
    }
    rose_fs_remove_child(file->parent, file);
    rose_file_free_recurse(file);

}

void rose_desktop_player::load(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate* isolate = args.GetIsolate();
    if (args.Length() < 2) {
        isolate->ThrowException(String::NewFromUtf8(isolate, "missing path"));
        return;
    }
    rose_vm* r = static_cast<rose_vm*>(Local<External>::Cast(args.Data())->Value());

    HandleScope scope(args.GetIsolate());
    v8::Isolate::Scope isolate_scope(isolate);
    v8::TryCatch try_catch(isolate);
    Local<Context> context = r->js->context.Get(isolate);
    if (context.IsEmpty()) {
        fprintf(stderr, "something went horribly wrong and I'm so, so sorry.\n");
    }
    v8::Context::Scope context_scope(context);

    if (!args[1]->IsString()) {
        isolate->ThrowException(String::NewFromUtf8(isolate, "path must be a string."));
        return;
    }
    v8::String::Utf8Value argStr(args[1]->ToString());
    string path(*argStr);
    // ltrim
    path.erase(path.begin(), std::find_if(path.begin(), path.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    // rtrim
    path.erase(std::find_if(path.rbegin(), path.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), path.end());


    if (path.length() == 0) {
        isolate->ThrowException(String::NewFromUtf8(isolate, "missing path"));
        return;
    }

    std::vector<std::string> elems;
    split(path, '/', elems);
    rose_file* file =  rose_fs_fetch_child(r->fs->root, ROSE_CART_DIR_NAME);
    if (file == NULL) {
        file = new rose_file();
        file->parent = r->fs->root;
        file->name = ROSE_CART_DIR_NAME;
        file->type = ROSE_CART_DIRECTORY;
        r->fs->write_file(file);
        args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, "could not load", v8::NewStringType::kNormal).ToLocalChecked());
        return;
    }
    for (int i = 0; i < elems.size(); i++) {
        file = rose_fs_fetch_child(file, elems[i].c_str());
        if (file == NULL){
            args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, "could not load", v8::NewStringType::kNormal).ToLocalChecked());
            return;
        }
    }
    if (file->type == ROSE_CART_DIRECTORY) {
        this->game_rt->retarget(file, file);
        string msg = "loaded ";
        msg += file->name;
        args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, msg.c_str(), v8::NewStringType::kNormal).ToLocalChecked());
    } else {
        args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, "could not load", v8::NewStringType::kNormal).ToLocalChecked());
    }
}

void rose_desktop_player::run(const v8::FunctionCallbackInfo<v8::Value>& args) {
    this->screenmode = ROSE_GAMEMODE;
    this->reload();
}

void rose_desktop_player::log(const v8::FunctionCallbackInfo<v8::Value>& args) {
    for (int i = 1; i < args.Length(); i++) {
        v8::String::Utf8Value argStr(args[i]->ToString());
        printf("%s\n", ToCString(argStr));
    }
}