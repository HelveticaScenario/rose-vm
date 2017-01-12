#include "sys/sys_sdl2.h"

// Renders screen
void render(rose_system_sdl2* s);

// Pixel manipulators
bool lock_texture(rose_system_sdl2* s);

bool unlock_texture(rose_system_sdl2* s);

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
    s->editor = NULL;
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
    fs->shutdown = &rose_sys_shutdown;

    string root_path = rose_sys_construct_full_path(NULL);
    rose_file* root = rose_sys_recursive_file_create(root_path, "");
    fs->root = root;
    fs->pwd = root;
    fs->cart = NULL;

    if (argc>1) {
        fs->cart = rose_fs_fetch_child(fs->root, argv[1]);
        if (fs->cart == NULL) {
            fs->cart = fs->root->contents[0];
        }
    } else {
        // TODO: Remove this
        fs->cart = fs->root->contents[0];
    }


    s->game = rose_game_create(fs);
    rose_game_error err = rose_game_init(s->game);

//    fs->cart = fs->root->contents[1];
//
//    s->editor = rose_game_create(fs);
//    rose_game_error err2 = rose_game_init(s->editor);
//    fs->cart = fs->root->contents[0];
    SDL_ShowCursor(SDL_DISABLE);
    return true;
}

rose_rt* get_present_base(rose_system_sdl2* s) {
    if (s->screen_mode == ROSE_GAMEMODE) {
        return s->game->rt;
    } else {
//        return s->editor->rt;
        return NULL;
    }
}

rose_game* get_present_game(rose_system_sdl2* s) {
    if (s->screen_mode == ROSE_GAMEMODE) {
        return s->game;
    } else {
//        return s->editor;
        return NULL;
    }
}

