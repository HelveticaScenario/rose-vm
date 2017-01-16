#include "rose.h"

rose_rt_error rose_desktop_player::call_update() {
    return this->get_active_rt()->call_update();
}

rose_rt_error rose_desktop_player::call_draw() {
    return this->get_active_rt()->call_draw();
}

rose_rt_error rose_desktop_player::call_onmouse(int16_t x, int16_t y) {
    return this->get_active_rt()->call_onmouse(x, y);
}

rose_rt_error rose_desktop_player::call_onwheel(int16_t x, int16_t y, bool inverted) {
    return this->get_active_rt()->call_onwheel(x, y, inverted);
}

rose_rt_error rose_desktop_player::call_onbtn(uint8_t code, bool pressed) {
    return this->get_active_rt()->call_onbtn(code, pressed);
}

rose_rt_error rose_desktop_player::call_onkey(rose_keycode keycode, bool pressed, bool repeat) {
    return this->get_active_rt()->call_onkey(keycode, pressed, repeat);
}

rose_rt_error rose_desktop_player::call_ontouch() {
    return this->get_active_rt()->call_update();
}

rose_desktop_player::rose_desktop_player(rose_fs* fs) {
    if (fs == NULL) {
        fprintf(stderr, "tried to create runtime game with null fs\n");
        exit(1);
    }
    this->fs = fs;
    this->game_rt = new rose_rt(fs);
    this->cart = NULL;
}

rose_desktop_player::~rose_desktop_player() {
    delete this->game_rt;
}


bool rose_desktop_player::reload() {
    if (this->cart == NULL) {
        return false;
    }
    if (!this->game_rt->clear()) {
        return false;
    }
    this->game_rt->retarget(cart, cart);
    if (!this->game_rt->load_run_main()) {
        return false;
    }

    // insert any game runtime specific init code here

    return !(this->game_rt->call_init() == ROSE_RT_CRITICAL_ERR);
}

bool rose_desktop_player::load_cart_path(string cart_path) {
    rose_file* cart = rose_fs_fetch_child(this->fs->root, cart_path);
    if (cart == NULL) {
        return false;
    }
    if (this->cart != NULL && !this->cart->on_disk) {
        rose_file_free_recurse(this->cart);
    }
    this->cart = cart;
    return reload();
}

rose_rt* rose_desktop_player::get_active_rt() {
    return this->game_rt;
}

void rose_desktop_player::save_input_frame() {
    get_active_rt()->save_input_frame();
}

void rose_desktop_player::update_mousestate(const rose_mousestate* mousestate) {
    get_active_rt()->update_mousestate(mousestate);
}

void rose_desktop_player::update_keystate(rose_keycode keycode, bool pressed) {
    get_active_rt()->update_keystate(keycode, pressed);
}

void
rose_desktop_player::reset_input(rose_mousestate* mousestate) {
    get_active_rt()->reset_input(mousestate);
}

bool rose_desktop_player::is_hd() {
    return this->get_active_rt()->meta.hd;
}

uint32_t rose_desktop_player::get_screen_length() {
    if (is_hd()) {
        return ROSE_HD_SCREEN_HEIGHT * ROSE_HD_SCREEN_WIDTH;
    } else {
        return ROSE_SCREEN_HEIGHT * ROSE_SCREEN_WIDTH;
    }
}

uint8_t rose_desktop_player::get_color_index(uint32_t i) {
    return *(get_active_rt()->screen->begin + i);
}

void rose_desktop_player::get_color(rose_color& color, uint8_t color_index) {
    auto c = (get_active_rt()->palette->begin + (color_index * 3));
    color.r = *(c++);
    color.g = *(c++);
    color.b = *c;
}
