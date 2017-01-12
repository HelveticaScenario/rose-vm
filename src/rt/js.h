#pragma once

#include "rt.fwd.h"

struct rose_js {
    rose_fs* fs;
    Isolate* isolate;
    Isolate::CreateParams create_params;
    Global<ObjectTemplate> global_template;
    Global<Context> context;
    vector<rose_file*> include_path;
    Global<v8::Map> module_cache;
};

rose_js* rose_js_create(rose_rt* r);

void rose_js_free(rose_js* js);



