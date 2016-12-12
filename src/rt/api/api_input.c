#include "rt/api/api_input.h"

Rose_RuntimeApiError rose_api_input_mouse(Rose_RuntimeBase* r, int16_t* x, int16_t* y) {
    int16_t* pointer = (int16_t*) r->pointer_positions->begin;
    *x = pointer[20];
    *y = pointer[21];
    return ROSE_API_ERR_NONE;
}

Rose_RuntimeApiError rose_api_input_btn(Rose_RuntimeBase* r, uint8_t idx, bool* res) {
    *res = rose_get_bit(r->btn_states->begin, idx);
    return ROSE_API_ERR_NONE;
}

Rose_RuntimeApiError rose_api_input_wheel(Rose_RuntimeBase* r, int16_t* x, int16_t* y) {
    int16_t* pointer = (int16_t*) r->mouse_wheel->begin;
    *x = pointer[0];
    *y = pointer[1];
    return ROSE_API_ERR_NONE;
}

Rose_RuntimeApiError rose_api_input_wheel_inverted(Rose_RuntimeBase* r, bool* res) {
    bool* inverted = (bool*) (r->mouse_wheel->begin + 4);
    *res = *inverted;
    return ROSE_API_ERR_NONE;
}

Rose_RuntimeApiError rose_api_input_key(Rose_RuntimeBase* r, Rose_KeyCode keycode, bool* res) {
    if (keycode >= ROSE_KEYCODE_UNKNOWN) {
        return ROSE_API_ERR_OUT_OF_BOUNDS;
    }
    *res = rose_get_bit(r->key_states->begin, keycode);
    return ROSE_API_ERR_NONE;
}