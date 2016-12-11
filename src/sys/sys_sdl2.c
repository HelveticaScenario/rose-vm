#include "sys/sys_sdl2.h"

// Renders screen
void render(Rose_SystemSdl2 *s);

// Pixel manipulators
bool lockTexture(Rose_SystemSdl2 *s);

bool unlockTexture(Rose_SystemSdl2 *s);

void copyPixels(Rose_SystemSdl2 *s, void *pixels);

uint32_t get_screen_mult(Rose_SystemSdl2 *s);

void make_screen_rect(Rose_SystemSdl2 *s, SDL_Rect* rect);

bool rose_sys_sdl2_init(Rose_SystemSdl2* s, int argc, char* argv[]) {

    s->window = NULL;
    s->renderer = NULL;
    s->texture = NULL;
    s->cartridge = NULL;
    // s->editor = NULL;
    // s->game = NULL;
    s->screenMode = GameMode;
    s->pixels = NULL;
    s->pitch = 0;
    s->windowWidth = 0;
    s->windowHeight = 0;
    s->widthMult = 0;
    s->heightMult = 0;


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
    s->renderer = SDL_CreateRenderer(s->window, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);
    if (s->renderer == NULL) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    SDL_GL_GetDrawableSize(s->window, &(s->windowWidth), &(s->windowHeight));
    s->widthMult = s->windowWidth / ROSE_SCREEN_WIDTH;
    s->heightMult = s->windowHeight / ROSE_SCREEN_HEIGHT;

    // Initialize renderer color
    SDL_SetRenderDrawColor(s->renderer, 0, 0, 0, 0xFF);

    // Create uninitialized texture
    s->texture = SDL_CreateTexture(s->renderer, SDL_PIXELFORMAT_RGB24,
                                SDL_TEXTUREACCESS_STREAMING, ROSE_SCREEN_WIDTH,
                                ROSE_SCREEN_HEIGHT);
    if (s->texture == NULL) {
        printf("Unable to create blank texture! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    s->cartridge = rose_cartridge_create();
    FILE * f = fopen ("/Users/contrarian/Documents/Play/Rosebud/test.lua", "rb");

    if (f) {
        free(s->cartridge->code); // free previous code buffer

        fseek (f, 0, SEEK_END);
        s->cartridge->code_size = ftell (f);
        fseek (f, 0, SEEK_SET);
        s->cartridge->code = malloc (s->cartridge->code_size + 1);
        if (s->cartridge->code)
        {
            fread (s->cartridge->code, 1, s->cartridge->code_size, f);
            s->cartridge->code[s->cartridge->code_size] = '\0';
        }
        fclose (f);
    }

    s->cartridge->data[0] = 2;
    s->game = rose_runtime_game_create(s->cartridge);
    Rose_RuntimeGameError err = rose_runtime_game_init(s->game);
    SDL_ShowCursor(SDL_DISABLE);

    return true;
}

void rose_sys_sdl2_run(Rose_SystemSdl2 *s) {
    Uint32 windowID = SDL_GetWindowID(s->window);

    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event event;
    // While application is running
    SDL_StartTextInput();

    Rose_MouseState mouseState;
    mouseState.x = 0;
    mouseState.y = 0;
    mouseState.leftBtnDown = false;
    mouseState.rightBtnDown = false;
    mouseState.middleBtnDown = false;
    mouseState.x1BtnDown = false;
    mouseState.x2BtnDown = false;
    mouseState.wheel_x = 0;
    mouseState.wheel_y = 0;
    SDL_Rect screen_rect;
    make_screen_rect(s, &screen_rect);

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
                                SDL_GL_GetDrawableSize(s->window, &(s->windowWidth), &(s->windowHeight));
                                s->widthMult = s->windowWidth / ROSE_SCREEN_WIDTH;
                                s->heightMult = s->windowHeight / ROSE_SCREEN_HEIGHT;
                                make_screen_rect(s, &screen_rect);
                                printf("%d %d\n", s->widthMult, s->heightMult);
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
                case SDL_TEXTINPUT: {
                    /* Add new text onto the end of our text */

                    break;
                }
                case SDL_MOUSEMOTION: {
                    int32_t mult = (int32_t) get_screen_mult(s);
                    if (mult == 0) 
                        break;
                    mouseState.x = (int16_t)((event.motion.x - ((int32_t)screen_rect.x)) / mult);
                    mouseState.y = (int16_t)((event.motion.y - ((int32_t)screen_rect.y)) / mult);
                    break;
                }
                case SDL_MOUSEBUTTONDOWN: {
                    switch (event.button.button) {
                        case SDL_BUTTON_LEFT:
                            mouseState.leftBtnDown = true;
                            break;
                        case SDL_BUTTON_RIGHT:
                            mouseState.rightBtnDown = true;
                            break;
                        case SDL_BUTTON_MIDDLE:
                            mouseState.middleBtnDown = true;
                            break;
                        case SDL_BUTTON_X1:
                            mouseState.x1BtnDown = true;
                            break;
                        case SDL_BUTTON_X2:
                            mouseState.x2BtnDown = true;
                            break;
                        default:
                            break;
                    }
                    
                    break;
                }
                case SDL_MOUSEBUTTONUP: {
                    switch (event.button.button) {
                        case SDL_BUTTON_LEFT:
                            mouseState.leftBtnDown = false;
                            break;
                        case SDL_BUTTON_RIGHT:
                            mouseState.rightBtnDown = false;
                            break;
                        case SDL_BUTTON_MIDDLE:
                            mouseState.middleBtnDown = false;
                            break;
                        case SDL_BUTTON_X1:
                            mouseState.x1BtnDown = false;
                            break;
                        case SDL_BUTTON_X2:
                            mouseState.x2BtnDown = false;
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
                        mouseState.wheel_inverted = true;
                    } else {
                        mouseState.wheel_inverted = false;
                    }
                    mouseState.wheel_x += x;
                    mouseState.wheel_y += y;

                }
            }
            // User requests quit
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
        Rose_RuntimeGameError err;
        rose_runtime_base_update_mousestate(s->game->base, &mouseState);
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

        mouseState.wheel_x = 0;
        mouseState.wheel_y = 0;

        if (lockTexture(s)) {
            uint8_t* pixels = (uint8_t *) s->pixels;
            int pitch = s->pitch;

            int pixelCount = (pitch) * ROSE_SCREEN_HEIGHT;

            Rose_MemoryRange* palette = s->game->base->palette;
            Rose_MemoryRange* screen = s->game->base->screen;
            Rose_MemoryIterator it = screen->begin;
            for (; it < screen->end; it = rose_memory_iterator_next(it)) {
                uint16_t i = it - screen->begin;
                uint8_t c = *it;
                pixels[(i * 3) + 0] = *(palette->begin + (c * 3) + 0);
                pixels[(i * 3) + 1] = *(palette->begin + (c * 3) + 1);
                pixels[(i * 3) + 2] = *(palette->begin + (c * 3) + 2);
            }
            unlockTexture(s);
        }
        render(s);
    }
}

