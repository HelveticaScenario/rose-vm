#pragma once

#include "enums.h"

typedef rose_fs_error (*rose_fs_hook_read_file)(rose_file* file);
typedef rose_fs_error (*rose_fs_hook_write_file)(rose_file* file);
typedef rose_fs_error (*rose_fs_hook_shutdown)();
typedef rose_fs_error (*rose_fs_hook_get_base_path)(char** path);

struct rose_fs {
    rose_file* root;
    rose_fs_hook_read_file read_file;
    rose_fs_hook_write_file write_file;
    rose_fs_hook_shutdown shutdown;
    rose_fs_hook_get_base_path get_base_path;
};

void rose_file_free_recurse(rose_file* info);

string rose_construct_path(rose_file* file);

rose_fs* rose_fs_create();

void rose_fs_free(rose_fs* fs);

rose_file* rose_fs_fetch_cart_data_file(rose_file* cart_root);

rose_file* rose_fs_fetch_cart_info_file(rose_file* cart_root);

rose_file* rose_fs_fetch_cart_js_main(rose_file* cart_root);

rose_file* rose_fs_fetch_cart_root(rose_file* file);

rose_file* rose_fs_fetch_neighbor(rose_file* file, const string& neighbor_name);

rose_file* rose_fs_fetch_child(rose_file* file, const string& child_name);

rose_file* rose_fs_fetch_child_and_index(rose_file* file, const string& child_name, size_t& index);

void rose_fs_add_child(rose_file* parent, rose_file* child);

void rose_fs_remove_child(rose_file* parent, rose_file* child);

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
