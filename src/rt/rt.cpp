#include "rose.h"

static v8::Platform* static_platform;

void rose_init(const char* base_path) {
    v8::V8::InitializeICUDefaultLocation(base_path);
    v8::V8::InitializeExternalStartupData(base_path);
    static_platform = v8::platform::CreateDefaultPlatform();
    v8::V8::InitializePlatform(static_platform);
    v8::V8::Initialize();
}

void rose_deinit() {
    V8::Dispose();
    V8::ShutdownPlatform();
    delete static_platform;
}

void rose_rt::make_mem_ranges() {
    bool hd = this->meta.hd;
    rose_memory_range* screen_range = this->screen == NULL ? new rose_memory_range() : this->screen;
    auto screen_begin = mem->end();
    std::advance(screen_begin, -(hd ? ROSE_HD_SCREEN_SIZE : ROSE_SCREEN_SIZE));
    screen_range->begin = screen_begin; // Robert, Here
    screen_range->end = mem->end();

    rose_memory_range* schema_range = this->schema == NULL ? new rose_memory_range() : this->schema;
    auto it = screen_range->end;
    std::advance(it, -(hd ? ROSE_HD_RUNTIME_RESERVED_MEMORY_SIZE : ROSE_RUNTIME_RESERVED_MEMORY_SIZE));
    std::advance(it, -ROSE_PALETTE_SIZE);
    std::advance(it, -ROSE_MEMORY_SCHEMA_SIZE);
    schema_range->begin = it;
    std::advance(it, ROSE_MEMORY_SCHEMA_SIZE);
    schema_range->end = it;

    rose_memory_range* palette_range = this->palette == NULL ? new rose_memory_range() : this->palette;
    palette_range->begin = it;
    std::advance(it, ROSE_PALETTE_SIZE);
    palette_range->end = it;

    rose_memory_range* palette_filter_range = this->palette_filter == NULL ? new rose_memory_range() : this->palette_filter;
    palette_filter_range->begin = it;
    std::advance(it, ROSE_PALETTE_INDEX_NUM);
    palette_filter_range->end = it;

    int i;
    for (i = 0; i < ROSE_PALETTE_INDEX_NUM; ++i) {
        palette_filter_range->begin[i] = (uint8_t) i;
    }

    rose_memory_range* palette_transparency_range = this->palette_transparency == NULL ? new rose_memory_range() : this->palette_transparency;
    palette_transparency_range->begin = it;
    std::advance(it, ROSE_PALETTE_INDEX_NUM / 8);
    palette_transparency_range->end = it;

    rose_set_bit(palette_transparency_range->begin, 0, true);

    rose_memory_range* clipping_region_range = this->clipping_region == NULL ? new rose_memory_range() : this->clipping_region;
    clipping_region_range->begin = it;
    std::advance(it, 8);
    clipping_region_range->end = it;

    uint16_t* clipping_region = (uint16_t*) clipping_region_range->begin;
    clipping_region[0] = 0;                      // x0
    clipping_region[1] = 0;                      // y0
    clipping_region[2] = (uint16_t) ((hd ? ROSE_HD_SCREEN_WIDTH : ROSE_SCREEN_WIDTH) - 1);  // x1
    clipping_region[3] = (uint16_t) ((hd ? ROSE_HD_SCREEN_HEIGHT : ROSE_SCREEN_HEIGHT) - 1); // y1

    auto pen_color_addr = it;
    *pen_color_addr = 6;
    std::advance(it, 1);

    rose_memory_range* print_cursor_range = this->print_cursor == NULL ? new rose_memory_range() : this->print_cursor;
    print_cursor_range->begin = it;
    std::advance(it, 4);
    print_cursor_range->end = it;

    uint16_t* print_cursor = (uint16_t*) print_cursor_range->begin;
    print_cursor[0] = 0; // x0
    print_cursor[1] = 0; // y0 // TODO: replace with actual starting position
    // once font size is finalized

    rose_memory_range* camera_offset_range = this->camera_offset == NULL ? new rose_memory_range() : this->camera_offset;
    camera_offset_range->begin = it;
    std::advance(it, 4);
    camera_offset_range->end = it;

    int16_t* camera_offset = (int16_t*) camera_offset_range->begin;
    camera_offset[0] = 0; // x0
    camera_offset[1] = 0; // y0 // TODO: replace with actual starting position
    // once font size is finalized

    rose_memory_range* pointer_positions_range = this->pointer_positions == NULL ? new rose_memory_range() : this->pointer_positions;
    pointer_positions_range->begin = it;
    std::advance(it, 11 * 4 /* 2 16 bit number */);
    pointer_positions_range->end = it;

    rose_memory_range* btn_states_range = this->btn_states == NULL ? new rose_memory_range() : this->btn_states;
    btn_states_range->begin = it;
    std::advance(it, 4 /* 32 bit fields */);
    btn_states_range->end = it;

    rose_memory_range* prev_btn_states_range = this->prev_btn_states == NULL ? new rose_memory_range() : this->prev_btn_states;
    prev_btn_states_range->begin = it;
    std::advance(it, 4 /* 32 bit fields */);
    prev_btn_states_range->end = it;

    rose_memory_range* mouse_wheel_range = this->mouse_wheel == NULL ? new rose_memory_range() : this->mouse_wheel;
    mouse_wheel_range->begin = it;
    std::advance(it, 5 /* 2 16 bit ints and one bool */);
    mouse_wheel_range->end = it;

    rose_memory_range* key_states_range = this->key_states == NULL ? new rose_memory_range() : this->key_states;
    key_states_range->begin = it;
    std::advance(it, 30 /* 240 bit fields */);
    key_states_range->end = it;

    rose_memory_range* prev_key_states_range = this->prev_key_states == NULL ? new rose_memory_range() : this->prev_key_states;
    prev_key_states_range->begin = it;
    std::advance(it, 30 /* 240 bit fields */);
    prev_key_states_range->end = it;

    this->screen = screen_range;
    this->schema = schema_range;
    this->palette = palette_range;
    this->palette_filter = palette_filter_range;
    this->palette_transparency = palette_transparency_range;
    this->clipping_region = clipping_region_range;
    this->pen_color_addr = pen_color_addr;
    this->print_cursor = print_cursor_range;
    this->camera_offset = camera_offset_range;
    this->pointer_positions = pointer_positions_range;
    this->btn_states = btn_states_range;
    this->prev_btn_states = prev_btn_states_range;
    this->mouse_wheel = mouse_wheel_range;
    this->key_states = key_states_range;
    this->prev_key_states = prev_key_states_range;
}

