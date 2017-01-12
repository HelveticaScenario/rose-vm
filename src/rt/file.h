#pragma once

#include "enums.h"

struct rose_file {
    rose_file_type type;
    string name;
    bool on_disk;
    struct rose_file* parent;
    vector<rose_file*> contents;
    bool in_mem;
    uint8_t* buffer;
    size_t buffer_len;
    time_t last_modification;
    bool removed;
};
