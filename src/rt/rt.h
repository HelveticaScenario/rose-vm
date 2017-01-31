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

struct rose_rt {
    rose_rt_meta meta;
    std::function<void(string)> error_cb;

    rose_js* js;
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

    rose_rt(rose_fs* fs);
    ~rose_rt();
    void make_mem_ranges();

    void retarget(rose_file* self, rose_file* target);
    bool clear();
    bool load_run_main();
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

    bool call_init();

    bool call_update();

    bool call_draw();

    bool call_onmouse(int16_t x, int16_t y);

    bool call_onwheel(int16_t x, int16_t y, bool inverted);

    bool call_onbtn(uint8_t code, bool pressed);

    bool call_onkey(rose_keycode keycode, bool pressed, bool repeat);

    bool call_ontouch();

    bool rose_call( const char* name, uint8_t nargs, Local<Value>* args);

    void copy_input_from_other(rose_rt* other);
    void copy_screen_from_other(rose_rt* other);
};

void rose_set_bit(uint8_t* trans, uint8_t addr, bool val);

bool rose_get_bit(uint8_t* trans, uint8_t addr);

void rose_init(const char* base_path);

void rose_deinit();