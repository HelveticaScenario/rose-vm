#include "runtime_base.h"

Rose_MemoryIterator rose_memory_iterator_begin(uint8_t m[]) { return &m[0]; }
Rose_MemoryIterator rose_memory_iterator_end(uint8_t m[], uint32_t len) { return &m[len]; }
Rose_MemoryIterator rose_memory_iterator_next(Rose_MemoryIterator i) { return ++i; }

Rose_RuntimeBase* rose_runtime_base_create(Rose_Cartridge* cartridge) {
    uint8_t* mem = (uint8_t*) malloc(ROSE_MEMORY_SIZE);
    memset(mem, 0, ROSE_MEMORY_SIZE);

    Rose_MemoryRange* screen_range = (Rose_MemoryRange*) malloc(sizeof(Rose_MemoryRange));
    Rose_MemoryIterator end = rose_memory_iterator_end(mem, ROSE_MEMORY_SIZE);
    Rose_MemoryIterator beg_screen = end - (ROSE_SCREEN_SIZE);
    screen_range->begin = beg_screen;
    screen_range->end = end;

    Rose_MemoryRange* palette_range = (Rose_MemoryRange*) malloc(sizeof(Rose_MemoryRange));
    Rose_MemoryIterator beg_palette = end - ROSE_RUNTIME_RESERVED_MEMORY_SIZE - ROSE_PALETTE_SIZE;
    Rose_MemoryIterator end_palette = beg_palette + ROSE_PALETTE_SIZE;
    palette_range->begin = beg_palette;
    palette_range->end = end_palette;

    Rose_MemoryRange* palette_filter_range = (Rose_MemoryRange*) malloc(sizeof(Rose_MemoryRange));
    Rose_MemoryIterator beg_palette_filter = end_palette;
    Rose_MemoryIterator end_palette_filter = beg_palette_filter + ROSE_PALETTE_INDEX_NUM;
    palette_filter_range->begin = beg_palette_filter;
    palette_filter_range->end = end_palette_filter;

    int i;
    for (i = 0; i < ROSE_PALETTE_INDEX_NUM; ++i) {
        beg_palette_filter[i] = i;
    }

    Rose_MemoryRange* palette_transparency_range = (Rose_MemoryRange*) malloc(sizeof(Rose_MemoryRange));
    Rose_MemoryIterator beg_palette_transparency = end_palette_filter;
    Rose_MemoryIterator end_palette_transparency = beg_palette_transparency + (ROSE_PALETTE_INDEX_NUM / 8);
    palette_transparency_range->begin = beg_palette_transparency;
    palette_transparency_range->end = end_palette_transparency;

    rose_set_bit(beg_palette_transparency, 0, true);

    Rose_MemoryRange* clipping_region_range = (Rose_MemoryRange*) malloc(sizeof(Rose_MemoryRange));
    Rose_MemoryIterator beg_clipping_region = end_palette_transparency;
    Rose_MemoryIterator end_clipping_region = beg_clipping_region + 8;
    clipping_region_range->begin = beg_clipping_region;
    clipping_region_range->end = end_clipping_region;

    uint16_t* clipping_region = (uint16_t*) beg_clipping_region;
    clipping_region[0] = 0; // x0
    clipping_region[1] = 0; // y0
    clipping_region[2] = ROSE_SCREEN_WIDTH - 1; // x1
    clipping_region[3] = ROSE_SCREEN_HEIGHT - 1; // y1

    Rose_MemoryIterator pen_color_addr = end_clipping_region;
    *pen_color_addr = 6;

    Rose_MemoryRange* print_cursor_range = (Rose_MemoryRange*) malloc(sizeof(Rose_MemoryRange));
    Rose_MemoryIterator beg_print_cursor = pen_color_addr + 1;
    Rose_MemoryIterator end_print_cursor = beg_print_cursor + 4;
    print_cursor_range->begin = beg_print_cursor;
    print_cursor_range->end = end_print_cursor;

    uint16_t* print_cursor = (uint16_t*) beg_print_cursor;
    print_cursor[0] = 0; // x0
    print_cursor[1] = 0; // y0 // TODO: replace with actual starting position once font size is finalized

    Rose_MemoryRange* camera_offset_range = (Rose_MemoryRange*) malloc(sizeof(Rose_MemoryRange));
    Rose_MemoryIterator beg_camera_offset = end_print_cursor;
    Rose_MemoryIterator end_camera_offset = beg_camera_offset + 4;
    camera_offset_range->begin = beg_camera_offset;
    camera_offset_range->end = end_camera_offset;

    int16_t* camera_offset = (int16_t*) beg_camera_offset;
    camera_offset[0] = 0; // x0
    camera_offset[1] = 0; // y0 // TODO: replace with actual starting position once font size is finalized

    Rose_MemoryRange* pointer_positions_range = (Rose_MemoryRange*) malloc(sizeof(Rose_MemoryRange));
    Rose_MemoryIterator beg_pointer_positions = end_camera_offset;
    Rose_MemoryIterator end_pointer_positions = beg_pointer_positions + (11 * 2 /* 16 bit number */);
    pointer_positions_range->begin = beg_pointer_positions;
    pointer_positions_range->end = end_pointer_positions;

    Rose_MemoryRange* btn_states_range = (Rose_MemoryRange*) malloc(sizeof(Rose_MemoryRange));
    Rose_MemoryIterator beg_btn_states = end_pointer_positions;
    Rose_MemoryIterator end_btn_states = beg_btn_states + 4 /* 32 bit fields */;
    btn_states_range->begin = beg_btn_states;
    btn_states_range->end = end_btn_states;

    Rose_RuntimeBase* r = (Rose_RuntimeBase*) malloc(sizeof(Rose_RuntimeBase));
    r->cartridge = cartridge;
    r->mem = mem;
    r->mem_size = ROSE_MEMORY_SIZE;
    r->screen = screen_range;
    r->palette = palette_range;
    r->palette_filter = palette_filter_range;
    r->palette_transparency = palette_transparency_range;
    r->clipping_region = clipping_region_range;
    r->pen_color_addr = pen_color_addr;
    r->print_cursor = print_cursor_range;
    r->camera_offset = camera_offset_range;
    r->pointer_positions = pointer_positions_range;
    r->btn_states = btn_states_range;
    
    return r;
}

void rose_runtime_base_free(Rose_RuntimeBase* r) {
    free(r->mem);
    free(r->screen);
    free(r->palette);
    free(r->palette_filter);
    free(r->palette_transparency);
    free(r->clipping_region);
    free(r->print_cursor);
    free(r->camera_offset);
    free(r->pointer_positions);
    free(r->btn_states);
    free(r);
}

void rose_set_bit(uint8_t* arr, uint8_t addr, bool val) {
    uint8_t idx = addr / 8;
    uint8_t bit = addr % 8;
    if (val) {
        arr[idx] |= 1 << bit;
    } else {
        arr[idx] &= ~(1 << bit);
    }
}

bool rose_get_bit(uint8_t* arr, uint8_t addr) {
    uint8_t idx = addr / 8;
    uint8_t bit = addr % 8;
    return (arr[idx] >> bit) & 1;
}