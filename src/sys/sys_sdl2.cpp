#include "sys/sys_sdl2.h"

// Renders screen
void render(rose_system_sdl2* s);

// Pixel manipulators
bool lock_texture(rose_system_sdl2* s);

bool unlock_texture(rose_system_sdl2* s);

uint32_t get_screen_mult(rose_system_sdl2* s);

void make_screen_rect(rose_system_sdl2* s, SDL_Rect* rect);

char* rose_sys_construct_full_path(rose_file* file);

rose_fs_error rose_sys_readfile(rose_file* file, uint8_t** buffer, size_t* buffer_len);
rose_fs_error rose_sys_writefile(rose_file* file, uint8_t* buffer, size_t buffer_len);
rose_fs_error rose_sys_update_file(rose_file* old_file, rose_file* new_file);
rose_fs_error rose_sys_new_file(rose_file* parent, rose_file** res, const char* name, const rose_file_type file_type);
rose_fs_error rose_sys_shutdown();

rose_file* rose_sys_recursive_file_create(const char* path, const char* name);

static char* PERF_PATH = NULL;

char* get_perf_path() {
    if (PERF_PATH == NULL) {
        char* path = SDL_GetPrefPath(ROSE_APP_NAME, "");
        size_t len = strlen(path);
        if (len == 1) {
            PERF_PATH = (char *)malloc(len+1);
            memcpy(PERF_PATH, path, len+1);
        } else if (path[len-2] == '/' || path[len-2] == '\\') {
            PERF_PATH = (char *)malloc(len);
            memcpy(PERF_PATH, path, len-1);
            PERF_PATH[len-1] = '\0';
        }
        SDL_free(path);
    }
    return PERF_PATH;
}


