#ifndef ROSE_RT_GAME_H
#define ROSE_RT_GAME_H

#include "config.h"
#include "rt/rt_base.h"

rose_runtime_game* rose_runtime_game_create(rose_fs* fs);

void rose_runtime_game_free(rose_runtime_game* r);

bool rose_runtime_game_reload(rose_runtime_game* r);

rose_runtime_game_error rose_runtime_game_init(rose_runtime_game* r);

rose_runtime_game_error rose_runtime_game_update(rose_runtime_game* r);

rose_runtime_game_error rose_runtime_game_draw(rose_runtime_game* r);

rose_runtime_game_error rose_runtime_game_onmouse(rose_runtime_game* r, int16_t x, int16_t y);

rose_runtime_game_error rose_runtime_game_onwheel(rose_runtime_game* r, int16_t x, int16_t y, bool inverted);

rose_runtime_game_error rose_runtime_game_onbtn(rose_runtime_game* r, uint8_t code, bool pressed);

rose_runtime_game_error rose_runtime_game_onkey(rose_runtime_game* r, rose_keycode keycode, bool pressed, bool repeat);

rose_runtime_game_error rose_runtime_game_ontouch(rose_runtime_game* r);

#endif
