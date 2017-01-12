#pragma once

#include "enums.h"
#include "rt.fwd.h"

struct rose_game {
    rose_rt* rt;
    rose_game(rose_fs* fs);
    ~rose_game();
    bool reload();
};


rose_game_error rose_game_init(rose_game* r);

rose_game_error rose_game_update(rose_game* r);

rose_game_error rose_game_draw(rose_game* r);

rose_game_error rose_game_onmouse(rose_game* r, int16_t x, int16_t y);

rose_game_error rose_game_onwheel(rose_game* r, int16_t x, int16_t y, bool inverted);

rose_game_error rose_game_onbtn(rose_game* r, uint8_t code, bool pressed);

rose_game_error rose_game_onkey(rose_game* r, rose_keycode keycode, bool pressed, bool repeat);

rose_game_error rose_game_ontouch(rose_game* r);

