#ifndef ROSE_FS_COMMON_H
#define ROSE_FS_COMMON_H

#include "config.h"
#include <string.h>
#include <zlib.h>
#include <archive.h>
#include <archive_entry.h>

typedef enum { ROSE_FILE_DISK, ROSE_FILE_MEMORY } Rose_FileType;
typedef struct {

} Rose_FileMemory;
typedef struct {

} Rose_FileDisk;

typedef struct {
    Rose_FileType type;
    Rose_FileMemory memory;
    Rose_FileDisk disk;
    char *name;
    uint32_t name_len;
} Rose_File;

#endif