bool rose_sys_sdl2_init(rose_system_sdl2* s, int argc, char* argv[]) {

    rose_init(SDL_GetBasePath());

    s->window = NULL;
    s->renderer = NULL;
    s->texture = NULL;
    s->fs = NULL;
    // s->editor = NULL;
    s->game = NULL;
    s->screen_mode = ROSE_GAMEMODE;
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
    s->width_mult = (uint16_t) (s->window_width / ROSE_SCREEN_WIDTH);
    s->height_mult = (uint16_t) (s->window_height / ROSE_SCREEN_HEIGHT);

    // Initialize renderer color
    SDL_SetRenderDrawColor(s->renderer, 0, 0, 0, 0xFF);

    // Create uninitialized texture
    s->texture = SDL_CreateTexture(s->renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, ROSE_SCREEN_WIDTH, ROSE_SCREEN_HEIGHT);
    if (s->texture == NULL) {
        printf("Unable to create blank texture! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    rose_fs* fs = rose_fs_create();
    fs->read_file = &rose_sys_readfile;
    fs->write_file = &rose_sys_writefile;
    fs->update_file = &rose_sys_update_file;
    fs->new_file = &rose_sys_new_file;
    fs->shutdown = &rose_sys_shutdown;

    char* root_path = rose_sys_construct_full_path(NULL);
    rose_file* root = rose_sys_recursive_file_create(root_path, "");
    fs->root = root;
    fs->pwd = root;
    fs->cart = NULL;


    // TODO: Remove this
    fs->cart = fs->root->contents[0];

    s->game = rose_runtime_game_create(fs);
    rose_runtime_game_error err = rose_runtime_game_init(s->game);
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

    rose_mousestate mousestate;
    mousestate.x = 0;
    mousestate.y = 0;
    mousestate.left_btn_down = false;
    mousestate.right_btn_down = false;
    mousestate.middle_btn_down = false;
    mousestate.x1_btn_down = false;
    mousestate.x2_btn_down = false;
    mousestate.wheel_x = 0;
    mousestate.wheel_y = 0;
    bool wheel_changed = false;
    SDL_Rect screen_rect;
    make_screen_rect(s, &screen_rect);
    rose_runtime_game_error err = ROSE_RT_GAME_NO_ERR;
    while (!quit) {
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
                                s->width_mult = (uint16_t) (s->window_width / ROSE_SCREEN_WIDTH);
                                s->height_mult = (uint16_t) (s->window_height / ROSE_SCREEN_HEIGHT);
                                make_screen_rect(s, &screen_rect);
                                printf("%d %d\n", s->width_mult, s->height_mult);
                                break;
                            }

                            case SDL_WINDOWEVENT_CLOSE: {
                                event.type = SDL_QUIT;
                                SDL_PushEvent(&event);
                                break;
                            }
                        }
                    }
                    break;
                }
                case SDL_KEYUP:
                case SDL_KEYDOWN: {
                    rose_keycode code = sdl_scancode_to_rose_keycode(event.key.keysym.scancode);
                    rose_runtime_base_update_keystate(s->game->base, code, event.key.state == SDL_PRESSED);
                    err = rose_runtime_game_onkey(s->game, code, event.key.state == SDL_PRESSED, event.key.repeat != 0);
                    switch (err) {
                        case ROSE_RT_GAME_CRITICAL_ERR:
                            quit = true;
                            break;
                        default:
                            break;
                    }
                    break;
                }
                case SDL_MOUSEMOTION: {
                    int32_t mult = (int32_t) get_screen_mult(s);
                    if (mult == 0)
                        break;
                    mousestate.x = (int16_t) ((event.motion.x - ((int32_t) screen_rect.x)) / mult);
                    mousestate.y = (int16_t) ((event.motion.y - ((int32_t) screen_rect.y)) / mult);
                    rose_runtime_base_update_mousestate(s->game->base, &mousestate);
                    err = rose_runtime_game_onmouse(s->game, mousestate.x, mousestate.y);
                    switch (err) {
                        case ROSE_RT_GAME_CRITICAL_ERR:
                            quit = true;
                            break;
                        default:
                            break;
                    }
                    break;
                }
                case SDL_MOUSEBUTTONDOWN: {
                    switch (event.button.button) {
                        case SDL_BUTTON_LEFT:
                            mousestate.left_btn_down = true;
                            rose_runtime_base_update_mousestate(s->game->base, &mousestate);
                            err = rose_runtime_game_onbtn(s->game, ROSE_LEFT_MOUSE_IDX, true);
                            break;
                        case SDL_BUTTON_RIGHT:
                            mousestate.right_btn_down = true;
                            rose_runtime_base_update_mousestate(s->game->base, &mousestate);
                            err = rose_runtime_game_onbtn(s->game, ROSE_RIGHT_MOUSE_IDX, true);
                            break;
                        case SDL_BUTTON_MIDDLE:
                            mousestate.middle_btn_down = true;
                            rose_runtime_base_update_mousestate(s->game->base, &mousestate);
                            err = rose_runtime_game_onbtn(s->game, ROSE_MIDDLE_MOUSE_IDX, true);
                            break;
                        case SDL_BUTTON_X1:
                            mousestate.x1_btn_down = true;
                            rose_runtime_base_update_mousestate(s->game->base, &mousestate);
                            err = rose_runtime_game_onbtn(s->game, ROSE_X1_MOUSE_IDX, true);
                            break;
                        case SDL_BUTTON_X2:
                            mousestate.x2_btn_down = true;
                            rose_runtime_base_update_mousestate(s->game->base, &mousestate);
                            err = rose_runtime_game_onbtn(s->game, ROSE_X2_MOUSE_IDX, true);
                            break;
                        default:
                            break;
                    }
                    switch (err) {
                        case ROSE_RT_GAME_CRITICAL_ERR:
                            quit = true;
                            break;
                        default:
                            break;
                    }
                    break;
                }
                case SDL_MOUSEBUTTONUP: {
                    switch (event.button.button) {
                        case SDL_BUTTON_LEFT:
                            mousestate.left_btn_down = false;
                            rose_runtime_base_update_mousestate(s->game->base, &mousestate);
                            err = rose_runtime_game_onbtn(s->game, ROSE_LEFT_MOUSE_IDX, false);
                            break;
                        case SDL_BUTTON_RIGHT:
                            mousestate.right_btn_down = false;
                            rose_runtime_base_update_mousestate(s->game->base, &mousestate);
                            err = rose_runtime_game_onbtn(s->game, ROSE_RIGHT_MOUSE_IDX, false);
                            break;
                        case SDL_BUTTON_MIDDLE:
                            mousestate.middle_btn_down = false;
                            rose_runtime_base_update_mousestate(s->game->base, &mousestate);
                            err = rose_runtime_game_onbtn(s->game, ROSE_MIDDLE_MOUSE_IDX, false);
                            break;
                        case SDL_BUTTON_X1:
                            mousestate.x1_btn_down = false;
                            rose_runtime_base_update_mousestate(s->game->base, &mousestate);
                            err = rose_runtime_game_onbtn(s->game, ROSE_X1_MOUSE_IDX, false);
                            break;
                        case SDL_BUTTON_X2:
                            mousestate.x2_btn_down = false;
                            rose_runtime_base_update_mousestate(s->game->base, &mousestate);
                            err = rose_runtime_game_onbtn(s->game, ROSE_X2_MOUSE_IDX, false);
                            break;
                        default:
                            break;
                    }
                    switch (err) {
                        case ROSE_RT_GAME_CRITICAL_ERR:
                            quit = true;
                            break;
                        default:
                            break;
                    }
                    break;
                }
                case SDL_MOUSEWHEEL: {
                    int32_t x = event.wheel.x;
                    int32_t y = event.wheel.y;
                    if (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED) {
                        x *= -1;
                        y *= -1;
                        mousestate.wheel_inverted = true;
                    } else {
                        mousestate.wheel_inverted = false;
                    }
                    mousestate.wheel_x += x;
                    mousestate.wheel_y += y;
                    wheel_changed = true;
                    break;
                }
            }
            // User requests quit
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
        if (wheel_changed) {
            rose_runtime_base_update_mousestate(s->game->base, &mousestate);
            err = rose_runtime_game_onwheel(s->game, mousestate.wheel_x, mousestate.wheel_x, mousestate.wheel_inverted);
            switch (err) {
                case ROSE_RT_GAME_CRITICAL_ERR:
                    quit = true;
                    break;
                default:
                    break;
            }
            wheel_changed = false;
        }
        err = rose_runtime_game_update(s->game);
        switch (err) {
            case ROSE_RT_GAME_CRITICAL_ERR:
                quit = true;
                break;
            default:
                break;
        }

        err = rose_runtime_game_draw(s->game);
        switch (err) {
            case ROSE_RT_GAME_CRITICAL_ERR:
                quit = true;
                break;
            default:
                break;
        }

        mousestate.wheel_x = 0;
        mousestate.wheel_y = 0;

        if (lock_texture(s)) {
            uint8_t* pixels = (uint8_t*) s->pixels;
            int pitch = s->pitch;

            int pixel_count = (pitch) * ROSE_SCREEN_HEIGHT;

            rose_memory_range* palette = s->game->base->palette;
            rose_memory_range* screen = s->game->base->screen;
            rose_memory_iterator it = screen->begin;
            for (; it < screen->end; it = rose_memory_iterator_next(it)) {
                uint16_t i = (uint16_t) (it - screen->begin);
                uint8_t c = *it;
                pixels[(i * 3) + 0] = *(palette->begin + (c * 3) + 0);
                pixels[(i * 3) + 1] = *(palette->begin + (c * 3) + 1);
                pixels[(i * 3) + 2] = *(palette->begin + (c * 3) + 2);
            }
            unlock_texture(s);
        }
        render(s);
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

    if (s->game != NULL) {
        rose_runtime_game_free(s->game);
        s->game = NULL;
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

void render(rose_system_sdl2* s) {
    SDL_RenderClear(s->renderer);
    SDL_Rect rect;
    make_screen_rect(s, &rect);
    SDL_RenderCopy(s->renderer, s->texture, NULL, &rect);
    SDL_RenderPresent(s->renderer);
}

void make_screen_rect(rose_system_sdl2* s, SDL_Rect* rect) {
    uint32_t mult = get_screen_mult(s);
    rect->w = ROSE_SCREEN_WIDTH * mult;
    rect->h = ROSE_SCREEN_HEIGHT * mult;
    rect->x = (s->window_width - rect->w) / 2;
    rect->y = (s->window_height - rect->h) / 2;
}

bool lock_texture(rose_system_sdl2* s) {
    // Texture is already locked
    if (s->pixels != NULL) {
        printf("Texture is already locked!\n");
        return false;
    }

    if (SDL_LockTexture(s->texture, NULL, &(s->pixels), &(s->pitch)) != 0) {
        printf("Unable to lock texture! %s\n", SDL_GetError());
        return false;
    }

    return true;
}

bool unlock_texture(rose_system_sdl2* s) {
    // Texture is not locked
    if (s->pixels == NULL) {
        printf("Texture is not locked!\n");
        return false;
    }

    SDL_UnlockTexture(s->texture);
    s->pixels = NULL;
    s->pitch = 0;

    return true;
}

char* rose_sys_construct_full_path(rose_file* file) {
    if (file == NULL) {
        return get_perf_path();
    }
    char* path = rose_construct_path(file);
    char* base_path = get_perf_path();
    size_t base_path_len = strlen(base_path);
    size_t abs_path_len = strlen(path);
    char* full_path = (char*)malloc(base_path_len + abs_path_len + 1);
    memset(full_path, '\0', base_path_len + abs_path_len + 1);
    strcpy(full_path, base_path);
    strcat(full_path, path);
    free(path);
    return full_path;
}

rose_fs_error rose_sys_readfile(rose_file* file, uint8_t** buffer, size_t* buffer_len) {
    char* full_path = rose_sys_construct_full_path(file);
    SDL_RWops *rw = SDL_RWFromFile(full_path, "rb");
    free(full_path);
    if (rw == NULL)
        return ROSE_FS_CRITICAL_ERR;

    Sint64 res_size = SDL_RWsize(rw);
    uint8_t* res = (uint8_t*)malloc((size_t) (res_size + 1));

    Sint64 nb_read_total = 0, nb_read = 1;
    uint8_t* buf = res;
    while (nb_read_total < res_size && nb_read != 0) {
        nb_read = (Sint64) SDL_RWread(rw, buf, sizeof(uint8_t), res_size - nb_read_total);
        nb_read_total += nb_read;
        buf += nb_read;
    }
    SDL_RWclose(rw);
    if (nb_read_total != res_size) {
        free(res);
        return ROSE_FS_CRITICAL_ERR;
    }
    *buffer = res;
    *buffer_len = (size_t) nb_read_total;
    return ROSE_FS_NO_ERR;
}

rose_fs_error rose_sys_writefile(rose_file* file, uint8_t* buffer, size_t buffer_len) {
    fprintf(stderr, "rose_sys_writefile unimplemented\n");
    exit(1);
}

rose_fs_error rose_sys_update_file(rose_file* old_file, rose_file* new_file) {
    fprintf(stderr, "rose_sys_update_file unimplemented\n");
    exit(1);
}

rose_fs_error rose_sys_new_file(rose_file* parent, rose_file** res, const char* name, rose_file_type file_type) {
    fprintf(stderr, "rose_sys_new_file unimplemented\n");
    exit(1);
}

rose_fs_error rose_sys_shutdown() {
    fprintf(stderr, "rose_sys_shutdown unimplemented\n");
    exit(1);
}

rose_file* rose_sys_recursive_file_create(const char* path, const char* name) {

    struct stat attrib;
    stat(path, &attrib);
    rose_file* file = NULL;

    rose_file_type type;
    if ((attrib.st_mode & S_IFMT) == S_IFDIR) {
        type = ROSE_DIRECTORY;
    } else if((attrib.st_mode & S_IFMT) == S_IFREG) {
        if (strcmp(name, ROSE_DATA_FILE_NAME) == 0) {
            type = ROSE_DATA_FILE;
        } else if (strcmp(name + (strlen(name) - strlen(ROSE_CODE_FILE_SUFFIX)), ROSE_CODE_FILE_SUFFIX) == 0) {
            type = ROSE_CODE_FILE;
        } else {
            return NULL;
        }
    } else {
        return NULL;
    }

    rose_fill_file_struct(&file, type, name, attrib.st_size, attrib.st_mtime);

    size_t file_count = 0;
    size_t arr_size = 64;
    rose_file** arr = (rose_file**)malloc(sizeof(*arr) * arr_size);

    if (type == ROSE_DIRECTORY) {
        DIR *dp;
        struct dirent *ep;
        dp = opendir (path);
        if (dp != NULL)
        {

            while ((ep = readdir(dp))) {
                if (strcmp(ep->d_name,".") == 0 || strcmp(ep->d_name,"..") == 0 || strcmp(ep->d_name,".DS_Store") == 0) {
                    continue;
                }
                if (ep->d_type == DT_REG || ep->d_type == DT_DIR) { /* If the entry is a regular file or directory*/
                    file_count++;
                    if (file_count > arr_size) {
                        arr_size *= 2;
                        arr = (rose_file**) realloc(arr, sizeof(*arr) * arr_size);
                    }
                    char* new_name = (char*) malloc(sizeof(char) * (ep->d_namlen + 1));
                    memset(new_name, '\0', sizeof(char) * (ep->d_namlen + 1));
                    strcpy(new_name, ep->d_name);

                    char* new_path = (char*) malloc(sizeof(char) * (strlen(path) + ep->d_namlen + 1));
                    memset(new_path, '\0', sizeof(char) * (strlen(path) + ep->d_namlen + 2));
                    strcpy(new_path, path);

                    #ifdef _WIN32
                        if (path[strlen(path) - 1] != '\\') {
                            strcat(new_path, "\\");
                        }
                    #else
                        if (path[strlen(path) - 1] != '/') {
                            strcat(new_path, "/");
                        }
                    #endif
                    strcat(new_path, ep->d_name);
                    rose_file* new_file = rose_sys_recursive_file_create(new_path, new_name);
                    free(new_name);
                    free(new_path);
                    if (new_file == NULL) {
                        file_count--;
                        continue;
                    } else {
                        new_file->parent = file;
                        arr[file_count-1] = new_file;
                    }

                }
            }

            (void) closedir (dp);
        }
        else {
            perror ("Couldn't open the directory");
            free(arr);
            exit(1);
        }
    }

    if (file_count == 0) {
        free(arr);
        arr = NULL;
    } else {
        arr = (rose_file**) realloc(arr, sizeof(*arr) * file_count);
    }

    file->contents = arr;
    file->contents_len = file_count;
    if (rose_fs_fetch_cart_data_file(file) != NULL && rose_fs_fetch_cart_lua_main(file) != NULL) {
        rose_fill_file_struct(&file, ROSE_CART_DIRECTORY, file->name, file->size, file->last_disk_modification);
    }

    return file;
}
