#ifndef RUNTIME_BASE_H
#define RUNTIME_BASE_H

#import "config.h"
#import "cartridge.h"

typedef struct {
    short x;
    short y;
    short relX;
    short relY;
    bool leftBtnDown;
    bool middleBtnDown;
    bool rightBtnDown;
    bool x1BtnDown;
    bool x2BtnDown;
} Rose_MouseState;

typedef uint8_t* Rose_MemoryIterator;

typedef struct {
    Rose_MemoryIterator begin;
    Rose_MemoryIterator end;
} Rose_MemoryRange;

typedef struct {
    Rose_Cartridge* cartridge;
    uint8_t* mem;
    uint_fast32_t mem_size;
    Rose_MemoryRange* screen;
    Rose_MemoryRange* palette;
} Rose_RuntimeBase;

Rose_MemoryRange* rose_memory_range_create(uint8_t begin[], uint_fast32_t len);
Rose_MemoryIterator rose_memory_iterator_begin(uint8_t m[]);
Rose_MemoryIterator rose_memory_iterator_end(uint8_t m[], uint_fast32_t len);
Rose_MemoryIterator rose_memory_iterator_next(Rose_MemoryIterator i);

Rose_RuntimeBase* rose_runtime_base_create(Rose_Cartridge* cartridge);
void rose_runtime_base_free(Rose_RuntimeBase* r);

void updateMouseState(Rose_RuntimeBase* r, const Rose_MouseState* mouseState);

static uint8_t default_palette[16*3] = {
    0, 0, 0,   // black
    29, 43, 83,  // dark-blue
    126, 37, 83,  // dark-purple
    0, 135, 81,  // dark-green
    171, 82, 54,  // brown
    95, 87, 79,  // dark-gray
    194, 195, 199, // light-gray
    255, 241, 232, // white
    255, 0, 77,  // red
    255, 164, 0,   // orange
    255, 236, 39,  // yellow
    0, 228, 54,  // green
    41, 173, 255, // blue
    131, 118, 156, // indigo
    255, 119, 168, // pink
    255, 204, 170  // peach
};

#endif