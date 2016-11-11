#ifndef ROSE_CARTRIDGE_H
#define ROSE_CARTRIDGE_H

#include "config.h"
#include <string.h>

typedef struct {
    uint8_t* data;
    size_t data_size;
    char* code;
    size_t code_size;
} Rose_Cartridge;

Rose_Cartridge* rose_cartridge_create();

void rose_cartridge_free(Rose_Cartridge* cart);

#endif