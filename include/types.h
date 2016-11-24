#ifndef ROSE_TYPES_H
#define ROSE_TYPES_H

#include <stdint.h>

typedef enum {
    ROSE_API_ERR_NONE,
    ROSE_API_ERR_OUT_OF_BOUNDS
} Rose_RuntimeApiError;

typedef enum { GameMode, EditorMode } Rose_ScreenMode;

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Rose_Color;

#endif