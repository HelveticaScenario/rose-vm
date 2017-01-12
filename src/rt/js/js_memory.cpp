#include "rt/js/js_memory.h"

void rose_js_memory_poke(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HandleScope scope(args.GetIsolate());
    rose_rt* r = static_cast<rose_rt*>(Local<External>::Cast(args.Data())->Value());
    uint32_t idx = (uint32_t) args[0]->Uint32Value();
    uint8_t val = (uint8_t) args[1]->Int32Value();
    int err = rose_api_memory_poke(r, idx, val);
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

void rose_js_memory_peek(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HandleScope scope(args.GetIsolate());
    rose_rt* r = static_cast<rose_rt*>(Local<External>::Cast(args.Data())->Value());
    if (args.Length() >= 1) {
        uint32_t idx = (uint32_t) args[0]->Uint32Value();
        uint8_t res;
        int err = rose_api_memory_peek(r, idx, &res);
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
    } else {
        args.GetReturnValue().Set(0);
    }
}

void rose_js_memory_memcpy(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HandleScope scope(args.GetIsolate());
    rose_rt* r = static_cast<rose_rt*>(Local<External>::Cast(args.Data())->Value());
    if (args.Length() >= 3) {
        uint32_t dest_addr = (uint32_t) args[0]->Uint32Value();
        uint32_t source_addr = (uint32_t) args[1]->Uint32Value();
        uint32_t len = (uint32_t) args[2]->Uint32Value();
        int err = rose_api_memory_memcpy(r, dest_addr, source_addr, len);
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
}

void rose_js_memory_memset(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HandleScope scope(args.GetIsolate());
    rose_rt* r = static_cast<rose_rt*>(Local<External>::Cast(args.Data())->Value());
    if (args.Length() >= 3) {
        uint32_t dest_addr = (uint32_t) args[0]->Uint32Value();
        uint8_t val = (uint8_t) args[1]->Uint32Value();
        uint32_t len = (uint32_t) args[2]->Uint32Value();
        int err = rose_api_memory_memset(r, dest_addr, val, len);
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
}

void rose_js_memory_cstore(const v8::FunctionCallbackInfo<v8::Value>& args) {
    printf("cstore probably works, but it is untested.\n");
    HandleScope scope(args.GetIsolate());
    rose_rt* r = static_cast<rose_rt*>(Local<External>::Cast(args.Data())->Value());
    if (args.Length() >= 3) {
        uint32_t dest_addr = (uint32_t) args[0]->Uint32Value();
        uint32_t source_addr = (uint32_t) args[1]->Uint32Value();
        uint32_t len = (uint32_t) args[2]->Uint32Value();
        int err = rose_api_memory_cstore(r, dest_addr, source_addr, len);
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
}


void rose_js_memory_reload(const v8::FunctionCallbackInfo<v8::Value>& args) {
    printf("reload probably works, but it is untested.\n");
    HandleScope scope(args.GetIsolate());
    rose_rt* r = static_cast<rose_rt*>(Local<External>::Cast(args.Data())->Value());
    if (args.Length() >= 3) {
        uint32_t dest_addr = (uint32_t) args[0]->Uint32Value();
        uint32_t source_addr = (uint32_t) args[1]->Uint32Value();
        uint32_t len = (uint32_t) args[2]->Uint32Value();
        int err = rose_api_memory_reload(r, dest_addr, source_addr, len);
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
}


void rose_js_memory__writestr(const v8::FunctionCallbackInfo<v8::Value>& args) {
    printf("_writestr probably works, but it is untested.\n");
    Isolate* isolate = args.GetIsolate();
    if (args.Length() < 2) {
        isolate->ThrowException(String::NewFromUtf8(isolate, "missing path"));
        return;
    }
    rose_rt* r = static_cast<rose_rt*>(Local<External>::Cast(args.Data())->Value());

    HandleScope scope(args.GetIsolate());
    v8::Isolate::Scope isolate_scope(isolate);
    v8::TryCatch try_catch(isolate);
    Local<Context> context = r->js->context.Get(isolate);
    if (context.IsEmpty()) {
        fprintf(stderr, "something went horribly wrong and I'm so, so sorry.\n");
    }
    v8::Context::Scope context_scope(context);

    if (!args[0]->IsString()) {
        isolate->ThrowException(String::NewFromUtf8(isolate, "path must be a string."));
        return;
    }

    if (!args[1]->IsString()) {
        isolate->ThrowException(String::NewFromUtf8(isolate, "content must be a string."));
        return;
    }
    v8::String::Utf8Value argStr(args[0]->ToString());
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
    rose_file* file =  r->fs->cart;
    for (int i = 0; i < elems.size(); i++) {
        file = rose_fs_fetch_child(file, elems[i].c_str());
        if (file == NULL){
            isolate->ThrowException(String::NewFromUtf8(isolate, "invalid path"));
            return;
        }
    }
    String::Utf8Value content(args[1]);
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

void rose_js_memory__readstr(const v8::FunctionCallbackInfo<v8::Value>& args) {
    printf("_readstr probably works, but it is untested.\n");
    Isolate* isolate = args.GetIsolate();
    if (args.Length() < 1) {
        isolate->ThrowException(String::NewFromUtf8(isolate, "missing path"));
        return;
    }
    rose_rt* r = static_cast<rose_rt*>(Local<External>::Cast(args.Data())->Value());

    HandleScope scope(args.GetIsolate());
    v8::Isolate::Scope isolate_scope(isolate);
    v8::TryCatch try_catch(isolate);
    Local<Context> context = r->js->context.Get(isolate);
    if (context.IsEmpty()) {
        fprintf(stderr, "something went horribly wrong and I'm so, so sorry.\n");
    }
    v8::Context::Scope context_scope(context);

    if (!args[0]->IsString()) {
        isolate->ThrowException(String::NewFromUtf8(isolate, "path must be a string."));
        return;
    }
    v8::String::Utf8Value argStr(args[0]->ToString());
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
    rose_file* file =  r->fs->cart;
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


void rose_js_memory__savefile(const v8::FunctionCallbackInfo<v8::Value>& args) {
    printf("_savefile probably works, but it is untested.\n");
    Isolate* isolate = args.GetIsolate();
    if (args.Length() < 1) {
        isolate->ThrowException(String::NewFromUtf8(isolate, "missing path"));
        return;
    }
    rose_rt* r = static_cast<rose_rt*>(Local<External>::Cast(args.Data())->Value());

    HandleScope scope(args.GetIsolate());
    v8::Isolate::Scope isolate_scope(isolate);
    v8::TryCatch try_catch(isolate);
    Local<Context> context = r->js->context.Get(isolate);
    if (context.IsEmpty()) {
        fprintf(stderr, "something went horribly wrong and I'm so, so sorry.\n");
    }
    v8::Context::Scope context_scope(context);

    if (!args[0]->IsString()) {
        isolate->ThrowException(String::NewFromUtf8(isolate, "path must be a string."));
        return;
    }
    v8::String::Utf8Value argStr(args[0]->ToString());
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
    rose_file* file =  r->fs->cart;
    for (int i = 0; i < elems.size(); i++) {
        file = rose_fs_fetch_child(file, elems[i].c_str());
        if (file == NULL){
            isolate->ThrowException(String::NewFromUtf8(isolate, "invalid path"));
            return;
        }
    }
    r->fs->write_file(file);

}

void rose_js_memory__mkfile(const v8::FunctionCallbackInfo<v8::Value>& args) {
    printf("_mkfile probably works, but it is untested.\n");
    Isolate* isolate = args.GetIsolate();
    if (args.Length() < 1) {
        isolate->ThrowException(String::NewFromUtf8(isolate, "missing path"));
        return;
    }
    rose_rt* r = static_cast<rose_rt*>(Local<External>::Cast(args.Data())->Value());

    HandleScope scope(args.GetIsolate());
    v8::Isolate::Scope isolate_scope(isolate);
    v8::TryCatch try_catch(isolate);
    Local<Context> context = r->js->context.Get(isolate);
    if (context.IsEmpty()) {
        fprintf(stderr, "something went horribly wrong and I'm so, so sorry.\n");
    }
    v8::Context::Scope context_scope(context);

    if (!args[0]->IsString()) {
        isolate->ThrowException(String::NewFromUtf8(isolate, "path must be a string."));
        return;
    }
    v8::String::Utf8Value argStr(args[0]->ToString());
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
    rose_file* file =  r->fs->cart;
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

void rose_js_memory__rmfile(const v8::FunctionCallbackInfo<v8::Value>& args) {
    printf("_rmfile probably works, but it is untested.\n");
    Isolate* isolate = args.GetIsolate();
    if (args.Length() < 1) {
        isolate->ThrowException(String::NewFromUtf8(isolate, "missing path"));
        return;
    }
    rose_rt* r = static_cast<rose_rt*>(Local<External>::Cast(args.Data())->Value());

    HandleScope scope(args.GetIsolate());
    v8::Isolate::Scope isolate_scope(isolate);
    v8::TryCatch try_catch(isolate);
    Local<Context> context = r->js->context.Get(isolate);
    if (context.IsEmpty()) {
        fprintf(stderr, "something went horribly wrong and I'm so, so sorry.\n");
    }
    v8::Context::Scope context_scope(context);

    if (!args[0]->IsString()) {
        isolate->ThrowException(String::NewFromUtf8(isolate, "path must be a string."));
        return;
    }
    v8::String::Utf8Value argStr(args[0]->ToString());
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
    rose_file* file =  r->fs->cart;
    for (int i = 0; i < elems.size(); i++) {
        file = rose_fs_fetch_child(file, elems[i].c_str());
        if (file == NULL){
            isolate->ThrowException(String::NewFromUtf8(isolate, "invalid path"));
            return;
        }
    }
    if (file == r->fs->cart) {
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