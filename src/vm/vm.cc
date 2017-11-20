#include "vm.h"

// static v8::Platform* static_platform;

// void rose_init(const char* base_path) {
//     v8::V8::InitializeICUDefaultLocation(base_path);
//     v8::V8::InitializeExternalStartupData(base_path);
//     static_platform = v8::platform::CreateDefaultPlatform();
//     v8::V8::InitializePlatform(static_platform);
//     v8::V8::Initialize();
// }

// void rose_deinit() {
//     V8::Dispose();
//     V8::ShutdownPlatform();
//     delete static_platform;
// }

void rose_vm::reset_palette_map()
{
    for (int i = 0; i < ROSE_PALETTE_INDEX_NUM; ++i)
    {
        palette_map.begin[i] = (uint8_t)i;
    }
}

void rose_vm::reset_palette_transparency()
{
    for (auto it = palette_transparency.begin; it > palette_transparency.end; it++)
    {
        *it = 0;
    }
    rose_set_bit(palette_transparency.begin, 0, true);
}

void rose_vm::reset_clipping_region()
{
    int16_t *ptr = (int16_t *)clipping_region.begin;
    ptr[0] = 0;                                                                     // x0
    ptr[1] = 0;                                                                     // y0
    ptr[2] = (int16_t)((meta.hd ? ROSE_HD_SCREEN_WIDTH : ROSE_SCREEN_WIDTH) - 1);   // x1
    ptr[3] = (int16_t)((meta.hd ? ROSE_HD_SCREEN_HEIGHT : ROSE_SCREEN_HEIGHT) - 1); // y1
}

void rose_vm::reset_pen_color()
{
    *pen_color_addr = 6;
}

void rose_vm::reset_print_cursor()
{
    int16_t *ptr = (int16_t *)print_cursor.begin;
    ptr[0] = 0;
    ptr[1] = 0;
}

void rose_vm::reset_camera_offset()
{
    int16_t *ptr = (int16_t *)camera_offset.begin;
    ptr[0] = 0; // x0
    ptr[1] = 0; // y0 // TODO: replace with actual starting position
    // once font size is finalized
}

void rose_vm::reset_pointer_positions()
{
    int16_t *ptr = (int16_t *)pointer_positions.begin;
    ptr[0] = 0; // x0
    ptr[1] = 0; // y0 // TODO: replace with actual starting position
}

void rose_vm::reset_btn_states()
{
    for (auto it = btn_states.begin; it < btn_states.end; it++)
    {
        *it = 0;
    }
    for (auto it = prev_btn_states.begin; it < prev_btn_states.end; it++)
    {
        *it = 0;
    }
}

void rose_vm::reset_mouse_wheel()
{
    for (auto it = mouse_wheel.begin; it < mouse_wheel.end; it++)
    {
        *it = 0;
    }
}

void rose_vm::reset_key_states()
{
    for (auto it = key_states.begin; it < key_states.end; it++)
    {
        *it = 0;
    }
    for (auto it = prev_key_states.begin; it < prev_key_states.end; it++)
    {
        *it = 0;
    }
}

void rose_vm::reset_palette()
{
    memcpy(palette.begin, rose_default_palette, sizeof(rose_default_palette));
}

void rose_vm::reset_schema()
{
    set_spritesheet_meta(0, 256, 256, 1, 1);
}

void rose_vm::reset_screen()
{
    cls();
}

void rose_vm::reset_userdata()
{
    memset(mem, 0, (size_t)(meta.hd ? ROSE_HD_USERSPACE_MEMORY_SIZE : ROSE_USERSPACE_MEMORY_SIZE));
}

void rose_vm::reset_font_data()
{
    memcpy(font_data.begin, rose_bit_font, sizeof(rose_bit_font));
}

void rose_vm::copy_input_from_other(rose_vm *other)
{
    auto len = prev_key_states.end - pointer_positions.begin;
    memcpy(pointer_positions.begin, other->pointer_positions.begin, len);
}

