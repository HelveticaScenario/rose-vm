#include <cassert>
#include "rt/rt.h"

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

rose_memory_iterator rose_memory_iterator_begin(uint8_t m[]) { return &m[0]; }

rose_memory_iterator rose_memory_iterator_end(uint8_t m[], uint32_t len) { return &m[len]; }

rose_memory_iterator rose_memory_iterator_next(rose_memory_iterator i) { return ++i; }

rose_rt* rose_rt_create(rose_fs* fs) {
    if (fs == NULL) {
        fprintf(stderr, "tried to create runtime base with null fs\n");
        exit(1);
    }

    uint8_t* mem = (uint8_t*) malloc(ROSE_MEMORY_SIZE);
    memset(mem, 0, ROSE_MEMORY_SIZE);

    rose_memory_range* screen_range = (rose_memory_range*) malloc(sizeof(rose_memory_range));
    rose_memory_iterator end = rose_memory_iterator_end(mem, ROSE_MEMORY_SIZE);
    rose_memory_iterator beg_screen = end - (ROSE_SCREEN_SIZE);
    screen_range->begin = beg_screen;
    screen_range->end = end;

    rose_memory_range* schema_range = (rose_memory_range*) malloc(sizeof(rose_memory_range));
    rose_memory_iterator beg_schema = end - ROSE_RUNTIME_RESERVED_MEMORY_SIZE - ROSE_PALETTE_SIZE - ROSE_MEMORY_SCHEMA_SIZE;
    rose_memory_iterator end_schema = beg_schema + ROSE_MEMORY_SCHEMA_SIZE;
    schema_range->begin = beg_schema;
    schema_range->end = end_schema;

    rose_memory_range* palette_range = (rose_memory_range*) malloc(sizeof(rose_memory_range));
    rose_memory_iterator beg_palette = end_schema;
    rose_memory_iterator end_palette = beg_palette + ROSE_PALETTE_SIZE;
    palette_range->begin = beg_palette;
    palette_range->end = end_palette;

    rose_memory_range* palette_filter_range = (rose_memory_range*) malloc(sizeof(rose_memory_range));
    rose_memory_iterator beg_palette_filter = end_palette;
    rose_memory_iterator end_palette_filter = beg_palette_filter + ROSE_PALETTE_INDEX_NUM;
    palette_filter_range->begin = beg_palette_filter;
    palette_filter_range->end = end_palette_filter;

    int i;
    for (i = 0; i < ROSE_PALETTE_INDEX_NUM; ++i) {
        beg_palette_filter[i] = (uint8_t) i;
    }

    rose_memory_range* palette_transparency_range = (rose_memory_range*) malloc(sizeof(rose_memory_range));
    rose_memory_iterator beg_palette_transparency = end_palette_filter;
    rose_memory_iterator end_palette_transparency = beg_palette_transparency + (ROSE_PALETTE_INDEX_NUM / 8);
    palette_transparency_range->begin = beg_palette_transparency;
    palette_transparency_range->end = end_palette_transparency;

    rose_set_bit(beg_palette_transparency, 0, true);

    rose_memory_range* clipping_region_range = (rose_memory_range*) malloc(sizeof(rose_memory_range));
    rose_memory_iterator beg_clipping_region = end_palette_transparency;
    rose_memory_iterator end_clipping_region = beg_clipping_region + 8;
    clipping_region_range->begin = beg_clipping_region;
    clipping_region_range->end = end_clipping_region;

    uint16_t* clipping_region = (uint16_t*) beg_clipping_region;
    clipping_region[0] = 0;                      // x0
    clipping_region[1] = 0;                      // y0
    clipping_region[2] = ROSE_SCREEN_WIDTH - 1;  // x1
    clipping_region[3] = ROSE_SCREEN_HEIGHT - 1; // y1

    rose_memory_iterator pen_color_addr = end_clipping_region;
    *pen_color_addr = 6;

    rose_memory_range* print_cursor_range = (rose_memory_range*) malloc(sizeof(rose_memory_range));
    rose_memory_iterator beg_print_cursor = pen_color_addr + 1;
    rose_memory_iterator end_print_cursor = beg_print_cursor + 4;
    print_cursor_range->begin = beg_print_cursor;
    print_cursor_range->end = end_print_cursor;

    uint16_t* print_cursor = (uint16_t*) beg_print_cursor;
    print_cursor[0] = 0; // x0
    print_cursor[1] = 0; // y0 // TODO: replace with actual starting position
    // once font size is finalized

    rose_memory_range* camera_offset_range = (rose_memory_range*) malloc(sizeof(rose_memory_range));
    rose_memory_iterator beg_camera_offset = end_print_cursor;
    rose_memory_iterator end_camera_offset = beg_camera_offset + 4;
    camera_offset_range->begin = beg_camera_offset;
    camera_offset_range->end = end_camera_offset;

    int16_t* camera_offset = (int16_t*) beg_camera_offset;
    camera_offset[0] = 0; // x0
    camera_offset[1] = 0; // y0 // TODO: replace with actual starting position
    // once font size is finalized

    rose_memory_range* pointer_positions_range = (rose_memory_range*) malloc(sizeof(rose_memory_range));
    rose_memory_iterator beg_pointer_positions = end_camera_offset;
    rose_memory_iterator end_pointer_positions = beg_pointer_positions + (11 * 4 /* 16 bit number */);
    pointer_positions_range->begin = beg_pointer_positions;
    pointer_positions_range->end = end_pointer_positions;

    rose_memory_range* btn_states_range = (rose_memory_range*) malloc(sizeof(rose_memory_range));
    rose_memory_iterator beg_btn_states = end_pointer_positions;
    rose_memory_iterator end_btn_states = beg_btn_states + 4 /* 32 bit fields */;
    btn_states_range->begin = beg_btn_states;
    btn_states_range->end = end_btn_states;

    rose_memory_range* prev_btn_states_range = (rose_memory_range*) malloc(sizeof(rose_memory_range));
    rose_memory_iterator beg_prev_btn_states = end_btn_states;
    rose_memory_iterator end_prev_btn_states = beg_prev_btn_states + 4 /* 32 bit fields */;
    prev_btn_states_range->begin = beg_prev_btn_states;
    prev_btn_states_range->end = end_prev_btn_states;

    rose_memory_range* mouse_wheel_range = (rose_memory_range*) malloc(sizeof(rose_memory_range));
    rose_memory_iterator beg_mouse_wheel = end_prev_btn_states;
    rose_memory_iterator end_mouse_wheel = beg_mouse_wheel + 5 /* 2 16 bit ints and one bool */;
    mouse_wheel_range->begin = beg_mouse_wheel;
    mouse_wheel_range->end = end_mouse_wheel;

    rose_memory_range* key_states_range = (rose_memory_range*) malloc(sizeof(rose_memory_range));
    rose_memory_iterator beg_key_states = end_mouse_wheel;
    rose_memory_iterator end_key_states = beg_key_states + 30 /* 240 bit fields */;
    key_states_range->begin = beg_key_states;
    key_states_range->end = end_key_states;

    rose_memory_range* prev_key_states_range = (rose_memory_range*) malloc(sizeof(rose_memory_range));
    rose_memory_iterator beg_prev_key_states = end_key_states;
    rose_memory_iterator end_prev_key_states = beg_prev_key_states + 30 /* 240 bit fields */;
    prev_key_states_range->begin = beg_prev_key_states;
    prev_key_states_range->end = end_prev_key_states;

    rose_rt* r = (rose_rt*) malloc(sizeof(rose_rt));

    r->fs = fs;
    r->mem = mem;
    r->mem_size = ROSE_MEMORY_SIZE;
    r->screen = screen_range;
    r->schema = schema_range;
    r->palette = palette_range;
    r->palette_filter = palette_filter_range;
    r->palette_transparency = palette_transparency_range;
    r->clipping_region = clipping_region_range;
    r->pen_color_addr = pen_color_addr;
    r->print_cursor = print_cursor_range;
    r->camera_offset = camera_offset_range;
    r->pointer_positions = pointer_positions_range;
    r->btn_states = btn_states_range;
    r->prev_btn_states = prev_btn_states_range;
    r->mouse_wheel = mouse_wheel_range;
    r->key_states = key_states_range;
    r->prev_key_states = prev_key_states_range;

    r->js = rose_js_create(r);
    return r;
}

