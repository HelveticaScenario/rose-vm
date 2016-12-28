#include "rt/fs/fs_base.h"

void rose_file_free_recurse(rose_file* info) {
    if (info->name != NULL) {
        free((void*) info->name);
    }
    int i;
    for (i = 0; i < info->contents_len; i++ ) {
        rose_file_free_recurse(info->contents[i]);
    }
    free(info);
}



void rose_fs_free(rose_fs* fs) {
    rose_file_free_recurse(fs->root);
    free(fs);
}

rose_fs* rose_fs_create() {
    rose_fs *fs = (rose_fs *)malloc(sizeof(rose_fs));
    fs->root = NULL;
    fs->cart = NULL;
    return fs;
}

char* rose_construct_path(rose_file* file) {
    size_t file_count = 0;
    size_t arr_size = 64;
    rose_file** arr = (rose_file**)malloc(sizeof(*arr) * arr_size);
    size_t path_len = 1;
    while (file != NULL && file->parent != NULL) {
        arr[file_count] = file;
        path_len += strlen(file->name);
        file_count++;
        file = file->parent;
    }
    size_t num_path_separator = file_count < 1 ? 0 : (file_count - 1);
    char* path = (char*)malloc(sizeof(char) * (path_len + num_path_separator));
    memset(path, '\0', sizeof(char) * (path_len + num_path_separator));
    size_t i;
    for (i = file_count; i > 0; i--) {
        strcat(path, arr[i - 1]->name);
        if (i > 1) {
            #ifdef _WIN32
                strcat(path, "\\");
            #else
                strcat(path, "/");
            #endif
        }
    }

    return path;
}

void rose_fill_file_struct(rose_file** file, rose_file_type type, const char* name, off_t size, time_t last_disk_modification) {
    if (*file == NULL) {
        *file = (rose_file*)malloc(sizeof(**file));
        memset(*file, 0, sizeof(**file));
    }
    if (name != (*file)->name && (*file)->name != NULL) {
        free((void*) (*file)->name);
    }
    char* name_cpy = NULL;
    if (name == (*file)->name) {
        name_cpy = (char*) name;
    } else if (name != NULL) {
        name_cpy = (char*)malloc(strlen(name));
        strcpy(name_cpy, name);
    }
    rose_file file_stack = {
            .type = type,
            .name = name_cpy,
            .size = size,
            .last_disk_modification = last_disk_modification,
            .parent = (*file)->parent,
            .contents = (*file)->contents,
            .contents_len = (*file)->contents_len
    };

    memcpy(*file, &file_stack, sizeof(**file));
}

rose_file* rose_fs_fetch_cart_data_file(rose_file* cart_root) {
    rose_file* cart_data = NULL;
    int i;
    for (i = 0; i < cart_root->contents_len; i++) {
        if (cart_root->contents[i]->type == ROSE_DATA_FILE) {
            cart_data = cart_root->contents[i];
            break;
        }
    }
    return cart_data;
}

rose_file* rose_fs_fetch_cart_lua_main(rose_file* cart_root) {
    rose_file* lua_main = NULL;
    int i;
    for (i = 0; i < cart_root->contents_len; i++) {
        if (cart_root->contents[i]->type == ROSE_CODE_FILE && strcmp(cart_root->contents[i]->name, ROSE_MAIN_CODE_FILE_NAME) == 0) {
            lua_main = cart_root->contents[i];
            break;
        }
    }
    return lua_main;
}

rose_file* rose_fs_fetch_cart_root(rose_file* file) {
    fprintf(stderr, "rose_fs_fetch_cart_root unimplemented\n");
    exit(1);
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