void rose_vm::copy_screen_from_other(rose_vm *other)
{
    if (meta.hd == other->meta.hd)
    {
        memcpy(screen.begin, other->screen.begin, screen.end - screen.begin);
    }
    else if (meta.hd)
    {
        uint8_t *this_screen_ptr = screen.begin;
        uint8_t *other_screen_ptr = other->screen.begin;
        for (auto i = 0; i < ROSE_SCREEN_SIZE; i++)
        {
            this_screen_ptr[i * 2] = other_screen_ptr[i];
            this_screen_ptr[i * 2 + 1] = other_screen_ptr[i];
        }
    }
    else
    {
        uint8_t *this_screen_ptr = screen.begin;
        uint8_t *other_screen_ptr = other->screen.begin;
        for (auto i = 0; i < ROSE_SCREEN_SIZE; i++)
        {
            this_screen_ptr[i] = other_screen_ptr[i * 2];
        }
    }
}


// TODO: need some way of testing that these are correct, makes me nervous
void rose_vm::make_mem_ranges()
{
    bool hd = meta.hd;
    screen.end = mem + ROSE_MEMORY_SIZE;
    screen.begin  = screen.end -  (hd ? ROSE_HD_SCREEN_SIZE : ROSE_SCREEN_SIZE);

    auto it = screen.end;
    it -= (hd ? ROSE_HD_RUNTIME_RESERVED_MEMORY_SIZE : ROSE_RUNTIME_RESERVED_MEMORY_SIZE);
    it -= ROSE_PALETTE_SIZE;
    it -= ROSE_MEMORY_SCHEMA_SIZE;
    schema.begin = it;
    it += ROSE_MEMORY_SCHEMA_SIZE;
    schema.end = it;

    palette.begin = it;
    it += ROSE_PALETTE_SIZE;
    palette.end = it;

    palette_map.begin = it;
    it += ROSE_PALETTE_INDEX_NUM;
    palette_map.end = it;

    palette_transparency.begin = it;
    it += (ROSE_PALETTE_INDEX_NUM / 8);
    palette_transparency.end = it;

    clipping_region.begin = it;
    it += 8;
    clipping_region.end = it;

    pen_color_addr = it;

    it += 1;

    print_cursor.begin = it;
    it += 4;
    print_cursor.end = it;

    camera_offset.begin = it;
    it += 4;
    camera_offset.end = it;

    pointer_positions.begin = it;
    it += (11 * 4 /* 2 16 bit number */);
    pointer_positions.end = it;

    btn_states.begin = it;
    it += (4 /* 32 bit fields */);
    btn_states.end = it;

    prev_btn_states.begin = it;
    it += (4 /* 32 bit fields */);
    prev_btn_states.end = it;

    mouse_wheel.begin = it;
    it += (5 /* 2 16 bit ints and one bool */);
    mouse_wheel.end = it;

    key_states.begin = it;
    it += (30 /* 240 bit fields */);
    key_states.end = it;

    prev_key_states.begin = it;
    it += (30 /* 240 bit fields */);
    prev_key_states.end = it;

    font_data.begin = it;
    it += (sizeof(rose_bit_font));
    font_data.end = it;
}
// rose_vm::rose_vm() {

// }

// rose_vm::rose_vm(rose_fs* fs) {
rose_vm::rose_vm(unsigned char *mem)
{
    // if (fs == NULL) {
    //     fprintf(stderr, "tried to create runtime base with null fs\n");
    //     exit(1);
    // }

    // this->fs = fs;

    meta.name = "";
    meta.author = "";
    meta.hd = false;

    this->mem = mem;
    make_mem_ranges();
    memset(this->mem, 0, ROSE_MEMORY_SIZE);
    reset_palette_map();
    reset_palette_transparency();
    reset_clipping_region();
    reset_pen_color();
    reset_print_cursor();
    reset_camera_offset();
    reset_pointer_positions();
    reset_btn_states();
    reset_mouse_wheel();
    reset_key_states();
    reset_palette();
    reset_screen();
    reset_schema();
    reset_userdata();
    reset_font_data();
}

