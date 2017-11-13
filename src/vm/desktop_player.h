#pragma once

#include "enums.h"
#include "fs.fwd.h"
#include "file.fwd.h"
#include "vm.fwd.h"
#include "rose.h"

struct rose_editor_instance {
    rose_vm* rt;
};

struct rose_desktop_player {
    rose_fs* fs;
    rose_file* cart;
    rose_vm* shell_rt;
    rose_vm* game_rt;

    vector<rose_editor_instance*> editors;

    rose_screenmode screenmode;

    vector<rose_file*> default_editors;

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

    function<void(const v8::FunctionCallbackInfo<v8::Value>& args)> shell_extcmd_cb;
    function<void(const v8::FunctionCallbackInfo<v8::Value>& args)> game_extcmd_cb;
    function<void(const v8::FunctionCallbackInfo<v8::Value>& args)> editor_extcmd_cb;

    void call_update();

    void call_draw();

    void call_onmouse(int16_t x, int16_t y);

    void call_onwheel(int16_t x, int16_t y, bool inverted);

    void call_onbtn(uint8_t code, bool pressed);

    void call_onkey(rose_keycode keycode, bool pressed, bool repeat);

    void call_ontouch();


    void writestr(const v8::FunctionCallbackInfo<v8::Value>& args);

    void readstr(const v8::FunctionCallbackInfo<v8::Value>& args);

    void savefile(const v8::FunctionCallbackInfo<v8::Value>& args);

    void mkfile(const v8::FunctionCallbackInfo<v8::Value>& args);

    void rmfile(const v8::FunctionCallbackInfo<v8::Value>& args);

    void load(const v8::FunctionCallbackInfo<v8::Value>& args);

    void run(const v8::FunctionCallbackInfo<v8::Value>& args);

    void log(const v8::FunctionCallbackInfo<v8::Value>& args);

    rose_file* fetch_sys_cart(string cart_name);

    rose_vm* get_active_rt();
    bool is_reload_shortcut(rose_keycode keycode, bool pressed);


};


