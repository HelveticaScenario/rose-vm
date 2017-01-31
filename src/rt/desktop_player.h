#pragma once

#include "enums.h"
#include "fs.fwd.h"
#include "file.fwd.h"
#include "rt.fwd.h"
#include "rose.h"

struct rose_editor_instance {
    rose_rt* rt;
};

struct rose_desktop_player {
    rose_fs* fs;
    rose_file* cart;
    rose_rt* game_rt;

    vector<rose_editor_instance*> editors;

    rose_screenmode screenmode;

    vector<rose_file*> default_editors;
    rose_file* carts_root;

    rose_desktop_player(rose_fs* fs);
    ~rose_desktop_player();
    bool load_cart_path(string cart_path);
    bool reload();

    bool is_hd();
    uint32_t get_screen_length();
    uint8_t get_color_index(uint32_t i);
    void get_color(rose_color& color, uint8_t color_index);

    void save_input_frame();
    void update_keystate(rose_keycode keycode, bool pressed);
    void error_cb(string exception_string);

    void call_update();

    void call_draw();

    void call_onmouse(int16_t x, int16_t y);

    void call_onwheel(int16_t x, int16_t y, bool inverted);

    void call_onbtn(uint8_t code, bool pressed);

    void call_onkey(rose_keycode keycode, bool pressed, bool repeat);

    void call_ontouch();

private:
    rose_rt* get_active_rt();
    bool is_reload_shortcut(rose_keycode keycode, bool pressed);
};


