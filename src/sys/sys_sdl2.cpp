#include "sys_sdl2.h"

// Renders screen
void render(rose_system_sdl2* s, SDL_Texture* texture);

// Pixel manipulators
bool lock_texture(void** pixels, int32_t* pitch, SDL_Texture* texture);

bool unlock_texture(void** pixels, int32_t* pitch, SDL_Texture* texture) ;

uint32_t get_screen_mult(rose_system_sdl2* s);

void make_screen_rect(rose_system_sdl2* s, SDL_Rect* rect);

string rose_sys_construct_full_path(rose_file* file);

rose_fs_error rose_sys_readfile(rose_file* file);
rose_fs_error rose_sys_writefile(rose_file* file);
rose_fs_error rose_sys_shutdown();

rose_file* rose_sys_recursive_file_create(const string& path, const string& name);

static string PERF_PATH = "";

string get_perf_path() {
    if (PERF_PATH == "") {
        PERF_PATH = SDL_GetPrefPath(ROSE_APP_NAME, "");
        auto len = PERF_PATH.length();
        if (PERF_PATH[len-2] == '/' || PERF_PATH[len-2] == '\\') {
            PERF_PATH.pop_back();
        }
    }
    return PERF_PATH;
}


bool rose_sys_sdl2_init(rose_system_sdl2* s, int argc, char* argv[]) {


    rose_init(SDL_GetBasePath());

    s->window = NULL;
    s->renderer = NULL;
    s->texture = NULL;
    s->fs = NULL;
    s->player = NULL;
    s->pixels = NULL;
    s->pitch = 0;
    s->window_width = 0;
    s->window_height = 0;
    s->width_mult = 0;
    s->height_mult = 0;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    // Set texture filtering to linear
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0")) {
        printf("Warning: Nearest Neighbor texture filtering not enabled!");
    }

    // Seed random
    srand(SDL_GetTicks());

    // Create window
    s->window = SDL_CreateWindow(
            "Rosebud", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            ROSE_INITIAL_WINDOW_WIDTH, ROSE_INITIAL_WINDOW_HEIGHT,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (s->window == NULL) {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    // Create renderer for window
    s->renderer = SDL_CreateRenderer(s->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (s->renderer == NULL) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    SDL_GL_GetDrawableSize(s->window, &(s->window_width), &(s->window_height));
    s->width_mult = (uint16_t) (s->window_width / ROSE_HD_SCREEN_WIDTH);
    s->height_mult = (uint16_t) (s->window_height / ROSE_HD_SCREEN_HEIGHT);

    // Initialize renderer color
    SDL_SetRenderDrawColor(s->renderer, 0, 0, 0, 0xFF);

    // Create uninitialized texture
    s->texture = SDL_CreateTexture(s->renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, ROSE_SCREEN_WIDTH, ROSE_SCREEN_HEIGHT);
    if (s->texture == NULL) {
        printf("Unable to create blank texture! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    s->hd_texture = SDL_CreateTexture(s->renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, ROSE_HD_SCREEN_WIDTH, ROSE_HD_SCREEN_HEIGHT);
    if (s->hd_texture == NULL) {
        printf("Unable to create blank hd texture! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    rose_fs* fs = rose_fs_create();
    fs->read_file = &rose_sys_readfile;
    fs->write_file = &rose_sys_writefile;
    fs->shutdown = &rose_sys_shutdown;

    string root_path = rose_sys_construct_full_path(NULL);
    rose_file* root = rose_sys_recursive_file_create(root_path, "");
    fs->root = root;
    s->player = new rose_desktop_player(fs);
    if (argc>1) {
        s->player->load_cart_path(argv[1]);
    }

    SDL_ShowCursor(SDL_DISABLE);
    return true;
}

void rose_sys_sdl2_run(rose_system_sdl2* s) {
    uint32_t windowID = SDL_GetWindowID(s->window);

    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event event;
    // While application is running
    SDL_StartTextInput();

    SDL_Rect screen_rect;
    make_screen_rect(s, &screen_rect);
    int i = 0;
    while (!quit) {
        s->player->save_input_frame();
        int16_t wheel_x = 0;
        int16_t wheel_y = 0;
        bool wheel_inverted = false;


        // Handle events on queue
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: {
                    quit = true;
                    break;
                }
                case SDL_WINDOWEVENT: {
                    if (event.window.windowID == windowID) {
                        switch (event.window.event) {

                            case SDL_WINDOWEVENT_SIZE_CHANGED: {
                                SDL_GL_GetDrawableSize(s->window, &(s->window_width), &(s->window_height));
                                s->width_mult = (uint16_t) (s->window_width / ROSE_HD_SCREEN_WIDTH);
                                s->height_mult = (uint16_t) (s->window_height / ROSE_HD_SCREEN_HEIGHT);
                                make_screen_rect(s, &screen_rect);
                                break;
                            }

                            case SDL_WINDOWEVENT_CLOSE: {
                                event.type = SDL_QUIT;
                                SDL_PushEvent(&event);
                                break;
                            }
                            default:break;
                        }
                    }
                    break;
                }
                case SDL_KEYUP:
                case SDL_KEYDOWN: {
                    rose_keycode code = sdl_scancode_to_rose_keycode(event.key.keysym.scancode);
                    s->player->call_onkey(code, event.key.state == SDL_PRESSED, event.key.repeat != 0);
                    break;
                }
                case SDL_MOUSEMOTION: {
                    int32_t mult = (int32_t) get_screen_mult(s);
                    if (mult == 0)
                        break;
                    if (!s->player->is_hd()) {
                        mult *= 2;
                    }
                    int16_t x = (int16_t) ((event.motion.x - ((int32_t) screen_rect.x)) / mult);
                    int16_t y = (int16_t) ((event.motion.y - ((int32_t) screen_rect.y)) / mult);
                    s->player->call_onmouse(x, y);
                    break;
                }
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP: {
                    bool pressed = event.button.state == SDL_PRESSED;
                    s->player->call_onbtn((uint8_t) (event.button.button + 9), pressed);
                    break;
                }
                case SDL_MOUSEWHEEL: {
                    int32_t x = event.wheel.x;
                    int32_t y = event.wheel.y;
                    if (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED) {
                        x *= -1;
                        y *= -1;
                        wheel_inverted = true;
                    } else {
                        wheel_inverted = false;
                    }
                    wheel_x += x;
                    wheel_y += y;
                    break;
                }
                default:break;
            }
        }

        s->player->call_onwheel(wheel_x, wheel_x, wheel_inverted);

        SDL_Texture* tex = NULL;
        bool hd = s->player->is_hd();
        if (hd) {
            tex = s->hd_texture;
        } else {
            tex = s->texture;
        }


        s->player->call_update();

        s->player->call_draw();


        if (lock_texture(&(s->pixels), &(s->pitch), tex)) {
            uint8_t* pixels = (uint8_t*) s->pixels;
            int pitch = s->pitch;

            rose_color color;

            auto screen_length = s->player->get_screen_length();
            for (auto item = 0; item < screen_length; ++item) {
                uint8_t c_index = s->player->get_color_index(item);
                s->player->get_color(color, c_index);
                pixels[(item * 3) + 0] = color.r;
                pixels[(item * 3) + 1] = color.g;
                pixels[(item * 3) + 2] = color.b;
            }
            unlock_texture(&(s->pixels), &(s->pitch), tex);
        }

        render(s, tex);
    }
}

void rose_sys_sdl2_free(rose_system_sdl2* s) {
    if (s->texture != NULL) {
        SDL_DestroyTexture(s->texture);
        s->texture = NULL;
        s->pixels = NULL;
        s->pitch = 0;
    }

    if (s->renderer != NULL) {
        SDL_DestroyRenderer(s->renderer);
        s->renderer = NULL;
    }

    if (s->window != NULL) {
        SDL_DestroyWindow(s->window);
        s->window = NULL;
    }

    if (s->player != NULL) {
        delete s->player;
        s->player = NULL;
    }

    if (s->fs != NULL) {
        rose_fs_free(s->fs);
        s->fs = NULL;
    }
    SDL_Quit();

    rose_deinit();
}

uint32_t get_screen_mult(rose_system_sdl2* s) {
    return s->width_mult < s->height_mult ? s->width_mult : s->height_mult;
}

void render(rose_system_sdl2* s, SDL_Texture* texture) {
    SDL_RenderClear(s->renderer);
    SDL_Rect rect;
    make_screen_rect(s, &rect);
    SDL_RenderCopy(s->renderer, texture, NULL, &rect);
    SDL_RenderPresent(s->renderer);
}

void make_screen_rect(rose_system_sdl2* s, SDL_Rect* rect) {
    uint32_t mult = get_screen_mult(s);
    rect->w = ROSE_HD_SCREEN_WIDTH * mult;
    rect->h = ROSE_HD_SCREEN_HEIGHT * mult;
    rect->x = (s->window_width - rect->w) / 2;
    rect->y = (s->window_height - rect->h) / 2;
}

bool lock_texture(void** pixels, int32_t* pitch, SDL_Texture* texture) {
    // Texture is already locked
    if (*(pixels) != NULL) {
        printf("Texture is already locked!\n");
        return false;
    }
    if (SDL_LockTexture(texture, NULL, pixels, pitch) != 0) {
        printf("Unable to lock texture! %s\n", SDL_GetError());
        return false;
    }
    return true;
}

bool unlock_texture(void** pixels, int32_t* pitch, SDL_Texture* texture) {
    // Texture is not locked
    if ((*pixels) == NULL) {
        printf("Texture is not locked!\n");
        return false;
    }

    SDL_UnlockTexture(texture);
    *(pixels) = NULL;
    *(pitch) = 0;

    return true;
}

string rose_sys_construct_full_path(rose_file* file) {
    if (file == NULL) {
        return get_perf_path();
    }
    return get_perf_path() + rose_construct_path(file);
}

rose_fs_error rose_sys_readfile(rose_file* file) {
    if (file->removed) {
        return ROSE_FS_FILE_REMOVED;
    }
    if (!file->on_disk) {
        return ROSE_FS_FILE_NOT_ON_DISK;
    }
    if (file->type == ROSE_DIRECTORY || file->type == ROSE_CART_DIRECTORY ) {
        return ROSE_FS_CANT_READ_DIR_ERR;
    }
    string full_path = rose_sys_construct_full_path(file);
    SDL_RWops *rw = SDL_RWFromFile(full_path.c_str(), "rb");
    if (rw == NULL)
        return ROSE_FS_CRITICAL_ERR;

    Sint64 res_size = SDL_RWsize(rw);
    if (file->in_mem) {
        file->buffer = (uint8_t*)realloc(file->buffer, res_size);
    } else {
        file->buffer = (uint8_t*)malloc(res_size);
    }
    memset(file->buffer, 0, (size_t) res_size);


    size_t nb_read_total = 0, nb_read = 1;
    uint8_t* buf = file->buffer;
    while (nb_read_total < res_size && nb_read != 0) {
        nb_read = (Sint64) SDL_RWread(rw, buf, sizeof(uint8_t), 1);
        nb_read_total += nb_read;
        buf += nb_read;
        if (nb_read_total > 48) {
            int i =0;
        }
    }
    SDL_RWclose(rw);
    if (nb_read_total != res_size) {
        return ROSE_FS_CRITICAL_ERR;
    }
    file->buffer_len = (size_t) nb_read_total;
    file->in_mem = true;
    file->last_modification = time(NULL);
    if(file->last_modification == -1) {
        fprintf(stderr, "err when getting current time");
        exit(1);
    }
    return ROSE_FS_NO_ERR;
}

// http://stackoverflow.com/questions/2256945/removing-a-non-empty-directory-programmatically-in-c-or-c
int remove_directory(const string& path)
{
    DIR *d = opendir(path.c_str());
    size_t path_len = path.size();
    int r = -1;

    // if (d)
    // {
    //     struct dirent *p;

    //     r = 0;

    //     while (!r && (p=readdir(d)))
    //     {
    //         int r2 = -1;
    //         char *buf;
    //         size_t len;

    //         /* Skip the names "." and ".." as we don't want to recurse on them. */
    //         if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
    //         {
    //             continue;
    //         }

    //         len = path_len + strlen(p->d_name) + 2;
    //         buf = (char*) malloc(len);

    //         if (buf)
    //         {
    //             struct stat statbuf;

    //             snprintf(buf, len, "%s/%s", path.c_str(), p->d_name);

    //             if (!stat(buf, &statbuf))
    //             {
    //                 if (S_ISDIR(statbuf.st_mode))
    //                 {
    //                     r2 = remove_directory(buf);
    //                 }
    //                 else
    //                 {
    //                     r2 = unlink(buf);
    //                 }
    //             }

    //             free(buf);
    //         }

    //         r = r2;
    //     }

    //     closedir(d);
    // }

    // if (!r)
    // {
    //     r = rmdir(path.c_str());
    // }

    return r;
}

rose_fs_error rose_sys_writefile(rose_file* file) {
    string full_path = rose_sys_construct_full_path(file);
    if (file->removed) {
        if (file->type == ROSE_DIRECTORY || file->type == ROSE_CART_DIRECTORY ) {
            int err = remove_directory(full_path);
            if (err != 0) {
                fprintf(stderr, "remove_directory error %s %d\n", full_path.c_str(), errno);
                return ROSE_FS_CRITICAL_ERR;
            }
            return ROSE_FS_NO_ERR;
        } else {
            // int err = unlink(full_path.c_str());
            // if (err != 0) {
            //     fprintf(stderr, "remove error %s %d\n", full_path.c_str(), errno);
                return ROSE_FS_CRITICAL_ERR;
            // }
            // return ROSE_FS_NO_ERR;
        }
    }
    if (file->type == ROSE_DIRECTORY || file->type == ROSE_CART_DIRECTORY ) {
        if (file->on_disk) {
            return ROSE_FS_NO_ERR;
        }
        int status = mkdir(full_path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if (status == 0 || errno == EEXIST) {
            file->on_disk = true;
            return ROSE_FS_NO_ERR;
        } else {
            return ROSE_FS_CRITICAL_ERR;
        }
    }
    SDL_RWops *rw = SDL_RWFromFile(full_path.c_str(), "wb");
    if (rw == NULL) {
        return ROSE_FS_CRITICAL_ERR;
    }

    size_t write_len = SDL_RWwrite(rw, file->buffer, sizeof(uint8_t), file->buffer_len);
    SDL_RWclose(rw);
    if (write_len != (sizeof(uint8_t) * file->buffer_len)) {
        return ROSE_FS_CRITICAL_ERR;
    }

    return ROSE_FS_NO_ERR;
}

rose_fs_error rose_sys_shutdown() {
    fprintf(stderr, "rose_sys_shutdown unimplemented\n");
    exit(1);
}

rose_file* rose_sys_recursive_file_create(const string& path, const string& name) {
    struct stat attrib;
    stat(path.c_str(), &attrib);

    rose_file_type type;
    if ((attrib.st_mode & S_IFMT) == S_IFDIR) {
        type = ROSE_DIRECTORY;
    } else if((attrib.st_mode & S_IFMT) == S_IFREG) {
        if (name == ROSE_DATA_FILE_NAME) {
            type = ROSE_DATA_FILE;
        } else if (name == ROSE_INFO_FILE_NAME) {
            type = ROSE_INFO_FILE;
        } else if (name.substr(name.size() - 3) == ROSE_CODE_FILE_SUFFIX) {
            type = ROSE_CODE_FILE;
        } else {
            return NULL;
        }
    } else {
        return NULL;
    }

    rose_file* file = new rose_file();
    file->type = type;
    file->name = name;
    file->on_disk = true;
    file->last_modification = attrib.st_mtime;
    file->in_mem = false;
    file->buffer_len = 0;
    file->buffer = NULL;
    file->parent = NULL;
    file->contents = {};
    file->removed = false;

    vector<rose_file*> arr = {};

    if (type == ROSE_DIRECTORY) {
        DIR *dp;
        struct dirent *ep;
        dp = opendir (path.c_str());
        if (dp != NULL)
        {

            while ((ep = readdir(dp))) {
                if (strcmp(ep->d_name,".") == 0
                    || strcmp(ep->d_name,"..") == 0
                    || strcmp(ep->d_name,".DS_Store") == 0
                    || strcmp(ep->d_name,".git") == 0) {
                    continue;
                }
                if (ep->d_type == DT_REG || ep->d_type == DT_DIR) { /* If the entry is a regular file or directory*/
                    string new_name = ep->d_name;

                    string new_path = path;

                    #ifdef _WIN32
                        new_path += '\\';
                    #else
                        new_path += '/';
                    #endif
                    new_path += new_name;
                    rose_file* new_file = rose_sys_recursive_file_create(new_path, new_name);
                    if (new_file != NULL) {
                        new_file->parent = file;
                        arr.push_back(new_file);
                    }

                }
            }

            (void) closedir (dp);
        }
        else {
            perror ("Couldn't open the directory");
            exit(1);
        }
    }



    file->contents = arr;
    if (rose_fs_fetch_cart_data_file(file) != NULL &&
            rose_fs_fetch_cart_js_main(file) != NULL) {
        file->type = ROSE_CART_DIRECTORY;
    }

    return file;
}