rose_screenmode get_other_mode(rose_system_sdl2* s) {
    if (s->screen_mode == ROSE_GAMEMODE) {
        return ROSE_EDITORMODE;
    } else {
        return ROSE_GAMEMODE;
    }
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
    rose_game_error err = ROSE_GAME_NO_ERR;
    while (!quit) {
        rose_rt_save_input_frame(get_present_base(s));

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
//                    if (code == ROSE_KEYCODE_ESCAPE && event.key.state == SDL_PRESSED) {
//                        s->screen_mode = get_other_mode(s);
//                        rose_rt_reset_input(get_present_base(s), &mousestate);
//                        break;
//                    }
                    rose_rt_update_keystate(get_present_base(s), code, event.key.state == SDL_PRESSED);
                    err = rose_game_onkey(get_present_game(s), code, event.key.state == SDL_PRESSED, event.key.repeat != 0);
                    switch (err) {
                        case ROSE_GAME_CRITICAL_ERR:
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
                    rose_rt_update_mousestate(get_present_base(s), &mousestate);
                    err = rose_game_onmouse(get_present_game(s), mousestate.x, mousestate.y);
                    switch (err) {
                        case ROSE_GAME_CRITICAL_ERR:
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
                            rose_rt_update_mousestate(get_present_base(s), &mousestate);
                            err = rose_game_onbtn(get_present_game(s), ROSE_LEFT_MOUSE_IDX, true);
                            break;
                        case SDL_BUTTON_RIGHT:
                            mousestate.right_btn_down = true;
                            rose_rt_update_mousestate(get_present_base(s), &mousestate);
                            err = rose_game_onbtn(get_present_game(s), ROSE_RIGHT_MOUSE_IDX, true);
                            break;
                        case SDL_BUTTON_MIDDLE:
                            mousestate.middle_btn_down = true;
                            rose_rt_update_mousestate(get_present_base(s), &mousestate);
                            err = rose_game_onbtn(get_present_game(s), ROSE_MIDDLE_MOUSE_IDX, true);
                            break;
                        case SDL_BUTTON_X1:
                            mousestate.x1_btn_down = true;
                            rose_rt_update_mousestate(get_present_base(s), &mousestate);
                            err = rose_game_onbtn(get_present_game(s), ROSE_X1_MOUSE_IDX, true);
                            break;
                        case SDL_BUTTON_X2:
                            mousestate.x2_btn_down = true;
                            rose_rt_update_mousestate(get_present_base(s), &mousestate);
                            err = rose_game_onbtn(get_present_game(s), ROSE_X2_MOUSE_IDX, true);
                            break;
                        default:
                            break;
                    }
                    switch (err) {
                        case ROSE_GAME_CRITICAL_ERR:
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
                            rose_rt_update_mousestate(get_present_base(s), &mousestate);
                            err = rose_game_onbtn(get_present_game(s), ROSE_LEFT_MOUSE_IDX, false);
                            break;
                        case SDL_BUTTON_RIGHT:
                            mousestate.right_btn_down = false;
                            rose_rt_update_mousestate(get_present_base(s), &mousestate);
                            err = rose_game_onbtn(get_present_game(s), ROSE_RIGHT_MOUSE_IDX, false);
                            break;
                        case SDL_BUTTON_MIDDLE:
                            mousestate.middle_btn_down = false;
                            rose_rt_update_mousestate(get_present_base(s), &mousestate);
                            err = rose_game_onbtn(get_present_game(s), ROSE_MIDDLE_MOUSE_IDX, false);
                            break;
                        case SDL_BUTTON_X1:
                            mousestate.x1_btn_down = false;
                            rose_rt_update_mousestate(get_present_base(s), &mousestate);
                            err = rose_game_onbtn(get_present_game(s), ROSE_X1_MOUSE_IDX, false);
                            break;
                        case SDL_BUTTON_X2:
                            mousestate.x2_btn_down = false;
                            rose_rt_update_mousestate(get_present_base(s), &mousestate);
                            err = rose_game_onbtn(get_present_game(s), ROSE_X2_MOUSE_IDX, false);
                            break;
                        default:
                            break;
                    }
                    switch (err) {
                        case ROSE_GAME_CRITICAL_ERR:
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
            rose_rt_update_mousestate(get_present_base(s), &mousestate);
            err = rose_game_onwheel(get_present_game(s), mousestate.wheel_x, mousestate.wheel_x, mousestate.wheel_inverted);
            switch (err) {
                case ROSE_GAME_CRITICAL_ERR:
                    quit = true;
                    break;
                default:
                    break;
            }
            wheel_changed = false;
        }
        err = rose_game_update(get_present_game(s));
        switch (err) {
            case ROSE_GAME_CRITICAL_ERR:
                quit = true;
                break;
            default:
                break;
        }

        err = rose_game_draw(get_present_game(s));
        switch (err) {
            case ROSE_GAME_CRITICAL_ERR:
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

            rose_memory_range* palette = get_present_base(s)->palette;
            rose_memory_range* screen = get_present_base(s)->screen;
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
        rose_game_free(s->game);
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
        file->buffer = (uint8_t*)realloc(file->buffer, (size_t)res_size);
    } else {
        file->buffer = (uint8_t*)malloc((size_t)res_size);
    }


    Sint64 nb_read_total = 0, nb_read = 1;
    uint8_t* buf = file->buffer;
    while (nb_read_total < res_size && nb_read != 0) {
        nb_read = (Sint64) SDL_RWread(rw, buf, sizeof(uint8_t), res_size - nb_read_total);
        nb_read_total += nb_read;
        buf += nb_read;
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

    if (d)
    {
        struct dirent *p;

        r = 0;

        while (!r && (p=readdir(d)))
        {
            int r2 = -1;
            char *buf;
            size_t len;

            /* Skip the names "." and ".." as we don't want to recurse on them. */
            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
            {
                continue;
            }

            len = path_len + strlen(p->d_name) + 2;
            buf = (char*) malloc(len);

            if (buf)
            {
                struct stat statbuf;

                snprintf(buf, len, "%s/%s", path.c_str(), p->d_name);

                if (!stat(buf, &statbuf))
                {
                    if (S_ISDIR(statbuf.st_mode))
                    {
                        r2 = remove_directory(buf);
                    }
                    else
                    {
                        r2 = unlink(buf);
                    }
                }

                free(buf);
            }

            r = r2;
        }

        closedir(d);
    }

    if (!r)
    {
        r = rmdir(path.c_str());
    }

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
            int err = unlink(full_path.c_str());
            if (err != 0) {
                fprintf(stderr, "remove error %s %d\n", full_path.c_str(), errno);
                return ROSE_FS_CRITICAL_ERR;
            }
            return ROSE_FS_NO_ERR;
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