rose_rt::rose_rt(rose_fs* fs) {
    if (fs == NULL) {
        fprintf(stderr, "tried to create runtime base with null fs\n");
        exit(1);
    }

//    std::array<uint8_t, ROSE_MEMORY_SIZE>* mem = ;


    this->meta.name = "";
    this->meta.author = "";
    this->meta.hd = false;
    this->fs = fs;
    this->mem = new std::array<uint8_t, ROSE_MEMORY_SIZE>();
    this->make_mem_ranges();
    this->js = rose_js_create(this);
}

rose_rt::~rose_rt() {
    rose_js_free(this->js);
    // dont free fs, managed by system layer
    delete this->mem;
    delete this->screen;
    delete this->schema;
    delete this->palette;
    delete this->palette_filter;
    delete this->palette_transparency;
    delete this->clipping_region;
    delete this->print_cursor;
    delete this->camera_offset;
    delete this->pointer_positions;
    delete this->btn_states;
    delete this->mouse_wheel;
    delete this->key_states;
}

bool rose_rt::clear() {
    this->mem->fill(0);
    this->js->module_cache.Reset(); // 16712468
    this->js->context.Reset();
    return true;
}


bool rose_rt::load_run_main() {
    if (this->self_cart == NULL) {
        return false;
    }

    auto isolate = this->js->isolate;

    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);
    v8::TryCatch try_catch(isolate);

    Local<Context> context = Context::New(isolate, NULL, this->js->global_template.Get(isolate));

    rose_file* cart_info = rose_fs_fetch_cart_info_file(this->self_cart);
    if (cart_info != NULL) {
        if (!cart_info->in_mem) {
            auto err = this->fs->read_file(cart_info);
            if (err == ROSE_FS_CRITICAL_ERR) {
                return false;
            }
        }

        if (cart_info->buffer[cart_info->buffer_len-1] != '\0') {
            cart_info->buffer_len++;
            cart_info->buffer = (uint8_t*) realloc(cart_info->buffer, cart_info->buffer_len);
            cart_info->buffer[cart_info->buffer_len-1] = '\0';
            cart_info->last_modification = time(NULL);
        }


        v8::Local<v8::String> json;
        if (!v8::String::NewFromUtf8(isolate, (const char*) cart_info->buffer, v8::NewStringType::kNormal).ToLocal(&json)) {
            ReportException(isolate, &try_catch);
            return false;
        }

        v8::Local<v8::Value> result;
        if (!v8::JSON::Parse(context, json).ToLocal(&result)) {
            ReportException(isolate, &try_catch);
            return false;
        }
        if (result->IsObject()) {
            auto obj = result->ToObject();
            auto name_key = v8::String::NewFromUtf8(isolate, "name", v8::NewStringType::kNormal).ToLocalChecked();
            if (obj->Has(name_key)) {
                v8::String::Utf8Value name(obj->Get(name_key)->ToString());
                this->meta.name = *name;
            }

            auto author_key = v8::String::NewFromUtf8(isolate, "author", v8::NewStringType::kNormal).ToLocalChecked();
            if (obj->Has(author_key)) {
                v8::String::Utf8Value author(obj->Get(author_key)->ToString());
                this->meta.author = *author;
            }

            auto hd_key = v8::String::NewFromUtf8(isolate, "hd", v8::NewStringType::kNormal).ToLocalChecked();
            if (obj->Has(hd_key)) {
                this->meta.hd = obj->Get(hd_key)->ToBoolean()->BooleanValue();
            }
        }
    }

    make_mem_ranges();

