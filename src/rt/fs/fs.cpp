#include "rt/fs/fs.h"

void rose_file_free_recurse(rose_file* info) {
    if (info->buffer != NULL) {
        free(info->buffer);
    }
    if (info->contents.size() != 0) {
        for (auto it = info->contents.begin(); it != info->contents.end(); ++it) {
            rose_file_free_recurse(*it);
        }
    }
    free(info);
}

void rose_fs_free(rose_fs* fs) {
    rose_file_free_recurse(fs->root);
    free(fs);
}

rose_fs* rose_fs_create() {
    rose_fs *fs = new rose_fs();
    fs->root = NULL;
    fs->cart = NULL;
    fs->pwd = NULL;
    return fs;
}

string rose_construct_path(rose_file* file) {
    vector<rose_file*> arr = {};

    while (file != NULL && file->parent != NULL) {
        arr.push_back(file);
        file = file->parent;
    }
    string path = "";
    auto i = arr.size();
    for (auto rit = arr.rbegin(); rit != arr.rend(); ++rit) {
        path += (*rit)->name;
        if (i > 1) {
            #ifdef _WIN32
                path += '\\';
            #else
                path += '/';
            #endif
        }
        --i;
    }

    return path;
}

void rose_fs_add_child(rose_file* parent, rose_file* child) {
    parent->contents.push_back(child);
}

void rose_fs_remove_child(rose_file* parent, rose_file* child) {
    size_t index;
    if (rose_fs_fetch_child_and_index(parent, child->name, index) != NULL) {
        parent->contents.erase(parent->contents.begin()+index);
    }
}

rose_file* rose_fs_fetch_cart_data_file(rose_file* cart_root) {
    if (cart_root->contents.size() == 0) {
        return NULL;
    }
    rose_file* data = NULL;
    for (auto it = cart_root->contents.begin(); it != cart_root->contents.end(); ++it) {
        auto file = *it;
        if (file->type == ROSE_DATA_FILE) {
            data = file;
            break;
        }
    }
    return data;
}

rose_file* rose_fs_fetch_cart_js_main(rose_file* cart_root) {
    if (cart_root->contents.size() == 0) {
        return NULL;
    }
    rose_file* js_main = NULL;
    for (auto it = cart_root->contents.begin(); it != cart_root->contents.end(); ++it) {
        auto file = *it;
        if (file->type == ROSE_CODE_FILE && file->name == ROSE_MAIN_CODE_FILE_NAME) {
            js_main = file;
            break;
        }
    }
    return js_main;
}

rose_file* rose_fs_fetch_cart_root(rose_file* file) {
    printf("rose_fs_fetch_cart_root should work, but it is untested\n");

    while (file != NULL && file->type != ROSE_CART_DIRECTORY) {
        file = file->parent;
    }
    return file;
}

rose_file* rose_fs_fetch_neighbor(rose_file* file, const string& neighbor_name) {
    if (file->parent == NULL) {
        return NULL;
    }

    return rose_fs_fetch_child(file->parent, neighbor_name);
}

rose_file* rose_fs_fetch_child(rose_file* file, const string& child_name) {
    size_t _;
    return rose_fs_fetch_child_and_index(file, child_name, _);
}

rose_file* rose_fs_fetch_child_and_index(rose_file* file, const string& child_name, size_t& index) {
    if (file->contents.size() == 0) {
        return NULL;
    }
    for (auto it = file->contents.begin(); it != file->contents.end(); ++it) {
        auto selected = *it;
        if (selected->name == child_name) {
            index = (size_t) (it - file->contents.begin());
            return selected;
        }
    }
    return NULL;
}
//
//void archive_test(const char *base_path) {
//
//    const char *config_fname = "config.txt.zip";
//    char *config_fpath = NULL;
//    config_fpath = (char*) malloc(strlen(base_path) + strlen(config_fname) + 1);
//    strcpy(config_fpath, base_path);
//    strcat(config_fpath, config_fname);
//    printf("%s\n", config_fpath);
//    struct stat file_stat;
//    int res;
//    res = stat(config_fpath, &file_stat);
//    printf("%d\n", res);
//    printf("%d\n", S_ISDIR(file_stat.st_mode));
//    res = stat(base_path, &file_stat);
//    printf("%d\n", S_ISDIR(file_stat.st_mode));
//    printf("%d\n", res);
//    printf("%s\n", base_path);
//
//    struct archive *a;
//    struct archive_entry *entry;
//    int r;
//
//    a = archive_read_new();
//    archive_read_support_filter_all(a);
//    archive_read_support_format_all(a);
//    r = archive_read_open_filename(
//        a,
//        "/Users/contrarian/Library/Application Support/Rosebud/config.txt.zip",
//        10240); // Note 1
//    if (r != ARCHIVE_OK)
//        exit(1);
//    while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
//        printf("%s\\n", archive_entry_pathname(entry));
//        archive_read_data_skip(a); // Note 2
//    }
//    r = archive_read_free(a); // Note 3
//    if (r != ARCHIVE_OK)
//        exit(1);
//}