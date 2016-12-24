#include <types.h>
#include "sys/sys_sdl2.h"

// Renders screen
void render(rose_system_sdl2* s);

// Pixel manipulators
bool lock_texture(rose_system_sdl2* s);

bool unlock_texture(rose_system_sdl2* s);

void copy_pixels(rose_system_sdl2* s, void* pixels);

uint32_t get_screen_mult(rose_system_sdl2* s);

void make_screen_rect(rose_system_sdl2* s, SDL_Rect* rect);

SDL_Scancode rose_keycode_to_sdl_scancode(rose_keycode key);

rose_keycode sdl_scancode_to_rose_keycode(SDL_Scancode scan);

void getfile(const char* abs_path, uint8_t** buffer, size_t* buffer_len);

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

    s->window = NULL;
    s->renderer = NULL;
    s->texture = NULL;
    s->cartridge = NULL;
    // s->editor = NULL;
    // s->game = NULL;
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
    fs->getfile = &getfile;

    getfile("test/CART_DATA", &fs->cart->data, &fs->cart->data_size);
    char* main_name = (char*)malloc(strlen("main.lua"));
    strcpy(main_name, "main.lua");
    char* main_path = (char*)malloc(strlen("test/main.lua"));
    strcpy(main_path, "test/main.lua");
    rose_file_info main_info = {
            .type = ROSE_FILE_DISK,
            .name = main_name,
            .path = main_path
    };
    fs->cart->code = (rose_file_info**)malloc(sizeof(*fs->cart->code) * 1);
    rose_file_info* main_info_heap = (rose_file_info*)malloc(sizeof(*main_info_heap));
    memcpy(main_info_heap, &main_info, sizeof(*main_info_heap));
    fs->cart->code[0] = main_info_heap;
    fs->cart->code_size = 1;

    s->game = rose_runtime_game_create(fs);
    rose_runtime_game_error err = rose_runtime_game_init(s->game);
    SDL_ShowCursor(SDL_DISABLE);


    return true;
}

void rose_sys_sdl2_run(rose_system_sdl2* s) {
    Uint32 windowID = SDL_GetWindowID(s->window);

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
    rose_runtime_game_error err;
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

    if (s->cartridge != NULL) {
        rose_cartridge_free(s->cartridge);
        s->cartridge = NULL;
    }
    SDL_Quit();
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

void copy_pixels(rose_system_sdl2* s, void* newPixels) {
    // Texture is locked
    if (s->pixels != NULL) {
        // Copy to locked pixels
        memcpy(s->pixels, newPixels, s->pitch * ROSE_SCREEN_HEIGHT);
    }
}

void getfile(const char* abs_path, uint8_t** buffer, size_t* buffer_len) {
    if (abs_path[0] == '/') {
        abs_path = abs_path+1;
    }
    char* base_path = get_perf_path();
    size_t base_path_len = strlen(base_path);
    size_t abs_path_len = strlen(abs_path);
    char* full_path = (char*)malloc(base_path_len + abs_path_len + 1);
    memcpy(full_path, base_path, base_path_len);
    memcpy(full_path + base_path_len, abs_path, abs_path_len);
    full_path[base_path_len + abs_path_len] = '\0';

    SDL_RWops *rw = SDL_RWFromFile(full_path, "rb");
    free(full_path);
    if (rw == NULL) return;

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
        return;
    }
    *buffer = res;
    *buffer_len = (size_t) nb_read_total;
}