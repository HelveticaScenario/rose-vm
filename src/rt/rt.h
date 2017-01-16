#pragma once

#include "config.h"
#include <array>
#include "common.fwd.h"
#include "fs.fwd.h"
#include "js.fwd.h"
#include "enums.h"

struct rose_mousestate {
    int16_t x;
    int16_t y;
    bool left_btn_down;
    bool middle_btn_down;
    bool right_btn_down;
    bool x1_btn_down;
    bool x2_btn_down;
    int16_t wheel_x;
    int16_t wheel_y;
    bool wheel_inverted;
};

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

    rose_js* js;
    rose_fs* fs;
    rose_file* self_cart;
    rose_file* target_cart;
    std::array<uint8_t, ROSE_MEMORY_SIZE>* mem;
    rose_memory_range* screen;
    rose_memory_range* schema;
    rose_memory_range* palette;
    rose_memory_range* palette_filter; // TODO: rename this to something not shit
    rose_memory_range* palette_transparency;
    rose_memory_range* clipping_region;
    std::array<uint8_t, ROSE_MEMORY_SIZE>::iterator pen_color_addr;
    rose_memory_range* print_cursor;
    rose_memory_range* camera_offset;
    rose_memory_range* pointer_positions;
    rose_memory_range* btn_states;
    rose_memory_range* prev_btn_states;
    rose_memory_range* mouse_wheel;
    rose_memory_range* key_states;
    rose_memory_range* prev_key_states;

    rose_rt(rose_fs* fs);
    ~rose_rt();
    void make_mem_ranges();

    void retarget(rose_file* self, rose_file* target);
    bool clear();
    bool load_run_main();
    void save_input_frame();
    void update_mousestate(const rose_mousestate* mousestate);
    void update_keystate(rose_keycode keycode, bool pressed);
    void reset_input(rose_mousestate* mousestate);

    rose_rt_error call_init();

    rose_rt_error call_update();

    rose_rt_error call_draw();

    rose_rt_error call_onmouse(int16_t x, int16_t y);

    rose_rt_error call_onwheel(int16_t x, int16_t y, bool inverted);

    rose_rt_error call_onbtn(uint8_t code, bool pressed);

    rose_rt_error call_onkey(rose_keycode keycode, bool pressed, bool repeat);

    rose_rt_error call_ontouch();

private:
    rose_rt_error rose_call( const char* name, uint8_t nargs, Local<Value>* args);

};

void rose_set_bit(uint8_t* trans, uint8_t addr, bool val);

bool rose_get_bit(uint8_t* trans, uint8_t addr);

void rose_init(const char* base_path);

void rose_deinit();