rose_vm::~rose_vm()
{
    // delete mem;
}

bool rose_vm::clear()
{
    reset_palette_map();
    reset_palette_transparency();
    reset_clipping_region();
    reset_pen_color();
    reset_print_cursor();
    reset_camera_offset();
    reset_palette();
    reset_schema();
    reset_userdata();
    reset_font_data();
    return true;
}

void rose_vm::save_input_frame()
{
    memcpy(prev_btn_states.begin, btn_states.begin, prev_btn_states.end - prev_btn_states.begin);
    memcpy(prev_key_states.begin, key_states.begin, prev_key_states.end - prev_key_states.begin);
}

void rose_vm::update_mouse_pos(int16_t x, int16_t y)
{
    int16_t *pointer = (int16_t *)pointer_positions.begin;
    pointer[20] = x;
    pointer[21] = y;
}

void rose_vm::update_btn_state(uint8_t btn, bool pressed)
{
    rose_set_bit(btn_states.begin, btn, pressed);
}

void rose_vm::update_wheel_state(int16_t delta_x, int16_t delta_y, bool inverted)
{
    int16_t *wheel_delta = (int16_t *)mouse_wheel.begin;
    wheel_delta[0] = delta_x;
    wheel_delta[1] = delta_y;

    bool *wheel_inverted = (bool *)(mouse_wheel.begin + 4);
    *wheel_inverted = inverted;
}

void rose_vm::update_keystate(rose_keycode keycode, bool pressed)
{
    if (keycode < ROSE_KEYCODE_MAX_VALUE)
    {
        rose_set_bit(key_states.begin, keycode, pressed);
    }
}

// rose_vm::rose_vm(rose_fs* fs): rose_vm(fs) {
//     js = rose_js_create(this);
// }

// rose_vm::~rose_vm() {
//     rose_js_free(js);
// }

// bool rose_vm::clear() {
//     if (rose_vm::clear()) {
//         js->module_cache.Reset();
//         js->context.Reset();
//         return true;
//     }
//     return false;
// }

// bool rose_vm::load_run_main() {
//     if (self_cart == NULL) {
//         return false;
//     }

//     auto isolate = js->isolate;

//     v8::Isolate::Scope isolate_scope(isolate);
//     v8::HandleScope handle_scope(isolate);
//     v8::TryCatch try_catch(isolate);

//     Local<Context> context = Context::New(isolate, NULL, js->global_template.Get(isolate));

//     rose_file* cart_info = rose_fs_fetch_cart_info_file(self_cart);
//     if (cart_info != NULL) {
//         if (!cart_info->in_mem) {
//             auto err = fs->read_file(cart_info);
//             if (err == ROSE_FS_CRITICAL_ERR) {
//                 return false;
//             }
//         }

//         if (cart_info->buffer[cart_info->buffer_len-1] != '\0') {
//             cart_info->buffer_len++;
//             cart_info->buffer = (uint8_t*) realloc(cart_info->buffer, cart_info->buffer_len);
//             cart_info->buffer[cart_info->buffer_len-1] = '\0';
//             cart_info->last_modification = time(NULL);
//         }

//         v8::Local<v8::String> json;
//         if (!v8::String::NewFromUtf8(isolate, (const char*) cart_info->buffer, v8::NewStringType::kNormal).ToLocal(&json)) {
//             ReportException(isolate, &try_catch, error_cb);
//             return false;
//         }

//         v8::Local<v8::Value> result;
//         if (!v8::JSON::Parse(context, json).ToLocal(&result)) {
//             ReportException(isolate, &try_catch, error_cb);
//             return false;
//         }
//         if (result->IsObject()) {
//             auto obj = result->ToObject();
//             auto name_key = v8::String::NewFromUtf8(isolate, "name", v8::NewStringType::kNormal).ToLocalChecked();
//             if (obj->Has(name_key)) {
//                 v8::String::Utf8Value name(obj->Get(name_key)->ToString());
//                 meta.name = *name;
//             }

