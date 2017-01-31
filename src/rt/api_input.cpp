#include "rose.h"

rose_api_error rose_rt_base::mouse(int16_t* x, int16_t* y) {
    int16_t* pointer = (int16_t*) pointer_positions.begin;
    *x = pointer[20];
    *y = pointer[21];
    return ROSE_API_ERR_NONE;
}

rose_api_error rose_rt_base::btn(uint8_t idx, bool* res) {
    *res = rose_get_bit(btn_states.begin, idx);
    return ROSE_API_ERR_NONE;
}

rose_api_error rose_rt_base::btnp(uint8_t idx, bool* res) {
    bool cur_state = rose_get_bit(btn_states.begin, idx);
    bool prev_state = rose_get_bit(prev_btn_states.begin, idx);
    *res = (cur_state && !prev_state);
    return ROSE_API_ERR_NONE;
}

rose_api_error rose_rt_base::wheel(int16_t* x, int16_t* y) {
    int16_t* pointer = (int16_t*) mouse_wheel.begin;
    *x = pointer[0];
    *y = pointer[1];
    return ROSE_API_ERR_NONE;
}

rose_api_error rose_rt_base::wheel_inverted(bool* res) {
    bool* inverted = (bool*) (mouse_wheel.begin + 4);
    *res = *inverted;
    return ROSE_API_ERR_NONE;
}

rose_api_error rose_rt_base::key(rose_keycode keycode, bool* res) {
    if (keycode >= ROSE_KEYCODE_UNKNOWN) {
        return ROSE_API_ERR_OUT_OF_BOUNDS;
    }
    *res = rose_get_bit(key_states.begin, keycode);
    return ROSE_API_ERR_NONE;
}

rose_api_error rose_rt_base::keyp(rose_keycode keycode, bool* res) {
    if (keycode >= ROSE_KEYCODE_UNKNOWN) {
        return ROSE_API_ERR_OUT_OF_BOUNDS;
    }
    bool cur_state = rose_get_bit(key_states.begin, keycode);
    bool prev_state = rose_get_bit(prev_key_states.begin, keycode);
    *res = (cur_state && !prev_state);
    return ROSE_API_ERR_NONE;
}