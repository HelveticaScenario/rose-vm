#ifndef ROSE_RT_BASE_H
#define ROSE_RT_BASE_H

#import "config.h"
#import "cartridge.h"

typedef struct {
    int16_t x;
    int16_t y;
    bool leftBtnDown;
    bool middleBtnDown;
    bool rightBtnDown;
    bool x1BtnDown;
    bool x2BtnDown;
    int16_t wheel_x;
    int16_t wheel_y;
    bool wheel_inverted;
} Rose_MouseState;

typedef uint8_t* Rose_MemoryIterator;

typedef struct {
    Rose_MemoryIterator begin;
    Rose_MemoryIterator end;
} Rose_MemoryRange;

typedef struct {
    Rose_Cartridge* cartridge;
    uint8_t* mem;
    uint32_t mem_size;
    Rose_MemoryRange* screen;
    Rose_MemoryRange* palette;
    Rose_MemoryRange* palette_filter; // TODO: rename this to something not shit
    Rose_MemoryRange* palette_transparency;
    Rose_MemoryRange* clipping_region;
    Rose_MemoryIterator pen_color_addr;
    Rose_MemoryRange* print_cursor;
    Rose_MemoryRange* camera_offset;
    Rose_MemoryRange* pointer_positions;
    Rose_MemoryRange* btn_states;
    Rose_MemoryRange* mouse_wheel;
} Rose_RuntimeBase;

Rose_MemoryRange* rose_memory_range_create(uint8_t begin[], uint32_t len);
Rose_MemoryIterator rose_memory_iterator_begin(uint8_t m[]);
Rose_MemoryIterator rose_memory_iterator_end(uint8_t m[], uint32_t len);
Rose_MemoryIterator rose_memory_iterator_next(Rose_MemoryIterator i);

Rose_RuntimeBase* rose_runtime_base_create(Rose_Cartridge* cartridge);

void rose_runtime_base_free(Rose_RuntimeBase* r);

void rose_runtime_base_update_mousestate(Rose_RuntimeBase* r, const Rose_MouseState* mouseState);

void rose_set_bit(uint8_t* trans, uint8_t addr, bool val);

bool rose_get_bit(uint8_t* trans, uint8_t addr);

#endif