//             auto author_key = v8::String::NewFromUtf8(isolate, "author", v8::NewStringType::kNormal).ToLocalChecked();
//             if (obj->Has(author_key)) {
//                 v8::String::Utf8Value author(obj->Get(author_key)->ToString());
//                 meta.author = *author;
//             }

//             auto hd_key = v8::String::NewFromUtf8(isolate, "hd", v8::NewStringType::kNormal).ToLocalChecked();
//             if (obj->Has(hd_key)) {
//                 meta.hd = obj->Get(hd_key)->ToBoolean()->BooleanValue();
//             }
//         }
//     }

//     make_mem_ranges();
//     clear();

// //    mem->fill(0);
// //    rt_memcpy(palette->begin, rose_default_palette, sizeof(rose_default_palette));
// //    rose_api_graphics_set_spritesheet_meta(this, 0, 256, 256, 1, 1);
// //    auto data = rose_fs_fetch_cart_data_file(self_cart);
// //    data->buffer = mem->begin();
// //    data->buffer_len = palette->end - data->buffer;
// //    fs->write_file(data);

//     rose_file* cart_data = rose_fs_fetch_cart_data_file(self_cart);
//     if (cart_data == NULL) {
//         fprintf(stderr, "ERROR: no data file found\n");
//         return false;
//     }

//     if (!cart_data->in_mem) {
//         auto err = fs->read_file(cart_data);
//         if (err == ROSE_FS_CRITICAL_ERR) {
//             return false;
//         }
//     }

//     if (cart_data->buffer_len > (mem->size() - (meta.hd ? ROSE_HD_RUNTIME_RESERVED_MEMORY_SIZE : ROSE_RUNTIME_RESERVED_MEMORY_SIZE))) {
//         fprintf(stderr, "ERROR: tried to reload runtime and cartridge memory size was bigger than available memory size\n");
//         return false;
//     }
//     memcpy(mem->data(), cart_data->buffer, cart_data->buffer_len);

//     rose_file* main = rose_fs_fetch_cart_js_main(self_cart);
//     if (main == NULL) {
//         fprintf(stderr, "ERROR: no main file found\n");
//         return false;
//     }
//     js->include_path.clear();
//     js->include_path.push_back(main);

//     if (!main->in_mem) {
//         fs->read_file(main);
//     }

//     if (main->buffer[main->buffer_len-1] != '\0') {
//         main->buffer_len++;
//         main->buffer = (uint8_t*) realloc(main->buffer, main->buffer_len);
//         main->buffer[main->buffer_len-1] = '\0';
//         main->last_modification = time(NULL);
//     }

//     js->context.Reset(isolate, context);
//     if (context.IsEmpty()) {
//         fprintf(stderr, "context was empty in load run main\n");
//     }
//     v8::Context::Scope context_scope(context);

//     v8::Local<v8::String> file_name = v8::String::NewFromUtf8(isolate, main->name.c_str(), v8::NewStringType::kNormal).ToLocalChecked();
//     v8::Local<v8::String> source;
//     if (!v8::String::NewFromUtf8(isolate, (const char*) main->buffer, v8::NewStringType::kNormal).ToLocal(&source)) {
//         ReportException(isolate, &try_catch, error_cb);
//         return false;
//     }

//     Local<Map> module_cache = Map::New(isolate);
//     js->module_cache.Reset(isolate, module_cache);

//     bool failed;
//     auto res = ExecuteString(isolate, source, file_name, true, &failed, error_cb);
//     if (failed) {
//         ReportException(isolate, &try_catch, error_cb);
//         return false;
//     }

//     while (v8::platform::PumpMessageLoop(static_platform, isolate))
//         continue;

//     return true;
// }

// void rose_vm::retarget(rose_file* self, rose_file* target) {
//     self_cart = self;
//     target_cart = target;
// }

