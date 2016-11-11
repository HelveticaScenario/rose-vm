#include "runtime_base.h"

Rose_MemoryIterator rose_memory_iterator_begin(uint8_t m[]) { return &m[0]; }
Rose_MemoryIterator rose_memory_iterator_end(uint8_t m[], uint_fast32_t len) { return &m[len]; }
Rose_MemoryIterator rose_memory_iterator_next(Rose_MemoryIterator i) { return ++i; }

Rose_RuntimeBase* rose_runtime_base_create(Rose_Cartridge* cartridge) {
    uint8_t* mem = (uint8_t*) malloc(ROSE_MEMORY_SIZE);
    memset(mem, 0, ROSE_MEMORY_SIZE);
    Rose_MemoryIterator end = rose_memory_iterator_end(mem, ROSE_MEMORY_SIZE);
    Rose_MemoryIterator beg_screen = end - (ROSE_SCREEN_SIZE);
    Rose_MemoryIterator beg_palette = beg_screen - (ROSE_PALETTE_SIZE);
    Rose_MemoryRange* screen_range = (Rose_MemoryRange*) malloc(sizeof(Rose_MemoryRange));
    screen_range->begin = beg_screen;
    screen_range->end = end;
    Rose_MemoryRange* palette_range = (Rose_MemoryRange*) malloc(sizeof(Rose_MemoryRange));
    palette_range->begin = beg_palette;
    palette_range->end = beg_screen;

    memcpy(palette_range->begin, default_palette, sizeof(default_palette));

    Rose_RuntimeBase* r = (Rose_RuntimeBase*) malloc(sizeof(Rose_RuntimeBase));
    r->cartridge = cartridge;
    r->mem = mem;
    r->mem_size = ROSE_MEMORY_SIZE;
    r->screen = screen_range;
    r->palette = palette_range;
    return r;
}

void rose_runtime_base_free(Rose_RuntimeBase* r) {
    free(r->mem);
    free(r->palette);
    free(r->screen);
    free(r);
}