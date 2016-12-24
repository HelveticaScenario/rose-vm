#include <types.h>
#include "rt/fs/fs_base.h"

void rose_file_info_free(rose_file_info* info) {
    if (info->name != NULL) {
        free((void*) info->name);
    }
    if (info->path != NULL) {
        free((void*) info->path);
    }
    free(info);
}

void rose_cartridge_free(rose_cartridge *cart) {
    int i;
    for (i = 0; i < cart->code_size; i++) {
        rose_file_info_free(cart->code[i]);
    }
    free(cart->code);
    free(cart->data);
    free(cart);
}

void rose_fs_free(rose_fs* fs) {
    rose_cartridge_free(fs->cart);
    free(fs);
}

rose_fs* rose_fs_create() {
    rose_fs *fs = (rose_fs *)malloc(sizeof(rose_fs));
    fs->cart = rose_cartridge_create();
    return fs;
}


rose_cartridge* rose_cartridge_create() {
    rose_cartridge *cart = (rose_cartridge *)malloc(sizeof(rose_cartridge));
    cart->code_size = 0;
    cart->code = NULL;
    cart->data_size = 0; // ROSE_MEMORY_SIZE - ROSE_RUNTIME_RESERVED_MEMORY_SIZE;
    cart->data = NULL; //(uint8_t *)malloc(cart->data_size);
//    memset(cart->data, 0, cart->data_size);
//    memcpy(
//        cart->data + cart->data_size - ROSE_PALETTE_SIZE, rose_default_palette,
//        sizeof(rose_default_palette));
    return cart;
}



void archive_test(const char *base_path) {

    const char *config_fname = "config.txt.zip";
    char *config_fpath = NULL;
    config_fpath = malloc(strlen(base_path) + strlen(config_fname) + 1);
    strcpy(config_fpath, base_path);
    strcat(config_fpath, config_fname);
    printf("%s\n", config_fpath);
    struct stat file_stat;
    int res;
    res = stat(config_fpath, &file_stat);
    printf("%d\n", res);
    printf("%d\n", S_ISDIR(file_stat.st_mode));
    res = stat(base_path, &file_stat);
    printf("%d\n", S_ISDIR(file_stat.st_mode));
    printf("%d\n", res);
    printf("%s\n", base_path);

    struct archive *a;
    struct archive_entry *entry;
    int r;

    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);
    r = archive_read_open_filename(
        a,
        "/Users/contrarian/Library/Application Support/Rosebud/config.txt.zip",
        10240); // Note 1
    if (r != ARCHIVE_OK)
        exit(1);
    while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
        printf("%s\\n", archive_entry_pathname(entry));
        archive_read_data_skip(a); // Note 2
    }
    r = archive_read_free(a); // Note 3
    if (r != ARCHIVE_OK)
        exit(1);
}