// bool rose_vm::rose_call( const char* name, uint8_t nargs, Local<Value>* args) {
//     auto isolate = js->isolate;
//     v8::Isolate::Scope isolate_scope(isolate);
//     v8::HandleScope handle_scope(isolate);
//     v8::Local<Context> context = js->context.Get(isolate);
//     if (context.IsEmpty()) {
//         error_cb("Error: Context was empty in rose_call.\n");
//         return false;
//     }
//     v8::Context::Scope context_scope(context);
//     v8::TryCatch try_catch(isolate);
//     Local<Object> jsGlobal = context->Global();
//     Handle<Value> value = jsGlobal->Get(String::NewFromUtf8(isolate, name));
//     Handle<Function> jsFunc = Handle<Function>::Cast(value);
//     if (!jsFunc->IsFunction()) {
//         return true;
//     }
//     v8::Local<v8::Value> result;
//     if (!jsFunc->Call(context, jsGlobal, nargs, args).ToLocal(&result)) {
//         assert(try_catch.HasCaught());
//         ReportException(isolate, &try_catch, error_cb);
//         return false;
//     }
//     assert(!try_catch.HasCaught());
//     return true;
// }

// bool rose_vm::call_init() {
//     return rose_call("_init", 0, NULL);
// }

// bool rose_vm::call_update() {
//     return rose_call("_update", 0, NULL);
// }

// bool rose_vm::call_draw() {
//     return rose_call("_draw", 0, NULL);
// }

// bool rose_vm::call_onmouse(int16_t x, int16_t y) {
//     auto isolate = js->isolate;
//     v8::HandleScope handle_scope(isolate);
//     Local<Value> args[2] = {Int32::New(isolate,(int16_t) x), Int32::New(isolate,(int16_t) y)};
//     return rose_call("_onmouse", 2, args);
// }

// bool rose_vm::call_onwheel(int16_t x, int16_t y, bool inverted) {
//     auto isolate = js->isolate;
//     v8::HandleScope handle_scope(isolate);
//     Local<Value> args[3] = {Int32::New(isolate, x), Int32::New(isolate, y), Boolean::New(isolate, inverted)};
//     return rose_call("_onwheel", 3, args);
// }

// bool rose_vm::call_onbtn(uint8_t code, bool pressed) {
//     auto isolate = js->isolate;
//     v8::HandleScope handle_scope(isolate);
//     Local<Value> args[2] = {Int32::New(isolate, code), Boolean::New(isolate, pressed)};
//     return rose_call("_onbtn", 2, args);
// }

// bool rose_vm::call_onkey(rose_keycode keycode, bool pressed, bool repeat) {
//     auto isolate = js->isolate;
//     v8::HandleScope handle_scope(isolate);
//     char character = rose_keycode_to_printable_char(keycode, rose_get_bit(key_states.begin, ROSE_KEYCODE_LSHIFT) || rose_get_bit(key_states.begin, ROSE_KEYCODE_RSHIFT));
//     char str[2];
//     str[0] = character;
//     str[1] = '\0';
// //    printf("%c\n", character);
//     Local<Value> args[4] = {
//             Int32::New(isolate, keycode),
//             Boolean::New(isolate, pressed),
//             Boolean::New(isolate, repeat),
//             String::NewFromUtf8(isolate, str)
//     };
//     return rose_call("_onkey", 4, args);
// }

// bool rose_vm::call_ontouch() {
//     // TODO: make this actually do something
//     return rose_call("_ontouch", 0, NULL);
// }

void rose_set_bit(uint8_t* arr, uint8_t addr, bool val)
{
    uint8_t idx = (uint8_t)(addr / 8);
    uint8_t bit = (uint8_t)(addr % 8);
    if (val)
    {
        arr[idx] |= 1 << bit;
    }
    else
    {
        arr[idx] &= ~(1 << bit);
    }
}

bool rose_get_bit(uint8_t* arr, uint8_t addr)
{
    uint8_t idx = (uint8_t)(addr / 8);
    uint8_t bit = (uint8_t)(addr % 8);
    return (bool)((arr[idx] >> bit) & 1);
}
