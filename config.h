#ifndef CONFIG_H
#define CONFIG_H
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <lua.h>
#include <lauxlib.h>
#include <luaconf.h>
#include <lualib.h>
#include <luajit.h>

#define ROSE_SCREEN_WIDTH 320
#define ROSE_SCREEN_HEIGHT 180
#define ROSE_SCREEN_SIZE (ROSE_SCREEN_WIDTH * ROSE_SCREEN_HEIGHT)
#define ROSE_INITIAL_WINDOW_WIDTH (ROSE_SCREEN_WIDTH * 2)
#define ROSE_INITIAL_WINDOW_HEIGHT (ROSE_SCREEN_HEIGHT * 2)
#define ROSE_PALETTE_SIZE (256 * 3)
#define ROSE_MEMORY_SIZE (1024 * 1024)
#define ROSE_RUNTIME_RESERVED_MEMORY_SIZE (1024 * 64)

#endif