bool rose_rt_clear(rose_rt* r) {
    memset(r->mem, 0, r->mem_size);
    r->js->module_cache.Reset();
    r->js->context.Reset();
    return true;
}


bool rose_rt_load_run_main(rose_rt* r) {
    if (r->fs->cart == NULL) {
        return false;
    }

    rose_file* cart_data = rose_fs_fetch_cart_data_file(r->fs->cart);
    if (cart_data == NULL) {
        fprintf(stderr, "ERROR: no data file found\n");
        return false;
    }

    if (!cart_data->in_mem) {
        auto err = r->fs->read_file(cart_data);
        if (err == ROSE_FS_CRITICAL_ERR) {
            return false;
        }
    }

    if (cart_data->buffer_len > (r->mem_size - ROSE_RUNTIME_RESERVED_MEMORY_SIZE)) {
        fprintf(stderr, "ERROR: tried to reload runtime and cartridge memory size was bigger than available memory size\n");
        return false;
    }
    memcpy(r->mem, cart_data->buffer, cart_data->buffer_len);
    rose_file* main = rose_fs_fetch_cart_js_main(r->fs->cart);
    if (main == NULL) {
        fprintf(stderr, "ERROR: no main file found\n");
        return false;
    }
    r->js->include_path.clear();
    r->js->include_path.push_back(main);

    if (!main->in_mem) {
        r->fs->read_file(main);
    }


    if (main->buffer[main->buffer_len-1] != '\0') {
        main->buffer_len++;
        main->buffer = (uint8_t*) realloc(main->buffer, main->buffer_len);
        main->buffer[main->buffer_len-1] = '\0';
        main->last_modification = time(NULL);
    }

    auto isolate = r->js->isolate;
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);
    v8::TryCatch try_catch(isolate);

    Local<Context> context = Context::New(isolate, NULL, r->js->global_template.Get(isolate));

    r->js->context.Reset(isolate, context);
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
    r->js->module_cache.Reset(isolate, module_cache);

    bool failed;
    auto res = ExecuteString(isolate, source, file_name, true, &failed);
    if (failed) {
        ReportException(isolate, &try_catch);
        return false;
    }

