#include "rose.h"

void rose_desktop_player::call_update() {
    switch (this->screenmode) {
        case ROSE_CONSOLEMODE:
            return;
        default:
            this->get_active_rt()->call_update();
    }

}

void rose_desktop_player::call_draw() {
    switch (this->screenmode) {
        case ROSE_CONSOLEMODE:
            return;
        default:
            this->get_active_rt()->call_draw();
    }

}

void rose_desktop_player::call_onmouse(int16_t x, int16_t y) {
    this->get_active_rt()->update_mouse_pos(x, y);
    this->get_active_rt()->call_onmouse(x, y);
}

void rose_desktop_player::call_onwheel(int16_t x, int16_t y, bool inverted) {
    if (x != 0 && y != 0) {
        this->get_active_rt()->update_wheel_state(x, y, inverted);
        this->get_active_rt()->call_onwheel(x, y, inverted);
    } else {
        return;
    }
}

void rose_desktop_player::call_onbtn(uint8_t code, bool pressed) {
    this->get_active_rt()->update_btn_state(code, pressed);
    this->get_active_rt()->call_onbtn(code, pressed);
}

bool rose_desktop_player::is_reload_shortcut(rose_keycode keycode, bool pressed) {
    bool res = keycode == ROSE_KEYCODE_R;
    res &= pressed;
    bool is_control = rose_get_bit(this->game_rt->key_states.begin, ROSE_KEYCODE_LCTRL) ||
                   rose_get_bit(this->game_rt->key_states.begin, ROSE_KEYCODE_RCTRL);

    bool is_gui = rose_get_bit(this->game_rt->key_states.begin, ROSE_KEYCODE_RGUI) ||
                  rose_get_bit(this->game_rt->key_states.begin, ROSE_KEYCODE_LGUI);
    res &= (is_control || is_gui);
    return res;
}

void rose_desktop_player::call_onkey(rose_keycode keycode, bool pressed, bool repeat) {
    if (this->is_reload_shortcut(keycode, pressed))
    {
        this->screenmode = ROSE_GAMEMODE;
        this->reload();
        return;
    }

    if (keycode == ROSE_KEYCODE_ESCAPE && pressed) {
        switch (this->screenmode) {
            case ROSE_GAMEMODE:{
                this->screenmode = ROSE_CONSOLEMODE;
                break;
            }
            case ROSE_CONSOLEMODE:{
                this->screenmode = ROSE_EDITORMODE;
                break;
            }
            case ROSE_EDITORMODE:{
                this->screenmode = ROSE_CONSOLEMODE;
                break;
            }
            default:break;
        }
        return;
    }

    this->update_keystate(keycode, pressed);
    this->get_active_rt()->call_onkey(keycode, pressed, repeat);
}

void rose_desktop_player::call_ontouch() {
    this->get_active_rt()->call_ontouch();
}

rose_desktop_player::rose_desktop_player(rose_fs* fs) {
    if (fs == NULL) {
        fprintf(stderr, "tried to create runtime game with null fs\n");
        exit(1);
    }
    this->fs = fs;
    this->game_rt = new rose_rt(fs);
    this->game_rt->error_cb = [this] (string exception_string) {
        this->error_cb(exception_string);
    };
    this->cart = NULL;
    this->carts_root = NULL;
    this->screenmode = rose_screenmode::ROSE_CONSOLEMODE;
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

    return this->game_rt->call_init();
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

void rose_desktop_player::update_keystate(rose_keycode keycode, bool pressed) {
    get_active_rt()->update_keystate(keycode, pressed);
}

bool rose_desktop_player::is_hd() {
    return this->get_active_rt()->meta.hd;
}

uint32_t rose_desktop_player::get_screen_length() {
    if (is_hd()) {
        return ROSE_HD_SCREEN_SIZE;
    } else {
        return ROSE_SCREEN_SIZE;
    }
}

uint8_t rose_desktop_player::get_color_index(uint32_t i) {
    switch (this->screenmode) {
        case ROSE_CONSOLEMODE: {
            return 1;
        }
        case ROSE_GAMEMODE: {
            return *(get_active_rt()->screen.begin + i);
        }
        case ROSE_EDITORMODE: {
            return 2;
        }
        case ROSE_STARTUPMODE: {
            return 3;
        }
    }

}

void rose_desktop_player::get_color(rose_color& color, uint8_t color_index) {
    auto c = (get_active_rt()->palette.begin + (color_index * 3));
    color.r = *(c++);
    color.g = *(c++);
    color.b = *c;
}

void rose_desktop_player::error_cb(string exception_string) {
    this->screenmode = ROSE_CONSOLEMODE;
    fprintf(stderr, "%s", exception_string.c_str());
}
