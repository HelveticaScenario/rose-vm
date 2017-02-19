#pragma once

#include "config.h"
#include <array>
#include "common.fwd.h"
#include "fs.fwd.h"
#include "js.fwd.h"
#include "enums.h"

struct rose_memory_range {
    std::array<uint8_t, ROSE_MEMORY_SIZE>::iterator begin;
    std::array<uint8_t, ROSE_MEMORY_SIZE>::iterator end;
};

struct rose_rt_meta {
    bool hd;
    string name;
    string author;
};

struct rose_rt_base {
    rose_rt_meta meta;
    rose_fs* fs;
    rose_file* self_cart;
    rose_file* target_cart;

    std::array<uint8_t, ROSE_MEMORY_SIZE>* mem;
    rose_memory_range screen;
    rose_memory_range schema;
    rose_memory_range palette;
    rose_memory_range palette_filter; // TODO: rename this to something not shit
    rose_memory_range palette_transparency;
    rose_memory_range clipping_region;
    std::array<uint8_t, ROSE_MEMORY_SIZE>::iterator pen_color_addr;
    rose_memory_range print_cursor;
    rose_memory_range camera_offset;
    rose_memory_range pointer_positions;
    rose_memory_range btn_states;
    rose_memory_range prev_btn_states;
    rose_memory_range mouse_wheel;
    rose_memory_range key_states;
    rose_memory_range prev_key_states;
    rose_memory_range font_data;

    rose_rt_base(rose_fs* fs);
    virtual ~rose_rt_base();
    void make_mem_ranges();

    virtual bool clear();
    void save_input_frame();
    void update_mouse_pos(int16_t x, int16_t y);
    void update_btn_state(uint8_t btn, bool pressed);
    void update_wheel_state(int16_t delta_x, int16_t delta_y, bool inverted);
    void update_keystate(rose_keycode keycode, bool pressed);

    void reset_palette_filter();
    void reset_palette_transparency();
    void reset_clipping_region();
    void reset_pen_color();
    void reset_print_cursor();
    void reset_camera_offset();
    void reset_pointer_positions();
    void reset_btn_states();
    void reset_mouse_wheel();
    void reset_key_states();
    void reset_palette();
    void reset_screen();
    void reset_schema();
    void reset_userdata();
    void reset_font_data();

    void copy_input_from_other(rose_rt* other);
    void copy_screen_from_other(rose_rt* other);

    // Graphics


    rose_api_error pset(int16_t x, int16_t y, uint8_t val);

    rose_api_error pset_default(int16_t x, int16_t y);

    rose_api_error pget(int16_t x, int16_t y, uint8_t* res);

    rose_api_error palset(uint8_t idx, rose_color c);

    rose_api_error palget(uint8_t idx, rose_color* res);

    rose_api_error line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t c);

    rose_api_error line_default(int16_t x0, int16_t y0, int16_t x1, int16_t y1);

    rose_api_error rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t c);

    rose_api_error rect_default(int16_t x0, int16_t y0, int16_t x1, int16_t y1);

    rose_api_error rectfill(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t c);

    rose_api_error rectfill_default(int16_t x0, int16_t y0, int16_t x1, int16_t y1);

    rose_api_error circ(int16_t x0, int16_t y0, uint16_t radius, uint8_t c);

    rose_api_error circ_default(int16_t x0, int16_t y0, uint16_t radius);

    rose_api_error circfill(int16_t x0, int16_t y0, uint16_t radius, uint8_t c);

    rose_api_error circfill_default(int16_t x0, int16_t y0, uint16_t radius);

    rose_api_error tri(int16_t v1_x, int16_t v1_y, int16_t v2_x, int16_t v2_y, int16_t v3_x, int16_t v3_y, uint8_t c);

    rose_api_error tri_default(int16_t v0_x, int16_t v0_y, int16_t v1_x, int16_t v1_y, int16_t v2_x, int16_t v2_y);

    rose_api_error trifill(int16_t v1_x, int16_t v1_y, int16_t v2_x, int16_t v2_y, int16_t v3_x, int16_t v3_y, uint8_t c);

    rose_api_error trifill_default(int16_t v0_x, int16_t v0_y, int16_t v1_x, int16_t v1_y, int16_t v2_x, int16_t v2_y);

    rose_api_error cls();

    rose_api_error get_spritesheet_meta(uint32_t* addr, uint16_t* sheet_width, uint16_t* sheet_height, uint8_t* sprite_width_mult, uint8_t* sprite_height_mult);

    rose_api_error set_spritesheet_meta(uint32_t addr, uint16_t sheet_width, uint16_t sheet_height, uint8_t sprite_width_mult, uint8_t sprite_height_mult);

    rose_api_error spr(uint32_t n, int16_t x, int16_t y, uint8_t w, uint8_t h, bool flip_x, bool flip_y);

    rose_api_error print(string str, int16_t x, int16_t y, uint8_t c);

    rose_api_error color(uint8_t c);


    // input

    rose_api_error mouse(int16_t* x, int16_t* y);

    rose_api_error btn(uint8_t idx, bool* res);

    rose_api_error btnp(uint8_t idx, bool* res);

    rose_api_error wheel(int16_t* x, int16_t* y);

    rose_api_error wheel_inverted(bool* res);

    rose_api_error key(rose_keycode keycode, bool* res);

    rose_api_error keyp(rose_keycode keycode, bool* res);

    // memory

    rose_api_error poke(uint32_t idx, uint8_t val);

    rose_api_error peek(uint32_t idx, uint8_t* res);

    rose_api_error rt_memcpy(uint32_t dest_addr, uint32_t source_addr, uint32_t len);

    rose_api_error rt_memset(uint32_t dest_addr, uint8_t val, uint32_t len);

    rose_api_error cstore(uint32_t dest_addr, uint32_t source_addr, uint32_t len);

    rose_api_error reload(uint32_t dest_addr, uint32_t source_addr, uint32_t len);


};

struct rose_rt: public rose_rt_base {
    std::function<void(string)> error_cb;

    rose_js* js;

    rose_rt(rose_fs* fs);
    ~rose_rt();
    bool clear();

    void retarget(rose_file* self, rose_file* target);
    bool load_run_main();

    function<void(const v8::FunctionCallbackInfo<v8::Value>& args)> extcmd_cb;

    bool call_init();

    bool call_update();

    bool call_draw();

    bool call_onmouse(int16_t x, int16_t y);

    bool call_onwheel(int16_t x, int16_t y, bool inverted);

    bool call_onbtn(uint8_t code, bool pressed);

    bool call_onkey(rose_keycode keycode, bool pressed, bool repeat);

    bool call_ontouch();

    bool rose_call( const char* name, uint8_t nargs, Local<Value>* args);
};

void rose_set_bit(uint8_t* trans, uint8_t addr, bool val);

bool rose_get_bit(uint8_t* trans, uint8_t addr);

void rose_init(const char* base_path);

void rose_deinit();