//    this->mem->fill(0);
//    memcpy(this->palette->begin, rose_default_palette, sizeof(rose_default_palette));
//    rose_api_graphics_set_spritesheet_meta(this, 0, 256, 256, 1, 1);
//    auto data = rose_fs_fetch_cart_data_file(this->self_cart);
//    data->buffer = this->mem->begin();
//    data->buffer_len = this->palette->end - data->buffer;
//    this->fs->write_file(data);

    rose_file* cart_data = rose_fs_fetch_cart_data_file(this->self_cart);
    if (cart_data == NULL) {
        fprintf(stderr, "ERROR: no data file found\n");
        return false;
    }

    if (!cart_data->in_mem) {
        auto err = this->fs->read_file(cart_data);
        if (err == ROSE_FS_CRITICAL_ERR) {
            return false;
        }
    }

    if (cart_data->buffer_len > (this->mem->size() - (this->meta.hd ? ROSE_HD_RUNTIME_RESERVED_MEMORY_SIZE : ROSE_RUNTIME_RESERVED_MEMORY_SIZE))) {
        fprintf(stderr, "ERROR: tried to reload runtime and cartridge memory size was bigger than available memory size\n");
        return false;
    }
    memcpy(this->mem->data(), cart_data->buffer, cart_data->buffer_len);

    rose_file* main = rose_fs_fetch_cart_js_main(this->self_cart);
    if (main == NULL) {
        fprintf(stderr, "ERROR: no main file found\n");
        return false;
    }
    this->js->include_path.clear();
    this->js->include_path.push_back(main);

    if (!main->in_mem) {
        this->fs->read_file(main);
    }


    if (main->buffer[main->buffer_len-1] != '\0') {
        main->buffer_len++;
        main->buffer = (uint8_t*) realloc(main->buffer, main->buffer_len);
        main->buffer[main->buffer_len-1] = '\0';
        main->last_modification = time(NULL);
    }



    this->js->context.Reset(isolate, context);
    if (context.IsEmpty()) {
        fprintf(stderr, "something went horribly wrong and I'm so, so sorry.\n");
    }
    v8::Context::Scope context_scope(context);

    v8::Local<v8::String> file_name = v8::String::NewFromUtf8(isolate, main->name.c_str(), v8::NewStringType::kNormal).ToLocalChecked();
    v8::Local<v8::String> source;
    if (!v8::String::NewFromUtf8(isolate, (const char*) main->buffer, v8::NewStringType::kNormal).ToLocal(&source)) {
        ReportException(isolate, &try_catch);
        return false;
    }

    Local<Map> module_cache = Map::New(isolate);
    this->js->module_cache.Reset(isolate, module_cache);

    bool failed;
    auto res = ExecuteString(isolate, source, file_name, true, &failed);
    if (failed) {
        ReportException(isolate, &try_catch);
        return false;
    }

    while (v8::platform::PumpMessageLoop(static_platform, isolate))
        continue;

    return true;
}

void rose_rt::save_input_frame() {
    memcpy(this->prev_btn_states->begin, this->btn_states->begin, this->prev_btn_states->end - this->prev_btn_states->begin);
    memcpy(this->prev_key_states->begin, this->key_states->begin, this->prev_key_states->end - this->prev_key_states->begin);
}

void rose_rt::update_mousestate(const rose_mousestate* mousestate) {
    int16_t* pointer = (int16_t*) this->pointer_positions->begin;
    pointer[20] = mousestate->x;
    pointer[21] = mousestate->y;

    rose_set_bit(this->btn_states->begin, ROSE_LEFT_MOUSE_IDX, mousestate->left_btn_down);
    rose_set_bit(this->btn_states->begin, ROSE_RIGHT_MOUSE_IDX, mousestate->right_btn_down);
    rose_set_bit(this->btn_states->begin, ROSE_MIDDLE_MOUSE_IDX, mousestate->middle_btn_down);
    rose_set_bit(this->btn_states->begin, ROSE_X1_MOUSE_IDX, mousestate->x1_btn_down);
    rose_set_bit(this->btn_states->begin, ROSE_X2_MOUSE_IDX, mousestate->x2_btn_down);

    int16_t* wheel_delta = (int16_t*) this->mouse_wheel->begin;
    wheel_delta[0] = mousestate->wheel_x;
    wheel_delta[1] = mousestate->wheel_y;

    bool* wheel_inverted = (bool*) (this->mouse_wheel->begin + 4);
    *wheel_inverted = mousestate->wheel_inverted;
}