//    auto main_path = rose_construct_path(main);
//    module_cache->Set(context, v8::String::NewFromUtf8(isolate, (const char*) main_path, v8::NewStringType::kNormal).ToLocalChecked(), res);
//    free(main_path);

//    auto thing = r->js->module_cache.Get(isolate);
    while (v8::platform::PumpMessageLoop(static_platform, isolate))
        continue;

    return true;
}

void rose_rt_free(rose_rt* r) {
    rose_js_free(r->js);
    // dont free fs, managed by system layer
    free(r->mem);
    free(r->screen);
    free(r->schema);
    free(r->palette);
    free(r->palette_filter);
    free(r->palette_transparency);
    free(r->clipping_region);
    free(r->print_cursor);
    free(r->camera_offset);
    free(r->pointer_positions);
    free(r->btn_states);
    free(r->mouse_wheel);
    free(r->key_states);
    free(r);
}

void rose_rt_save_input_frame(rose_rt* r) {
    memcpy(r->prev_btn_states->begin, r->btn_states->begin, r->prev_btn_states->end - r->prev_btn_states->begin);
    memcpy(r->prev_key_states->begin, r->key_states->begin, r->prev_key_states->end - r->prev_key_states->begin);
}

void rose_rt_update_mousestate(rose_rt* r, const rose_mousestate* mousestate) {
    int16_t* pointer = (int16_t*) r->pointer_positions->begin;
    pointer[20] = mousestate->x;
    pointer[21] = mousestate->y;

    rose_set_bit(r->btn_states->begin, ROSE_LEFT_MOUSE_IDX, mousestate->left_btn_down);
    rose_set_bit(r->btn_states->begin, ROSE_RIGHT_MOUSE_IDX, mousestate->right_btn_down);
    rose_set_bit(r->btn_states->begin, ROSE_MIDDLE_MOUSE_IDX, mousestate->middle_btn_down);
    rose_set_bit(r->btn_states->begin, ROSE_X1_MOUSE_IDX, mousestate->x1_btn_down);
    rose_set_bit(r->btn_states->begin, ROSE_X2_MOUSE_IDX, mousestate->x2_btn_down);

    int16_t* wheel_delta = (int16_t*) r->mouse_wheel->begin;
    wheel_delta[0] = mousestate->wheel_x;
    wheel_delta[1] = mousestate->wheel_y;

    bool* wheel_inverted = (bool*) (r->mouse_wheel->begin + 4);
    *wheel_inverted = mousestate->wheel_inverted;
}

