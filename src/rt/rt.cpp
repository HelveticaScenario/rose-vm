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

rose_rt::rose_rt(rose_fs* fs) {
    if (fs == NULL) {
        fprintf(stderr, "tried to create runtime base with null fs\n");
        exit(1);
    }

    std::array<uint8_t, ROSE_MEMORY_SIZE>* mem = new std::array<uint8_t, ROSE_MEMORY_SIZE>();

    rose_memory_range* screen_range = new rose_memory_range;
    auto end = mem->end();
    auto beg_screen = end - (ROSE_SCREEN_SIZE);
    screen_range->begin = beg_screen;
    screen_range->end = end;

    rose_memory_range* schema_range = new rose_memory_range;
    auto beg_schema = end - ROSE_RUNTIME_RESERVED_MEMORY_SIZE - ROSE_PALETTE_SIZE - ROSE_MEMORY_SCHEMA_SIZE;
    auto end_schema = beg_schema + ROSE_MEMORY_SCHEMA_SIZE;
    schema_range->begin = beg_schema;
    schema_range->end = end_schema;

    rose_memory_range* palette_range = new rose_memory_range;
    auto beg_palette = end_schema;
    auto end_palette = beg_palette + ROSE_PALETTE_SIZE;
    palette_range->begin = beg_palette;
    palette_range->end = end_palette;

    rose_memory_range* palette_filter_range = new rose_memory_range;
    auto beg_palette_filter = end_palette;
    auto end_palette_filter = beg_palette_filter + ROSE_PALETTE_INDEX_NUM;
    palette_filter_range->begin = beg_palette_filter;
    palette_filter_range->end = end_palette_filter;

    int i;
    for (i = 0; i < ROSE_PALETTE_INDEX_NUM; ++i) {
        beg_palette_filter[i] = (uint8_t) i;
    }

    rose_memory_range* palette_transparency_range = new rose_memory_range;
    auto beg_palette_transparency = end_palette_filter;
    auto end_palette_transparency = beg_palette_transparency + (ROSE_PALETTE_INDEX_NUM / 8);
    palette_transparency_range->begin = beg_palette_transparency;
    palette_transparency_range->end = end_palette_transparency;

    rose_set_bit(beg_palette_transparency, 0, true);

    rose_memory_range* clipping_region_range = new rose_memory_range;
    auto beg_clipping_region = end_palette_transparency;
    auto end_clipping_region = beg_clipping_region + 8;
    clipping_region_range->begin = beg_clipping_region;
    clipping_region_range->end = end_clipping_region;

    uint16_t* clipping_region = (uint16_t*) beg_clipping_region;
    clipping_region[0] = 0;                      // x0
    clipping_region[1] = 0;                      // y0
    clipping_region[2] = ROSE_SCREEN_WIDTH - 1;  // x1
    clipping_region[3] = ROSE_SCREEN_HEIGHT - 1; // y1

    auto pen_color_addr = end_clipping_region;
    *pen_color_addr = 6;

    rose_memory_range* print_cursor_range = new rose_memory_range;
    auto beg_print_cursor = pen_color_addr + 1;
    auto end_print_cursor = beg_print_cursor + 4;
    print_cursor_range->begin = beg_print_cursor;
    print_cursor_range->end = end_print_cursor;

    uint16_t* print_cursor = (uint16_t*) beg_print_cursor;
    print_cursor[0] = 0; // x0
    print_cursor[1] = 0; // y0 // TODO: replace with actual starting position
    // once font size is finalized

    rose_memory_range* camera_offset_range = new rose_memory_range;
    auto beg_camera_offset = end_print_cursor;
    auto end_camera_offset = beg_camera_offset + 4;
    camera_offset_range->begin = beg_camera_offset;
    camera_offset_range->end = end_camera_offset;

    int16_t* camera_offset = (int16_t*) beg_camera_offset;
    camera_offset[0] = 0; // x0
    camera_offset[1] = 0; // y0 // TODO: replace with actual starting position
    // once font size is finalized

    rose_memory_range* pointer_positions_range = new rose_memory_range;
    auto beg_pointer_positions = end_camera_offset;
    auto end_pointer_positions = beg_pointer_positions + (11 * 4 /* 16 bit number */);
    pointer_positions_range->begin = beg_pointer_positions;
    pointer_positions_range->end = end_pointer_positions;

    rose_memory_range* btn_states_range = new rose_memory_range;
    auto beg_btn_states = end_pointer_positions;
    auto end_btn_states = beg_btn_states + 4 /* 32 bit fields */;
    btn_states_range->begin = beg_btn_states;
    btn_states_range->end = end_btn_states;

    rose_memory_range* prev_btn_states_range = new rose_memory_range;
    auto beg_prev_btn_states = end_btn_states;
    auto end_prev_btn_states = beg_prev_btn_states + 4 /* 32 bit fields */;
    prev_btn_states_range->begin = beg_prev_btn_states;
    prev_btn_states_range->end = end_prev_btn_states;

    rose_memory_range* mouse_wheel_range = new rose_memory_range;
    auto beg_mouse_wheel = end_prev_btn_states;
    auto end_mouse_wheel = beg_mouse_wheel + 5 /* 2 16 bit ints and one bool */;
    mouse_wheel_range->begin = beg_mouse_wheel;
    mouse_wheel_range->end = end_mouse_wheel;

    rose_memory_range* key_states_range = new rose_memory_range;
    auto beg_key_states = end_mouse_wheel;
    auto end_key_states = beg_key_states + 30 /* 240 bit fields */;
    key_states_range->begin = beg_key_states;
    key_states_range->end = end_key_states;

    rose_memory_range* prev_key_states_range = new rose_memory_range;
    auto beg_prev_key_states = end_key_states;
    auto end_prev_key_states = beg_prev_key_states + 30 /* 240 bit fields */;
    prev_key_states_range->begin = beg_prev_key_states;
    prev_key_states_range->end = end_prev_key_states;

    this->fs = fs;
    this->mem = mem;
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

    this->js = rose_js_create(this);
}

rose_rt::~rose_rt() {
    rose_js_free(this->js);
    // dont free fs, managed by system layer
    delete this->mem;
    free(this->screen);
    free(this->schema);
    free(this->palette);
    free(this->palette_filter);
    free(this->palette_transparency);
    free(this->clipping_region);
    free(this->print_cursor);
    free(this->camera_offset);
    free(this->pointer_positions);
    free(this->btn_states);
    free(this->mouse_wheel);
    free(this->key_states);
}

bool rose_rt::clear() {
    this->mem->fill(0);
    this->js->module_cache.Reset();
    this->js->context.Reset();
    return true;
}


bool rose_rt::load_run_main() {
    if (this->fs->cart == NULL) {
        return false;
    }

    rose_file* cart_data = rose_fs_fetch_cart_data_file(this->fs->cart);
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

    if (cart_data->buffer_len > (this->mem->size() - ROSE_RUNTIME_RESERVED_MEMORY_SIZE)) {
        fprintf(stderr, "ERROR: tried to reload runtime and cartridge memory size was bigger than available memory size\n");
        return false;
    }
    memcpy(this->mem->data(), cart_data->buffer, cart_data->buffer_len);
    rose_file* main = rose_fs_fetch_cart_js_main(this->fs->cart);
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

    auto isolate = this->js->isolate;
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);
    v8::TryCatch try_catch(isolate);

    Local<Context> context = Context::New(isolate, NULL, this->js->global_template.Get(isolate));

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


