#include "rt/fs/fs_cartridge.h"

Rose_Cartridge *rose_cartridge_create() {
    Rose_Cartridge *cart = (Rose_Cartridge *)malloc(sizeof(Rose_Cartridge));
    cart->code_size = 0;
    cart->code = (char *)malloc((cart->code_size + 1) * sizeof(char));
    cart->code[cart->code_size] = '\0';
    cart->data_size = ROSE_MEMORY_SIZE - ROSE_RUNTIME_RESERVED_MEMORY_SIZE;
    cart->data = (uint8_t *)malloc(cart->data_size);
    memset(cart->data, 0, cart->data_size);
    memcpy(
        cart->data + cart->data_size - ROSE_PALETTE_SIZE, rose_default_palette,
        sizeof(rose_default_palette));
    return cart;
}

void rose_cartridge_free(Rose_Cartridge *cart) {
    free(cart->code);
    free(cart->data);
    free(cart);
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