void rose_sys_sdl2_free(Rose_SystemSdl2 *s) {
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

uint32_t get_screen_mult(Rose_SystemSdl2 *s) {
    return s->widthMult < s->heightMult ? s->widthMult : s->heightMult;
}

void render(Rose_SystemSdl2 *s) {
    SDL_RenderClear(s->renderer);
    SDL_Rect rect;
    make_screen_rect(s, &rect);
    SDL_RenderCopy(s->renderer, s->texture, NULL, &rect);
    SDL_RenderPresent(s->renderer);
}

void make_screen_rect(Rose_SystemSdl2 *s, SDL_Rect* rect) {
    uint32_t mult = get_screen_mult(s);
    rect->w = ROSE_SCREEN_WIDTH * mult;
    rect->h = ROSE_SCREEN_HEIGHT * mult;
    rect->x = (s->windowWidth - rect->w) / 2;
    rect->y = (s->windowHeight - rect->h) / 2;
}

bool lockTexture(Rose_SystemSdl2 *s) {
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

bool unlockTexture(Rose_SystemSdl2 *s) {
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

void copyPixels(Rose_SystemSdl2 *s, void *newPixels) {
    // Texture is locked
    if (s->pixels != NULL) {
        // Copy to locked pixels
        memcpy(s->pixels, newPixels, s->pitch * ROSE_SCREEN_HEIGHT);
    }
}