void rose_rt::update_keystate(rose_keycode keycode, bool pressed) {
    if (keycode < ROSE_KEYCODE_UNKNOWN) {
        rose_set_bit(this->key_states->begin, keycode, pressed);
    }
}

void rose_rt::reset_input(rose_mousestate* mousestate) {
    mousestate->left_btn_down = false;
    mousestate->right_btn_down = false;
    mousestate->middle_btn_down = false;
    mousestate->x1_btn_down = false;
    mousestate->x2_btn_down = false;
    mousestate->wheel_x = 0;
    mousestate->wheel_y = 0;

    this->update_mousestate(mousestate);
    for (int keycode = ROSE_KEYCODE_A; keycode < ROSE_KEYCODE_UNKNOWN; ++keycode) {
        this->update_keystate((rose_keycode) keycode, false);
    }
    this->save_input_frame();
}

void rose_rt::retarget(rose_file* self, rose_file* target) {
    this->self_cart = self;
    this->target_cart = target;
}

rose_rt_error rose_rt::rose_call( const char* name, uint8_t nargs, Local<Value>* args) {
    auto isolate = this->js->isolate;
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);
    v8::Local<Context> context = this->js->context.Get(isolate);
    if (context.IsEmpty()) {
        return ROSE_RT_CRITICAL_ERR;
    }
    v8::Context::Scope context_scope(context);
    v8::TryCatch try_catch(isolate);
    Local<Object> jsGlobal = context->Global();
    Handle<Value> value = jsGlobal->Get(String::NewFromUtf8(isolate, name));
    Handle<Function> jsUpdateFunc = Handle<Function>::Cast(value);
    if (!jsUpdateFunc->IsFunction()) {
        return ROSE_RT_FUN_NOT_FOUND;
    }
    jsUpdateFunc->Call(jsGlobal, nargs, args);
    if (try_catch.HasCaught()) {
        v8::String::Utf8Value str(try_catch.Exception());
        fprintf(stderr, "Error: %s\n", *str);
        return ROSE_RT_CRITICAL_ERR;
    }
    return ROSE_RT_NO_ERR;
}

rose_rt_error rose_rt::call_init() {
    return rose_call("_init", 0, NULL);
}

rose_rt_error rose_rt::call_update() {
    return rose_call("_update", 0, NULL);
}

rose_rt_error rose_rt::call_draw() {
    return rose_call("_draw", 0, NULL);
}

rose_rt_error rose_rt::call_onmouse(int16_t x, int16_t y) {
    auto isolate = this->js->isolate;
    v8::HandleScope handle_scope(isolate);
    Local<Value> args[2] = {Int32::New(isolate,(int16_t) x), Int32::New(isolate,(int16_t) y)};
    return rose_call("_onmouse", 2, args);
}

rose_rt_error rose_rt::call_onwheel(int16_t x, int16_t y, bool inverted) {
    auto isolate = this->js->isolate;
    v8::HandleScope handle_scope(isolate);
    Local<Value> args[3] = {Int32::New(isolate, x), Int32::New(isolate, y), Boolean::New(isolate, inverted)};
    return rose_call("_onwheel", 3, args);
}

rose_rt_error rose_rt::call_onbtn(uint8_t code, bool pressed) {
    auto isolate = this->js->isolate;
    v8::HandleScope handle_scope(isolate);
    Local<Value> args[2] = {Int32::New(isolate, code), Boolean::New(isolate, pressed)};
    return rose_call("_onbtn", 2, args);
}

rose_rt_error rose_rt::call_onkey(rose_keycode keycode, bool pressed, bool repeat) {
    auto isolate = this->js->isolate;
    v8::HandleScope handle_scope(isolate);
    Local<Value> args[3] = {Int32::New(isolate, keycode), Boolean::New(isolate, pressed), Boolean::New(isolate, repeat)};
    return rose_call("_onkey", 2, args);
}

rose_rt_error rose_rt::call_ontouch() {
    // TODO: make this actually do something
    return rose_call("_ontouch", 0, NULL);
}

void rose_set_bit(uint8_t* arr, uint8_t addr, bool val) {
    uint8_t idx = (uint8_t) (addr / 8);
    uint8_t bit = (uint8_t) (addr % 8);
    if (val) {
        arr[idx] |= 1 << bit;
    } else {
        arr[idx] &= ~(1 << bit);
    }
}

bool rose_get_bit(uint8_t* arr, uint8_t addr) {
    uint8_t idx = (uint8_t) (addr / 8);
    uint8_t bit = (uint8_t) (addr % 8);
    return (bool) ((arr[idx] >> bit) & 1);
}


