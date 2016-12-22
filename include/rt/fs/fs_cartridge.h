#ifndef ROSE_FS_CARTRIDGE_H
#define ROSE_FS_CARTRIDGE_H

#include "config.h"
#include <string.h>
#include <zlib.h>
#include <archive.h>
#include <archive_entry.h>

typedef struct {
    uint8_t *data;
    size_t data_size;
    char *code;
    size_t code_size;
} Rose_Cartridge;

Rose_Cartridge *rose_cartridge_create();

void rose_cartridge_free(Rose_Cartridge *cart);

static uint8_t rose_default_palette[16 * 3] = {
    0,   0,   0,   // black
    29,  43,  83,  // dark-blue
    126, 37,  83,  // dark-purple
    0,   135, 81,  // dark-green
    171, 82,  54,  // brown
    95,  87,  79,  // dark-gray
    194, 195, 199, // light-gray
    255, 241, 232, // white
    255, 0,   77,  // red
    255, 164, 0,   // orange
    255, 236, 39,  // yellow
    0,   228, 54,  // green
    41,  173, 255, // blue
    131, 118, 156, // indigo
    255, 119, 168, // pink
    255, 204, 170  // peach
};

#endif