void rose_rt_update_keystate(rose_rt* r, rose_keycode keycode, bool pressed) {
    if (keycode < ROSE_KEYCODE_UNKNOWN) {
        rose_set_bit(r->key_states->begin, keycode, pressed);
    }
}

void rose_rt_reset_input(rose_rt* r, rose_mousestate* mousestate) {
    mousestate->left_btn_down = false;
    mousestate->right_btn_down = false;
    mousestate->middle_btn_down = false;
    mousestate->x1_btn_down = false;
    mousestate->x2_btn_down = false;
    mousestate->wheel_x = 0;
    mousestate->wheel_y = 0;

    rose_rt_update_mousestate(r, mousestate);
    for (int keycode = ROSE_KEYCODE_A; keycode < ROSE_KEYCODE_UNKNOWN; ++keycode) {
        rose_rt_update_keystate(r, (rose_keycode) keycode, false);
    }
    rose_rt_save_input_frame(r);
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

const char* rose_keycode_to_string(rose_keycode key) {
    switch (key) {
        case ROSE_KEYCODE_A:
            return "a";
            break;
        case ROSE_KEYCODE_B:
            return "b";
            break;
        case ROSE_KEYCODE_C:
            return "c";
            break;
        case ROSE_KEYCODE_D:
            return "d";
            break;
        case ROSE_KEYCODE_E:
            return "e";
            break;
        case ROSE_KEYCODE_F:
            return "f";
            break;
        case ROSE_KEYCODE_G:
            return "g";
            break;
        case ROSE_KEYCODE_H:
            return "h";
            break;
        case ROSE_KEYCODE_I:
            return "i";
            break;
        case ROSE_KEYCODE_J:
            return "j";
            break;
        case ROSE_KEYCODE_K:
            return "k";
            break;
        case ROSE_KEYCODE_L:
            return "l";
            break;
        case ROSE_KEYCODE_M:
            return "m";
            break;
        case ROSE_KEYCODE_N:
            return "n";
            break;
        case ROSE_KEYCODE_O:
            return "o";
            break;
        case ROSE_KEYCODE_P:
            return "p";
            break;
        case ROSE_KEYCODE_Q:
            return "q";
            break;
        case ROSE_KEYCODE_R:
            return "r";
            break;
        case ROSE_KEYCODE_S:
            return "s";
            break;
        case ROSE_KEYCODE_T:
            return "t";
            break;
        case ROSE_KEYCODE_U:
            return "u";
            break;
        case ROSE_KEYCODE_V:
            return "v";
            break;
        case ROSE_KEYCODE_W:
            return "w";
            break;
        case ROSE_KEYCODE_X:
            return "x";
            break;
        case ROSE_KEYCODE_Y:
            return "y";
            break;
        case ROSE_KEYCODE_Z:
            return "z";
            break;
        case ROSE_KEYCODE_1:
            return "1";
            break;
        case ROSE_KEYCODE_2:
            return "2";
            break;
        case ROSE_KEYCODE_3:
            return "3";
            break;
        case ROSE_KEYCODE_4:
            return "4";
            break;
        case ROSE_KEYCODE_5:
            return "5";
            break;
        case ROSE_KEYCODE_6:
            return "6";
            break;
        case ROSE_KEYCODE_7:
            return "7";
            break;
        case ROSE_KEYCODE_8:
            return "8";
            break;
        case ROSE_KEYCODE_9:
            return "9";
            break;
        case ROSE_KEYCODE_0:
            return "0";
            break;
        case ROSE_KEYCODE_RETURN:
            return "return";
            break;
        case ROSE_KEYCODE_ESCAPE:
            return "escape";
            break;
        case ROSE_KEYCODE_BACKSPACE:
            return "backspace";
            break;
        case ROSE_KEYCODE_TAB:
            return "tab";
            break;
        case ROSE_KEYCODE_SPACE:
            return "space";
            break;
        case ROSE_KEYCODE_MINUS:
            return "minus";
            break;
        case ROSE_KEYCODE_EQUALS:
            return "equals";
            break;
        case ROSE_KEYCODE_LEFTBRACKET:
            return "leftbracket";
            break;
        case ROSE_KEYCODE_RIGHTBRACKET:
            return "rightbracket";
            break;
        case ROSE_KEYCODE_BACKSLASH:
            return "backslash";
            break;
        case ROSE_KEYCODE_NONUSHASH:
            return "nonushash";
            break;
        case ROSE_KEYCODE_SEMICOLON:
            return "semicolon";
            break;
        case ROSE_KEYCODE_APOSTROPHE:
            return "apostrophe";
            break;
        case ROSE_KEYCODE_GRAVE:
            return "grave";
            break;
        case ROSE_KEYCODE_COMMA:
            return "comma";
            break;
        case ROSE_KEYCODE_PERIOD:
            return "period";
            break;
        case ROSE_KEYCODE_SLASH:
            return "slash";
            break;
        case ROSE_KEYCODE_CAPSLOCK:
            return "capslock";
            break;
        case ROSE_KEYCODE_F1:
            return "f1";
            break;
        case ROSE_KEYCODE_F2:
            return "f2";
            break;
        case ROSE_KEYCODE_F3:
            return "f3";
            break;
        case ROSE_KEYCODE_F4:
            return "f4";
            break;
        case ROSE_KEYCODE_F5:
            return "f5";
            break;
        case ROSE_KEYCODE_F6:
            return "f6";
            break;
        case ROSE_KEYCODE_F7:
            return "f7";
            break;
        case ROSE_KEYCODE_F8:
            return "f8";
            break;
        case ROSE_KEYCODE_F9:
            return "f9";
            break;
        case ROSE_KEYCODE_F10:
            return "f10";
            break;
        case ROSE_KEYCODE_F11:
            return "f11";
            break;
        case ROSE_KEYCODE_F12:
            return "f12";
            break;
        case ROSE_KEYCODE_PRINTSCREEN:
            return "printscreen";
            break;
        case ROSE_KEYCODE_SCROLLLOCK:
            return "scrolllock";
            break;
        case ROSE_KEYCODE_PAUSE:
            return "pause";
            break;
        case ROSE_KEYCODE_INSERT:
            return "insert";
            break;
        case ROSE_KEYCODE_HOME:
            return "home";
            break;
        case ROSE_KEYCODE_PAGEUP:
            return "pageup";
            break;
        case ROSE_KEYCODE_DELETE:
            return "delete";
            break;
        case ROSE_KEYCODE_END:
            return "end";
            break;
        case ROSE_KEYCODE_PAGEDOWN:
            return "pagedown";
            break;
        case ROSE_KEYCODE_RIGHT:
            return "right";
            break;
        case ROSE_KEYCODE_LEFT:
            return "left";
            break;
        case ROSE_KEYCODE_DOWN:
            return "down";
            break;
        case ROSE_KEYCODE_UP:
            return "up";
            break;
        case ROSE_KEYCODE_NUMLOCKCLEAR:
            return "numlockclear";
            break;
        case ROSE_KEYCODE_KP_DIVIDE:
            return "kp_divide";
            break;
        case ROSE_KEYCODE_KP_MULTIPLY:
            return "kp_multiply";
            break;
        case ROSE_KEYCODE_KP_MINUS:
            return "kp_minus";
            break;
        case ROSE_KEYCODE_KP_PLUS:
            return "kp_plus";
            break;
        case ROSE_KEYCODE_KP_ENTER:
            return "kp_enter";
            break;
        case ROSE_KEYCODE_KP_1:
            return "kp_1";
            break;
        case ROSE_KEYCODE_KP_2:
            return "kp_2";
            break;
        case ROSE_KEYCODE_KP_3:
            return "kp_3";
            break;
        case ROSE_KEYCODE_KP_4:
            return "kp_4";
            break;
        case ROSE_KEYCODE_KP_5:
            return "kp_5";
            break;
        case ROSE_KEYCODE_KP_6:
            return "kp_6";
            break;
        case ROSE_KEYCODE_KP_7:
            return "kp_7";
            break;
        case ROSE_KEYCODE_KP_8:
            return "kp_8";
            break;
        case ROSE_KEYCODE_KP_9:
            return "kp_9";
            break;
        case ROSE_KEYCODE_KP_0:
            return "kp_0";
            break;
        case ROSE_KEYCODE_KP_PERIOD:
            return "kp_period";
            break;
        case ROSE_KEYCODE_NONUSBACKSLASH:
            return "nonusbackslash";
            break;
        case ROSE_KEYCODE_APPLICATION:
            return "application";
            break;
        case ROSE_KEYCODE_POWER:
            return "power";
            break;
        case ROSE_KEYCODE_KP_EQUALS:
            return "kp_equals";
            break;
        case ROSE_KEYCODE_F13:
            return "f13";
            break;
        case ROSE_KEYCODE_F14:
            return "f14";
            break;
        case ROSE_KEYCODE_F15:
            return "f15";
            break;
        case ROSE_KEYCODE_F16:
            return "f16";
            break;
        case ROSE_KEYCODE_F17:
            return "f17";
            break;
        case ROSE_KEYCODE_F18:
            return "f18";
            break;
        case ROSE_KEYCODE_F19:
            return "f19";
            break;
        case ROSE_KEYCODE_F20:
            return "f20";
            break;
        case ROSE_KEYCODE_F21:
            return "f21";
            break;
        case ROSE_KEYCODE_F22:
            return "f22";
            break;
        case ROSE_KEYCODE_F23:
            return "f23";
            break;
        case ROSE_KEYCODE_F24:
            return "f24";
            break;
        case ROSE_KEYCODE_EXECUTE:
            return "execute";
            break;
        case ROSE_KEYCODE_HELP:
            return "help";
            break;
        case ROSE_KEYCODE_MENU:
            return "menu";
            break;
        case ROSE_KEYCODE_SELECT:
            return "select";
            break;
        case ROSE_KEYCODE_STOP:
            return "stop";
            break;
        case ROSE_KEYCODE_AGAIN:
            return "again";
            break;
        case ROSE_KEYCODE_UNDO:
            return "undo";
            break;
        case ROSE_KEYCODE_CUT:
            return "cut";
            break;
        case ROSE_KEYCODE_COPY:
            return "copy";
            break;
        case ROSE_KEYCODE_PASTE:
            return "paste";
            break;
        case ROSE_KEYCODE_FIND:
            return "find";
            break;
        case ROSE_KEYCODE_MUTE:
            return "mute";
            break;
        case ROSE_KEYCODE_VOLUMEUP:
            return "volumeup";
            break;
        case ROSE_KEYCODE_VOLUMEDOWN:
            return "volumedown";
            break;
        case ROSE_KEYCODE_KP_COMMA:
            return "kp_comma";
            break;
        case ROSE_KEYCODE_KP_EQUALSAS400:
            return "kp_equalsas400";
            break;
        case ROSE_KEYCODE_INTERNATIONAL1:
            return "international1";
            break;
        case ROSE_KEYCODE_INTERNATIONAL2:
            return "international2";
            break;
        case ROSE_KEYCODE_INTERNATIONAL3:
            return "international3";
            break;
        case ROSE_KEYCODE_INTERNATIONAL4:
            return "international4";
            break;
        case ROSE_KEYCODE_INTERNATIONAL5:
            return "international5";
            break;
        case ROSE_KEYCODE_INTERNATIONAL6:
            return "international6";
            break;
        case ROSE_KEYCODE_INTERNATIONAL7:
            return "international7";
            break;
        case ROSE_KEYCODE_INTERNATIONAL8:
            return "international8";
            break;
        case ROSE_KEYCODE_INTERNATIONAL9:
            return "international9";
            break;
        case ROSE_KEYCODE_LANG1:
            return "lang1";
            break;
        case ROSE_KEYCODE_LANG2:
            return "lang2";
            break;
        case ROSE_KEYCODE_LANG3:
            return "lang3";
            break;
        case ROSE_KEYCODE_LANG4:
            return "lang4";
            break;
        case ROSE_KEYCODE_LANG5:
            return "lang5";
            break;
        case ROSE_KEYCODE_LANG6:
            return "lang6";
            break;
        case ROSE_KEYCODE_LANG7:
            return "lang7";
            break;
        case ROSE_KEYCODE_LANG8:
            return "lang8";
            break;
        case ROSE_KEYCODE_LANG9:
            return "lang9";
            break;
        case ROSE_KEYCODE_ALTERASE:
            return "alterase";
            break;
        case ROSE_KEYCODE_SYSREQ:
            return "sysreq";
            break;
        case ROSE_KEYCODE_CANCEL:
            return "cancel";
            break;
        case ROSE_KEYCODE_CLEAR:
            return "clear";
            break;
        case ROSE_KEYCODE_PRIOR:
            return "prior";
            break;
        case ROSE_KEYCODE_RETURN2:
            return "return2";
            break;
        case ROSE_KEYCODE_SEPARATOR:
            return "separator";
            break;
        case ROSE_KEYCODE_OUT:
            return "out";
            break;
        case ROSE_KEYCODE_OPER:
            return "oper";
            break;
        case ROSE_KEYCODE_CLEARAGAIN:
            return "clearagain";
            break;
        case ROSE_KEYCODE_CRSEL:
            return "crsel";
            break;
        case ROSE_KEYCODE_EXSEL:
            return "exsel";
            break;
        case ROSE_KEYCODE_KP_00:
            return "kp_00";
            break;
        case ROSE_KEYCODE_KP_000:
            return "kp_000";
            break;
        case ROSE_KEYCODE_THOUSANDSSEPARATOR:
            return "thousandsseparator";
            break;
        case ROSE_KEYCODE_DECIMALSEPARATOR:
            return "decimalseparator";
            break;
        case ROSE_KEYCODE_CURRENCYUNIT:
            return "currencyunit";
            break;
        case ROSE_KEYCODE_CURRENCYSUBUNIT:
            return "currencysubunit";
            break;
        case ROSE_KEYCODE_KP_LEFTPAREN:
            return "kp_leftparen";
            break;
        case ROSE_KEYCODE_KP_RIGHTPAREN:
            return "kp_rightparen";
            break;
        case ROSE_KEYCODE_KP_LEFTBRACE:
            return "kp_leftbrace";
            break;
        case ROSE_KEYCODE_KP_RIGHTBRACE:
            return "kp_rightbrace";
            break;
        case ROSE_KEYCODE_KP_TAB:
            return "kp_tab";
            break;
        case ROSE_KEYCODE_KP_BACKSPACE:
            return "kp_backspace";
            break;
        case ROSE_KEYCODE_KP_A:
            return "kp_a";
            break;
        case ROSE_KEYCODE_KP_B:
            return "kp_b";
            break;
        case ROSE_KEYCODE_KP_C:
            return "kp_c";
            break;
        case ROSE_KEYCODE_KP_D:
            return "kp_d";
            break;
        case ROSE_KEYCODE_KP_E:
            return "kp_e";
            break;
        case ROSE_KEYCODE_KP_F:
            return "kp_f";
            break;
        case ROSE_KEYCODE_KP_XOR:
            return "kp_xor";
            break;
        case ROSE_KEYCODE_KP_POWER:
            return "kp_power";
            break;
        case ROSE_KEYCODE_KP_PERCENT:
            return "kp_percent";
            break;
        case ROSE_KEYCODE_KP_LESS:
            return "kp_less";
            break;
        case ROSE_KEYCODE_KP_GREATER:
            return "kp_greater";
            break;
        case ROSE_KEYCODE_KP_AMPERSAND:
            return "kp_ampersand";
            break;
        case ROSE_KEYCODE_KP_DBLAMPERSAND:
            return "kp_dblampersand";
            break;
        case ROSE_KEYCODE_KP_VERTICALBAR:
            return "kp_verticalbar";
            break;
        case ROSE_KEYCODE_KP_DBLVERTICALBAR:
            return "kp_dblverticalbar";
            break;
        case ROSE_KEYCODE_KP_COLON:
            return "kp_colon";
            break;
        case ROSE_KEYCODE_KP_HASH:
            return "kp_hash";
            break;
        case ROSE_KEYCODE_KP_SPACE:
            return "kp_space";
            break;
        case ROSE_KEYCODE_KP_AT:
            return "kp_at";
            break;
        case ROSE_KEYCODE_KP_EXCLAM:
            return "kp_exclam";
            break;
        case ROSE_KEYCODE_KP_MEMSTORE:
            return "kp_memstore";
            break;
        case ROSE_KEYCODE_KP_MEMRECALL:
            return "kp_memrecall";
            break;
        case ROSE_KEYCODE_KP_MEMCLEAR:
            return "kp_memclear";
            break;
        case ROSE_KEYCODE_KP_MEMADD:
            return "kp_memadd";
            break;
        case ROSE_KEYCODE_KP_MEMSUBTRACT:
            return "kp_memsubtract";
            break;
        case ROSE_KEYCODE_KP_MEMMULTIPLY:
            return "kp_memmultiply";
            break;
        case ROSE_KEYCODE_KP_MEMDIVIDE:
            return "kp_memdivide";
            break;
        case ROSE_KEYCODE_KP_PLUSMINUS:
            return "kp_plusminus";
            break;
        case ROSE_KEYCODE_KP_CLEAR:
            return "kp_clear";
            break;
        case ROSE_KEYCODE_KP_CLEARENTRY:
            return "kp_clearentry";
            break;
        case ROSE_KEYCODE_KP_BINARY:
            return "kp_binary";
            break;
        case ROSE_KEYCODE_KP_OCTAL:
            return "kp_octal";
            break;
        case ROSE_KEYCODE_KP_DECIMAL:
            return "kp_decimal";
            break;
        case ROSE_KEYCODE_KP_HEXADECIMAL:
            return "kp_hexadecimal";
            break;
        case ROSE_KEYCODE_LCTRL:
            return "lctrl";
            break;
        case ROSE_KEYCODE_LSHIFT:
            return "lshift";
            break;
        case ROSE_KEYCODE_LALT:
            return "lalt";
            break;
        case ROSE_KEYCODE_LGUI:
            return "lgui";
            break;
        case ROSE_KEYCODE_RCTRL:
            return "rctrl";
            break;
        case ROSE_KEYCODE_RSHIFT:
            return "rshift";
            break;
        case ROSE_KEYCODE_RALT:
            return "ralt";
            break;
        case ROSE_KEYCODE_RGUI:
            return "rgui";
            break;
        case ROSE_KEYCODE_MODE:
            return "mode";
            break;
        case ROSE_KEYCODE_AUDIONEXT:
            return "audionext";
            break;
        case ROSE_KEYCODE_AUDIOPREV:
            return "audioprev";
            break;
        case ROSE_KEYCODE_AUDIOSTOP:
            return "audiostop";
            break;
        case ROSE_KEYCODE_AUDIOPLAY:
            return "audioplay";
            break;
        case ROSE_KEYCODE_AUDIOMUTE:
            return "audiomute";
            break;
        case ROSE_KEYCODE_MEDIASELECT:
            return "mediaselect";
            break;
        case ROSE_KEYCODE_WWW:
            return "www";
            break;
        case ROSE_KEYCODE_MAIL:
            return "mail";
            break;
        case ROSE_KEYCODE_CALCULATOR:
            return "calculator";
            break;
        case ROSE_KEYCODE_COMPUTER:
            return "computer";
            break;
        case ROSE_KEYCODE_AC_SEARCH:
            return "ac_search";
            break;
        case ROSE_KEYCODE_AC_HOME:
            return "ac_home";
            break;
        case ROSE_KEYCODE_AC_BACK:
            return "ac_back";
            break;
        case ROSE_KEYCODE_AC_FORWARD:
            return "ac_forward";
            break;
        case ROSE_KEYCODE_AC_STOP:
            return "ac_stop";
            break;
        case ROSE_KEYCODE_AC_REFRESH:
            return "ac_refresh";
            break;
        case ROSE_KEYCODE_AC_BOOKMARKS:
            return "ac_bookmarks";
            break;
        case ROSE_KEYCODE_BRIGHTNESSDOWN:
            return "brightnessdown";
            break;
        case ROSE_KEYCODE_BRIGHTNESSUP:
            return "brightnessup";
            break;
        case ROSE_KEYCODE_DISPLAYSWITCH:
            return "displayswitch";
            break;
        case ROSE_KEYCODE_KBDILLUMTOGGLE:
            return "kbdillumtoggle";
            break;
        case ROSE_KEYCODE_KBDILLUMDOWN:
            return "kbdillumdown";
            break;
        case ROSE_KEYCODE_KBDILLUMUP:
            return "kbdillumup";
            break;
        case ROSE_KEYCODE_EJECT:
            return "eject";
            break;
        case ROSE_KEYCODE_SLEEP:
            return "sleep";
            break;
        case ROSE_KEYCODE_APP1:
            return "app1";
            break;
        case ROSE_KEYCODE_APP2:
            return "app2";
            break;
        default:
            return NULL;
    }
}