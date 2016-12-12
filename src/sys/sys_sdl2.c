#include "sys/sys_sdl2.h"

// Renders screen
void render(Rose_SystemSdl2 *s);

// Pixel manipulators
bool lockTexture(Rose_SystemSdl2 *s);

bool unlockTexture(Rose_SystemSdl2 *s);

void copyPixels(Rose_SystemSdl2 *s, void *pixels);

uint32_t get_screen_mult(Rose_SystemSdl2 *s);

void make_screen_rect(Rose_SystemSdl2 *s, SDL_Rect* rect);

SDL_Scancode rose_keycode_to_sdl_scancode(Rose_KeyCode key);

Rose_KeyCode sdl_scancode_to_rose_keycode(SDL_Scancode scan);

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
                case SDL_KEYUP:
                case SDL_KEYDOWN: {
                    Rose_KeyCode code = sdl_scancode_to_rose_keycode(event.key.keysym.scancode);
                    rose_runtime_base_update_keystate(s->game->base, code, event.key.state == SDL_PRESSED);
                    rose_runtime_game_onkey(s->game, code, event.key.state == SDL_PRESSED, event.key.repeat != 0);
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

SDL_Scancode rose_keycode_to_sdl_scancode(Rose_KeyCode key) {
    switch (key) {
        case ROSE_KEYCODE_A:
            return SDL_SCANCODE_A;
            break;
        case ROSE_KEYCODE_B:
            return SDL_SCANCODE_B;
            break;
        case ROSE_KEYCODE_C:
            return SDL_SCANCODE_C;
            break;
        case ROSE_KEYCODE_D:
            return SDL_SCANCODE_D;
            break;
        case ROSE_KEYCODE_E:
            return SDL_SCANCODE_E;
            break;
        case ROSE_KEYCODE_F:
            return SDL_SCANCODE_F;
            break;
        case ROSE_KEYCODE_G:
            return SDL_SCANCODE_G;
            break;
        case ROSE_KEYCODE_H:
            return SDL_SCANCODE_H;
            break;
        case ROSE_KEYCODE_I:
            return SDL_SCANCODE_I;
            break;
        case ROSE_KEYCODE_J:
            return SDL_SCANCODE_J;
            break;
        case ROSE_KEYCODE_K:
            return SDL_SCANCODE_K;
            break;
        case ROSE_KEYCODE_L:
            return SDL_SCANCODE_L;
            break;
        case ROSE_KEYCODE_M:
            return SDL_SCANCODE_M;
            break;
        case ROSE_KEYCODE_N:
            return SDL_SCANCODE_N;
            break;
        case ROSE_KEYCODE_O:
            return SDL_SCANCODE_O;
            break;
        case ROSE_KEYCODE_P:
            return SDL_SCANCODE_P;
            break;
        case ROSE_KEYCODE_Q:
            return SDL_SCANCODE_Q;
            break;
        case ROSE_KEYCODE_R:
            return SDL_SCANCODE_R;
            break;
        case ROSE_KEYCODE_S:
            return SDL_SCANCODE_S;
            break;
        case ROSE_KEYCODE_T:
            return SDL_SCANCODE_T;
            break;
        case ROSE_KEYCODE_U:
            return SDL_SCANCODE_U;
            break;
        case ROSE_KEYCODE_V:
            return SDL_SCANCODE_V;
            break;
        case ROSE_KEYCODE_W:
            return SDL_SCANCODE_W;
            break;
        case ROSE_KEYCODE_X:
            return SDL_SCANCODE_X;
            break;
        case ROSE_KEYCODE_Y:
            return SDL_SCANCODE_Y;
            break;
        case ROSE_KEYCODE_Z:
            return SDL_SCANCODE_Z;
            break;
        case ROSE_KEYCODE_1:
            return SDL_SCANCODE_1;
            break;
        case ROSE_KEYCODE_2:
            return SDL_SCANCODE_2;
            break;
        case ROSE_KEYCODE_3:
            return SDL_SCANCODE_3;
            break;
        case ROSE_KEYCODE_4:
            return SDL_SCANCODE_4;
            break;
        case ROSE_KEYCODE_5:
            return SDL_SCANCODE_5;
            break;
        case ROSE_KEYCODE_6:
            return SDL_SCANCODE_6;
            break;
        case ROSE_KEYCODE_7:
            return SDL_SCANCODE_7;
            break;
        case ROSE_KEYCODE_8:
            return SDL_SCANCODE_8;
            break;
        case ROSE_KEYCODE_9:
            return SDL_SCANCODE_9;
            break;
        case ROSE_KEYCODE_0:
            return SDL_SCANCODE_0;
            break;
        case ROSE_KEYCODE_RETURN:
            return SDL_SCANCODE_RETURN;
            break;
        case ROSE_KEYCODE_ESCAPE:
            return SDL_SCANCODE_ESCAPE;
            break;
        case ROSE_KEYCODE_BACKSPACE:
            return SDL_SCANCODE_BACKSPACE;
            break;
        case ROSE_KEYCODE_TAB:
            return SDL_SCANCODE_TAB;
            break;
        case ROSE_KEYCODE_SPACE:
            return SDL_SCANCODE_SPACE;
            break;
        case ROSE_KEYCODE_MINUS:
            return SDL_SCANCODE_MINUS;
            break;
        case ROSE_KEYCODE_EQUALS:
            return SDL_SCANCODE_EQUALS;
            break;
        case ROSE_KEYCODE_LEFTBRACKET:
            return SDL_SCANCODE_LEFTBRACKET;
            break;
        case ROSE_KEYCODE_RIGHTBRACKET:
            return SDL_SCANCODE_RIGHTBRACKET;
            break;
        case ROSE_KEYCODE_BACKSLASH:
            return SDL_SCANCODE_BACKSLASH;
            break;
        case ROSE_KEYCODE_NONUSHASH:
            return SDL_SCANCODE_NONUSHASH;
            break;
        case ROSE_KEYCODE_SEMICOLON:
            return SDL_SCANCODE_SEMICOLON;
            break;
        case ROSE_KEYCODE_APOSTROPHE:
            return SDL_SCANCODE_APOSTROPHE;
            break;
        case ROSE_KEYCODE_GRAVE:
            return SDL_SCANCODE_GRAVE;
            break;
        case ROSE_KEYCODE_COMMA:
            return SDL_SCANCODE_COMMA;
            break;
        case ROSE_KEYCODE_PERIOD:
            return SDL_SCANCODE_PERIOD;
            break;
        case ROSE_KEYCODE_SLASH:
            return SDL_SCANCODE_SLASH;
            break;
        case ROSE_KEYCODE_CAPSLOCK:
            return SDL_SCANCODE_CAPSLOCK;
            break;
        case ROSE_KEYCODE_F1:
            return SDL_SCANCODE_F1;
            break;
        case ROSE_KEYCODE_F2:
            return SDL_SCANCODE_F2;
            break;
        case ROSE_KEYCODE_F3:
            return SDL_SCANCODE_F3;
            break;
        case ROSE_KEYCODE_F4:
            return SDL_SCANCODE_F4;
            break;
        case ROSE_KEYCODE_F5:
            return SDL_SCANCODE_F5;
            break;
        case ROSE_KEYCODE_F6:
            return SDL_SCANCODE_F6;
            break;
        case ROSE_KEYCODE_F7:
            return SDL_SCANCODE_F7;
            break;
        case ROSE_KEYCODE_F8:
            return SDL_SCANCODE_F8;
            break;
        case ROSE_KEYCODE_F9:
            return SDL_SCANCODE_F9;
            break;
        case ROSE_KEYCODE_F10:
            return SDL_SCANCODE_F10;
            break;
        case ROSE_KEYCODE_F11:
            return SDL_SCANCODE_F11;
            break;
        case ROSE_KEYCODE_F12:
            return SDL_SCANCODE_F12;
            break;
        case ROSE_KEYCODE_PRINTSCREEN:
            return SDL_SCANCODE_PRINTSCREEN;
            break;
        case ROSE_KEYCODE_SCROLLLOCK:
            return SDL_SCANCODE_SCROLLLOCK;
            break;
        case ROSE_KEYCODE_PAUSE:
            return SDL_SCANCODE_PAUSE;
            break;
        case ROSE_KEYCODE_INSERT:
            return SDL_SCANCODE_INSERT;
            break;
        case ROSE_KEYCODE_HOME:
            return SDL_SCANCODE_HOME;
            break;
        case ROSE_KEYCODE_PAGEUP:
            return SDL_SCANCODE_PAGEUP;
            break;
        case ROSE_KEYCODE_DELETE:
            return SDL_SCANCODE_DELETE;
            break;
        case ROSE_KEYCODE_END:
            return SDL_SCANCODE_END;
            break;
        case ROSE_KEYCODE_PAGEDOWN:
            return SDL_SCANCODE_PAGEDOWN;
            break;
        case ROSE_KEYCODE_RIGHT:
            return SDL_SCANCODE_RIGHT;
            break;
        case ROSE_KEYCODE_LEFT:
            return SDL_SCANCODE_LEFT;
            break;
        case ROSE_KEYCODE_DOWN:
            return SDL_SCANCODE_DOWN;
            break;
        case ROSE_KEYCODE_UP:
            return SDL_SCANCODE_UP;
            break;
        case ROSE_KEYCODE_NUMLOCKCLEAR:
            return SDL_SCANCODE_NUMLOCKCLEAR;
            break;
        case ROSE_KEYCODE_KP_DIVIDE:
            return SDL_SCANCODE_KP_DIVIDE;
            break;
        case ROSE_KEYCODE_KP_MULTIPLY:
            return SDL_SCANCODE_KP_MULTIPLY;
            break;
        case ROSE_KEYCODE_KP_MINUS:
            return SDL_SCANCODE_KP_MINUS;
            break;
        case ROSE_KEYCODE_KP_PLUS:
            return SDL_SCANCODE_KP_PLUS;
            break;
        case ROSE_KEYCODE_KP_ENTER:
            return SDL_SCANCODE_KP_ENTER;
            break;
        case ROSE_KEYCODE_KP_1:
            return SDL_SCANCODE_KP_1;
            break;
        case ROSE_KEYCODE_KP_2:
            return SDL_SCANCODE_KP_2;
            break;
        case ROSE_KEYCODE_KP_3:
            return SDL_SCANCODE_KP_3;
            break;
        case ROSE_KEYCODE_KP_4:
            return SDL_SCANCODE_KP_4;
            break;
        case ROSE_KEYCODE_KP_5:
            return SDL_SCANCODE_KP_5;
            break;
        case ROSE_KEYCODE_KP_6:
            return SDL_SCANCODE_KP_6;
            break;
        case ROSE_KEYCODE_KP_7:
            return SDL_SCANCODE_KP_7;
            break;
        case ROSE_KEYCODE_KP_8:
            return SDL_SCANCODE_KP_8;
            break;
        case ROSE_KEYCODE_KP_9:
            return SDL_SCANCODE_KP_9;
            break;
        case ROSE_KEYCODE_KP_0:
            return SDL_SCANCODE_KP_0;
            break;
        case ROSE_KEYCODE_KP_PERIOD:
            return SDL_SCANCODE_KP_PERIOD;
            break;
        case ROSE_KEYCODE_NONUSBACKSLASH:
            return SDL_SCANCODE_NONUSBACKSLASH;
            break;
        case ROSE_KEYCODE_APPLICATION:
            return SDL_SCANCODE_APPLICATION;
            break;
        case ROSE_KEYCODE_POWER:
            return SDL_SCANCODE_POWER;
            break;
        case ROSE_KEYCODE_KP_EQUALS:
            return SDL_SCANCODE_KP_EQUALS;
            break;
        case ROSE_KEYCODE_F13:
            return SDL_SCANCODE_F13;
            break;
        case ROSE_KEYCODE_F14:
            return SDL_SCANCODE_F14;
            break;
        case ROSE_KEYCODE_F15:
            return SDL_SCANCODE_F15;
            break;
        case ROSE_KEYCODE_F16:
            return SDL_SCANCODE_F16;
            break;
        case ROSE_KEYCODE_F17:
            return SDL_SCANCODE_F17;
            break;
        case ROSE_KEYCODE_F18:
            return SDL_SCANCODE_F18;
            break;
        case ROSE_KEYCODE_F19:
            return SDL_SCANCODE_F19;
            break;
        case ROSE_KEYCODE_F20:
            return SDL_SCANCODE_F20;
            break;
        case ROSE_KEYCODE_F21:
            return SDL_SCANCODE_F21;
            break;
        case ROSE_KEYCODE_F22:
            return SDL_SCANCODE_F22;
            break;
        case ROSE_KEYCODE_F23:
            return SDL_SCANCODE_F23;
            break;
        case ROSE_KEYCODE_F24:
            return SDL_SCANCODE_F24;
            break;
        case ROSE_KEYCODE_EXECUTE:
            return SDL_SCANCODE_EXECUTE;
            break;
        case ROSE_KEYCODE_HELP:
            return SDL_SCANCODE_HELP;
            break;
        case ROSE_KEYCODE_MENU:
            return SDL_SCANCODE_MENU;
            break;
        case ROSE_KEYCODE_SELECT:
            return SDL_SCANCODE_SELECT;
            break;
        case ROSE_KEYCODE_STOP:
            return SDL_SCANCODE_STOP;
            break;
        case ROSE_KEYCODE_AGAIN:
            return SDL_SCANCODE_AGAIN;
            break;
        case ROSE_KEYCODE_UNDO:
            return SDL_SCANCODE_UNDO;
            break;
        case ROSE_KEYCODE_CUT:
            return SDL_SCANCODE_CUT;
            break;
        case ROSE_KEYCODE_COPY:
            return SDL_SCANCODE_COPY;
            break;
        case ROSE_KEYCODE_PASTE:
            return SDL_SCANCODE_PASTE;
            break;
        case ROSE_KEYCODE_FIND:
            return SDL_SCANCODE_FIND;
            break;
        case ROSE_KEYCODE_MUTE:
            return SDL_SCANCODE_MUTE;
            break;
        case ROSE_KEYCODE_VOLUMEUP:
            return SDL_SCANCODE_VOLUMEUP;
            break;
        case ROSE_KEYCODE_VOLUMEDOWN:
            return SDL_SCANCODE_VOLUMEDOWN;
            break;
        case ROSE_KEYCODE_KP_COMMA:
            return SDL_SCANCODE_KP_COMMA;
            break;
        case ROSE_KEYCODE_KP_EQUALSAS400:
            return SDL_SCANCODE_KP_EQUALSAS400;
            break;
        case ROSE_KEYCODE_INTERNATIONAL1:
            return SDL_SCANCODE_INTERNATIONAL1;
            break;
        case ROSE_KEYCODE_INTERNATIONAL2:
            return SDL_SCANCODE_INTERNATIONAL2;
            break;
        case ROSE_KEYCODE_INTERNATIONAL3:
            return SDL_SCANCODE_INTERNATIONAL3;
            break;
        case ROSE_KEYCODE_INTERNATIONAL4:
            return SDL_SCANCODE_INTERNATIONAL4;
            break;
        case ROSE_KEYCODE_INTERNATIONAL5:
            return SDL_SCANCODE_INTERNATIONAL5;
            break;
        case ROSE_KEYCODE_INTERNATIONAL6:
            return SDL_SCANCODE_INTERNATIONAL6;
            break;
        case ROSE_KEYCODE_INTERNATIONAL7:
            return SDL_SCANCODE_INTERNATIONAL7;
            break;
        case ROSE_KEYCODE_INTERNATIONAL8:
            return SDL_SCANCODE_INTERNATIONAL8;
            break;
        case ROSE_KEYCODE_INTERNATIONAL9:
            return SDL_SCANCODE_INTERNATIONAL9;
            break;
        case ROSE_KEYCODE_LANG1:
            return SDL_SCANCODE_LANG1;
            break;
        case ROSE_KEYCODE_LANG2:
            return SDL_SCANCODE_LANG2;
            break;
        case ROSE_KEYCODE_LANG3:
            return SDL_SCANCODE_LANG3;
            break;
        case ROSE_KEYCODE_LANG4:
            return SDL_SCANCODE_LANG4;
            break;
        case ROSE_KEYCODE_LANG5:
            return SDL_SCANCODE_LANG5;
            break;
        case ROSE_KEYCODE_LANG6:
            return SDL_SCANCODE_LANG6;
            break;
        case ROSE_KEYCODE_LANG7:
            return SDL_SCANCODE_LANG7;
            break;
        case ROSE_KEYCODE_LANG8:
            return SDL_SCANCODE_LANG8;
            break;
        case ROSE_KEYCODE_LANG9:
            return SDL_SCANCODE_LANG9;
            break;
        case ROSE_KEYCODE_ALTERASE:
            return SDL_SCANCODE_ALTERASE;
            break;
        case ROSE_KEYCODE_SYSREQ:
            return SDL_SCANCODE_SYSREQ;
            break;
        case ROSE_KEYCODE_CANCEL:
            return SDL_SCANCODE_CANCEL;
            break;
        case ROSE_KEYCODE_CLEAR:
            return SDL_SCANCODE_CLEAR;
            break;
        case ROSE_KEYCODE_PRIOR:
            return SDL_SCANCODE_PRIOR;
            break;
        case ROSE_KEYCODE_RETURN2:
            return SDL_SCANCODE_RETURN2;
            break;
        case ROSE_KEYCODE_SEPARATOR:
            return SDL_SCANCODE_SEPARATOR;
            break;
        case ROSE_KEYCODE_OUT:
            return SDL_SCANCODE_OUT;
            break;
        case ROSE_KEYCODE_OPER:
            return SDL_SCANCODE_OPER;
            break;
        case ROSE_KEYCODE_CLEARAGAIN:
            return SDL_SCANCODE_CLEARAGAIN;
            break;
        case ROSE_KEYCODE_CRSEL:
            return SDL_SCANCODE_CRSEL;
            break;
        case ROSE_KEYCODE_EXSEL:
            return SDL_SCANCODE_EXSEL;
            break;
        case ROSE_KEYCODE_KP_00:
            return SDL_SCANCODE_KP_00;
            break;
        case ROSE_KEYCODE_KP_000:
            return SDL_SCANCODE_KP_000;
            break;
        case ROSE_KEYCODE_THOUSANDSSEPARATOR:
            return SDL_SCANCODE_THOUSANDSSEPARATOR;
            break;
        case ROSE_KEYCODE_DECIMALSEPARATOR:
            return SDL_SCANCODE_DECIMALSEPARATOR;
            break;
        case ROSE_KEYCODE_CURRENCYUNIT:
            return SDL_SCANCODE_CURRENCYUNIT;
            break;
        case ROSE_KEYCODE_CURRENCYSUBUNIT:
            return SDL_SCANCODE_CURRENCYSUBUNIT;
            break;
        case ROSE_KEYCODE_KP_LEFTPAREN:
            return SDL_SCANCODE_KP_LEFTPAREN;
            break;
        case ROSE_KEYCODE_KP_RIGHTPAREN:
            return SDL_SCANCODE_KP_RIGHTPAREN;
            break;
        case ROSE_KEYCODE_KP_LEFTBRACE:
            return SDL_SCANCODE_KP_LEFTBRACE;
            break;
        case ROSE_KEYCODE_KP_RIGHTBRACE:
            return SDL_SCANCODE_KP_RIGHTBRACE;
            break;
        case ROSE_KEYCODE_KP_TAB:
            return SDL_SCANCODE_KP_TAB;
            break;
        case ROSE_KEYCODE_KP_BACKSPACE:
            return SDL_SCANCODE_KP_BACKSPACE;
            break;
        case ROSE_KEYCODE_KP_A:
            return SDL_SCANCODE_KP_A;
            break;
        case ROSE_KEYCODE_KP_B:
            return SDL_SCANCODE_KP_B;
            break;
        case ROSE_KEYCODE_KP_C:
            return SDL_SCANCODE_KP_C;
            break;
        case ROSE_KEYCODE_KP_D:
            return SDL_SCANCODE_KP_D;
            break;
        case ROSE_KEYCODE_KP_E:
            return SDL_SCANCODE_KP_E;
            break;
        case ROSE_KEYCODE_KP_F:
            return SDL_SCANCODE_KP_F;
            break;
        case ROSE_KEYCODE_KP_XOR:
            return SDL_SCANCODE_KP_XOR;
            break;
        case ROSE_KEYCODE_KP_POWER:
            return SDL_SCANCODE_KP_POWER;
            break;
        case ROSE_KEYCODE_KP_PERCENT:
            return SDL_SCANCODE_KP_PERCENT;
            break;
        case ROSE_KEYCODE_KP_LESS:
            return SDL_SCANCODE_KP_LESS;
            break;
        case ROSE_KEYCODE_KP_GREATER:
            return SDL_SCANCODE_KP_GREATER;
            break;
        case ROSE_KEYCODE_KP_AMPERSAND:
            return SDL_SCANCODE_KP_AMPERSAND;
            break;
        case ROSE_KEYCODE_KP_DBLAMPERSAND:
            return SDL_SCANCODE_KP_DBLAMPERSAND;
            break;
        case ROSE_KEYCODE_KP_VERTICALBAR:
            return SDL_SCANCODE_KP_VERTICALBAR;
            break;
        case ROSE_KEYCODE_KP_DBLVERTICALBAR:
            return SDL_SCANCODE_KP_DBLVERTICALBAR;
            break;
        case ROSE_KEYCODE_KP_COLON:
            return SDL_SCANCODE_KP_COLON;
            break;
        case ROSE_KEYCODE_KP_HASH:
            return SDL_SCANCODE_KP_HASH;
            break;
        case ROSE_KEYCODE_KP_SPACE:
            return SDL_SCANCODE_KP_SPACE;
            break;
        case ROSE_KEYCODE_KP_AT:
            return SDL_SCANCODE_KP_AT;
            break;
        case ROSE_KEYCODE_KP_EXCLAM:
            return SDL_SCANCODE_KP_EXCLAM;
            break;
        case ROSE_KEYCODE_KP_MEMSTORE:
            return SDL_SCANCODE_KP_MEMSTORE;
            break;
        case ROSE_KEYCODE_KP_MEMRECALL:
            return SDL_SCANCODE_KP_MEMRECALL;
            break;
        case ROSE_KEYCODE_KP_MEMCLEAR:
            return SDL_SCANCODE_KP_MEMCLEAR;
            break;
        case ROSE_KEYCODE_KP_MEMADD:
            return SDL_SCANCODE_KP_MEMADD;
            break;
        case ROSE_KEYCODE_KP_MEMSUBTRACT:
            return SDL_SCANCODE_KP_MEMSUBTRACT;
            break;
        case ROSE_KEYCODE_KP_MEMMULTIPLY:
            return SDL_SCANCODE_KP_MEMMULTIPLY;
            break;
        case ROSE_KEYCODE_KP_MEMDIVIDE:
            return SDL_SCANCODE_KP_MEMDIVIDE;
            break;
        case ROSE_KEYCODE_KP_PLUSMINUS:
            return SDL_SCANCODE_KP_PLUSMINUS;
            break;
        case ROSE_KEYCODE_KP_CLEAR:
            return SDL_SCANCODE_KP_CLEAR;
            break;
        case ROSE_KEYCODE_KP_CLEARENTRY:
            return SDL_SCANCODE_KP_CLEARENTRY;
            break;
        case ROSE_KEYCODE_KP_BINARY:
            return SDL_SCANCODE_KP_BINARY;
            break;
        case ROSE_KEYCODE_KP_OCTAL:
            return SDL_SCANCODE_KP_OCTAL;
            break;
        case ROSE_KEYCODE_KP_DECIMAL:
            return SDL_SCANCODE_KP_DECIMAL;
            break;
        case ROSE_KEYCODE_KP_HEXADECIMAL:
            return SDL_SCANCODE_KP_HEXADECIMAL;
            break;
        case ROSE_KEYCODE_LCTRL:
            return SDL_SCANCODE_LCTRL;
            break;
        case ROSE_KEYCODE_LSHIFT:
            return SDL_SCANCODE_LSHIFT;
            break;
        case ROSE_KEYCODE_LALT:
            return SDL_SCANCODE_LALT;
            break;
        case ROSE_KEYCODE_LGUI:
            return SDL_SCANCODE_LGUI;
            break;
        case ROSE_KEYCODE_RCTRL:
            return SDL_SCANCODE_RCTRL;
            break;
        case ROSE_KEYCODE_RSHIFT:
            return SDL_SCANCODE_RSHIFT;
            break;
        case ROSE_KEYCODE_RALT:
            return SDL_SCANCODE_RALT;
            break;
        case ROSE_KEYCODE_RGUI:
            return SDL_SCANCODE_RGUI;
            break;
        case ROSE_KEYCODE_MODE:
            return SDL_SCANCODE_MODE;
            break;
        case ROSE_KEYCODE_AUDIONEXT:
            return SDL_SCANCODE_AUDIONEXT;
            break;
        case ROSE_KEYCODE_AUDIOPREV:
            return SDL_SCANCODE_AUDIOPREV;
            break;
        case ROSE_KEYCODE_AUDIOSTOP:
            return SDL_SCANCODE_AUDIOSTOP;
            break;
        case ROSE_KEYCODE_AUDIOPLAY:
            return SDL_SCANCODE_AUDIOPLAY;
            break;
        case ROSE_KEYCODE_AUDIOMUTE:
            return SDL_SCANCODE_AUDIOMUTE;
            break;
        case ROSE_KEYCODE_MEDIASELECT:
            return SDL_SCANCODE_MEDIASELECT;
            break;
        case ROSE_KEYCODE_WWW:
            return SDL_SCANCODE_WWW;
            break;
        case ROSE_KEYCODE_MAIL:
            return SDL_SCANCODE_MAIL;
            break;
        case ROSE_KEYCODE_CALCULATOR:
            return SDL_SCANCODE_CALCULATOR;
            break;
        case ROSE_KEYCODE_COMPUTER:
            return SDL_SCANCODE_COMPUTER;
            break;
        case ROSE_KEYCODE_AC_SEARCH:
            return SDL_SCANCODE_AC_SEARCH;
            break;
        case ROSE_KEYCODE_AC_HOME:
            return SDL_SCANCODE_AC_HOME;
            break;
        case ROSE_KEYCODE_AC_BACK:
            return SDL_SCANCODE_AC_BACK;
            break;
        case ROSE_KEYCODE_AC_FORWARD:
            return SDL_SCANCODE_AC_FORWARD;
            break;
        case ROSE_KEYCODE_AC_STOP:
            return SDL_SCANCODE_AC_STOP;
            break;
        case ROSE_KEYCODE_AC_REFRESH:
            return SDL_SCANCODE_AC_REFRESH;
            break;
        case ROSE_KEYCODE_AC_BOOKMARKS:
            return SDL_SCANCODE_AC_BOOKMARKS;
            break;
        case ROSE_KEYCODE_BRIGHTNESSDOWN:
            return SDL_SCANCODE_BRIGHTNESSDOWN;
            break;
        case ROSE_KEYCODE_BRIGHTNESSUP:
            return SDL_SCANCODE_BRIGHTNESSUP;
            break;
        case ROSE_KEYCODE_DISPLAYSWITCH:
            return SDL_SCANCODE_DISPLAYSWITCH;
            break;
        case ROSE_KEYCODE_KBDILLUMTOGGLE:
            return SDL_SCANCODE_KBDILLUMTOGGLE;
            break;
        case ROSE_KEYCODE_KBDILLUMDOWN:
            return SDL_SCANCODE_KBDILLUMDOWN;
            break;
        case ROSE_KEYCODE_KBDILLUMUP:
            return SDL_SCANCODE_KBDILLUMUP;
            break;
        case ROSE_KEYCODE_EJECT:
            return SDL_SCANCODE_EJECT;
            break;
        case ROSE_KEYCODE_SLEEP:
            return SDL_SCANCODE_SLEEP;
            break;
        case ROSE_KEYCODE_APP1:
            return SDL_SCANCODE_APP1;
            break;
        case ROSE_KEYCODE_APP2:
            return SDL_SCANCODE_APP2;
            break;
        default:
            return SDL_SCANCODE_UNKNOWN;
    }
}

Rose_KeyCode sdl_scancode_to_rose_keycode(SDL_Scancode scan) {
    switch (scan) {
        case SDL_SCANCODE_A:
            return ROSE_KEYCODE_A;
            break;
        case SDL_SCANCODE_B:
            return ROSE_KEYCODE_B;
            break;
        case SDL_SCANCODE_C:
            return ROSE_KEYCODE_C;
            break;
        case SDL_SCANCODE_D:
            return ROSE_KEYCODE_D;
            break;
        case SDL_SCANCODE_E:
            return ROSE_KEYCODE_E;
            break;
        case SDL_SCANCODE_F:
            return ROSE_KEYCODE_F;
            break;
        case SDL_SCANCODE_G:
            return ROSE_KEYCODE_G;
            break;
        case SDL_SCANCODE_H:
            return ROSE_KEYCODE_H;
            break;
        case SDL_SCANCODE_I:
            return ROSE_KEYCODE_I;
            break;
        case SDL_SCANCODE_J:
            return ROSE_KEYCODE_J;
            break;
        case SDL_SCANCODE_K:
            return ROSE_KEYCODE_K;
            break;
        case SDL_SCANCODE_L:
            return ROSE_KEYCODE_L;
            break;
        case SDL_SCANCODE_M:
            return ROSE_KEYCODE_M;
            break;
        case SDL_SCANCODE_N:
            return ROSE_KEYCODE_N;
            break;
        case SDL_SCANCODE_O:
            return ROSE_KEYCODE_O;
            break;
        case SDL_SCANCODE_P:
            return ROSE_KEYCODE_P;
            break;
        case SDL_SCANCODE_Q:
            return ROSE_KEYCODE_Q;
            break;
        case SDL_SCANCODE_R:
            return ROSE_KEYCODE_R;
            break;
        case SDL_SCANCODE_S:
            return ROSE_KEYCODE_S;
            break;
        case SDL_SCANCODE_T:
            return ROSE_KEYCODE_T;
            break;
        case SDL_SCANCODE_U:
            return ROSE_KEYCODE_U;
            break;
        case SDL_SCANCODE_V:
            return ROSE_KEYCODE_V;
            break;
        case SDL_SCANCODE_W:
            return ROSE_KEYCODE_W;
            break;
        case SDL_SCANCODE_X:
            return ROSE_KEYCODE_X;
            break;
        case SDL_SCANCODE_Y:
            return ROSE_KEYCODE_Y;
            break;
        case SDL_SCANCODE_Z:
            return ROSE_KEYCODE_Z;
            break;
        case SDL_SCANCODE_1:
            return ROSE_KEYCODE_1;
            break;
        case SDL_SCANCODE_2:
            return ROSE_KEYCODE_2;
            break;
        case SDL_SCANCODE_3:
            return ROSE_KEYCODE_3;
            break;
        case SDL_SCANCODE_4:
            return ROSE_KEYCODE_4;
            break;
        case SDL_SCANCODE_5:
            return ROSE_KEYCODE_5;
            break;
        case SDL_SCANCODE_6:
            return ROSE_KEYCODE_6;
            break;
        case SDL_SCANCODE_7:
            return ROSE_KEYCODE_7;
            break;
        case SDL_SCANCODE_8:
            return ROSE_KEYCODE_8;
            break;
        case SDL_SCANCODE_9:
            return ROSE_KEYCODE_9;
            break;
        case SDL_SCANCODE_0:
            return ROSE_KEYCODE_0;
            break;
        case SDL_SCANCODE_RETURN:
            return ROSE_KEYCODE_RETURN;
            break;
        case SDL_SCANCODE_ESCAPE:
            return ROSE_KEYCODE_ESCAPE;
            break;
        case SDL_SCANCODE_BACKSPACE:
            return ROSE_KEYCODE_BACKSPACE;
            break;
        case SDL_SCANCODE_TAB:
            return ROSE_KEYCODE_TAB;
            break;
        case SDL_SCANCODE_SPACE:
            return ROSE_KEYCODE_SPACE;
            break;
        case SDL_SCANCODE_MINUS:
            return ROSE_KEYCODE_MINUS;
            break;
        case SDL_SCANCODE_EQUALS:
            return ROSE_KEYCODE_EQUALS;
            break;
        case SDL_SCANCODE_LEFTBRACKET:
            return ROSE_KEYCODE_LEFTBRACKET;
            break;
        case SDL_SCANCODE_RIGHTBRACKET:
            return ROSE_KEYCODE_RIGHTBRACKET;
            break;
        case SDL_SCANCODE_BACKSLASH:
            return ROSE_KEYCODE_BACKSLASH;
            break;
        case SDL_SCANCODE_NONUSHASH:
            return ROSE_KEYCODE_NONUSHASH;
            break;
        case SDL_SCANCODE_SEMICOLON:
            return ROSE_KEYCODE_SEMICOLON;
            break;
        case SDL_SCANCODE_APOSTROPHE:
            return ROSE_KEYCODE_APOSTROPHE;
            break;
        case SDL_SCANCODE_GRAVE:
            return ROSE_KEYCODE_GRAVE;
            break;
        case SDL_SCANCODE_COMMA:
            return ROSE_KEYCODE_COMMA;
            break;
        case SDL_SCANCODE_PERIOD:
            return ROSE_KEYCODE_PERIOD;
            break;
        case SDL_SCANCODE_SLASH:
            return ROSE_KEYCODE_SLASH;
            break;
        case SDL_SCANCODE_CAPSLOCK:
            return ROSE_KEYCODE_CAPSLOCK;
            break;
        case SDL_SCANCODE_F1:
            return ROSE_KEYCODE_F1;
            break;
        case SDL_SCANCODE_F2:
            return ROSE_KEYCODE_F2;
            break;
        case SDL_SCANCODE_F3:
            return ROSE_KEYCODE_F3;
            break;
        case SDL_SCANCODE_F4:
            return ROSE_KEYCODE_F4;
            break;
        case SDL_SCANCODE_F5:
            return ROSE_KEYCODE_F5;
            break;
        case SDL_SCANCODE_F6:
            return ROSE_KEYCODE_F6;
            break;
        case SDL_SCANCODE_F7:
            return ROSE_KEYCODE_F7;
            break;
        case SDL_SCANCODE_F8:
            return ROSE_KEYCODE_F8;
            break;
        case SDL_SCANCODE_F9:
            return ROSE_KEYCODE_F9;
            break;
        case SDL_SCANCODE_F10:
            return ROSE_KEYCODE_F10;
            break;
        case SDL_SCANCODE_F11:
            return ROSE_KEYCODE_F11;
            break;
        case SDL_SCANCODE_F12:
            return ROSE_KEYCODE_F12;
            break;
        case SDL_SCANCODE_PRINTSCREEN:
            return ROSE_KEYCODE_PRINTSCREEN;
            break;
        case SDL_SCANCODE_SCROLLLOCK:
            return ROSE_KEYCODE_SCROLLLOCK;
            break;
        case SDL_SCANCODE_PAUSE:
            return ROSE_KEYCODE_PAUSE;
            break;
        case SDL_SCANCODE_INSERT:
            return ROSE_KEYCODE_INSERT;
            break;
        case SDL_SCANCODE_HOME:
            return ROSE_KEYCODE_HOME;
            break;
        case SDL_SCANCODE_PAGEUP:
            return ROSE_KEYCODE_PAGEUP;
            break;
        case SDL_SCANCODE_DELETE:
            return ROSE_KEYCODE_DELETE;
            break;
        case SDL_SCANCODE_END:
            return ROSE_KEYCODE_END;
            break;
        case SDL_SCANCODE_PAGEDOWN:
            return ROSE_KEYCODE_PAGEDOWN;
            break;
        case SDL_SCANCODE_RIGHT:
            return ROSE_KEYCODE_RIGHT;
            break;
        case SDL_SCANCODE_LEFT:
            return ROSE_KEYCODE_LEFT;
            break;
        case SDL_SCANCODE_DOWN:
            return ROSE_KEYCODE_DOWN;
            break;
        case SDL_SCANCODE_UP:
            return ROSE_KEYCODE_UP;
            break;
        case SDL_SCANCODE_NUMLOCKCLEAR:
            return ROSE_KEYCODE_NUMLOCKCLEAR;
            break;
        case SDL_SCANCODE_KP_DIVIDE:
            return ROSE_KEYCODE_KP_DIVIDE;
            break;
        case SDL_SCANCODE_KP_MULTIPLY:
            return ROSE_KEYCODE_KP_MULTIPLY;
            break;
        case SDL_SCANCODE_KP_MINUS:
            return ROSE_KEYCODE_KP_MINUS;
            break;
        case SDL_SCANCODE_KP_PLUS:
            return ROSE_KEYCODE_KP_PLUS;
            break;
        case SDL_SCANCODE_KP_ENTER:
            return ROSE_KEYCODE_KP_ENTER;
            break;
        case SDL_SCANCODE_KP_1:
            return ROSE_KEYCODE_KP_1;
            break;
        case SDL_SCANCODE_KP_2:
            return ROSE_KEYCODE_KP_2;
            break;
        case SDL_SCANCODE_KP_3:
            return ROSE_KEYCODE_KP_3;
            break;
        case SDL_SCANCODE_KP_4:
            return ROSE_KEYCODE_KP_4;
            break;
        case SDL_SCANCODE_KP_5:
            return ROSE_KEYCODE_KP_5;
            break;
        case SDL_SCANCODE_KP_6:
            return ROSE_KEYCODE_KP_6;
            break;
        case SDL_SCANCODE_KP_7:
            return ROSE_KEYCODE_KP_7;
            break;
        case SDL_SCANCODE_KP_8:
            return ROSE_KEYCODE_KP_8;
            break;
        case SDL_SCANCODE_KP_9:
            return ROSE_KEYCODE_KP_9;
            break;
        case SDL_SCANCODE_KP_0:
            return ROSE_KEYCODE_KP_0;
            break;
        case SDL_SCANCODE_KP_PERIOD:
            return ROSE_KEYCODE_KP_PERIOD;
            break;
        case SDL_SCANCODE_NONUSBACKSLASH:
            return ROSE_KEYCODE_NONUSBACKSLASH;
            break;
        case SDL_SCANCODE_APPLICATION:
            return ROSE_KEYCODE_APPLICATION;
            break;
        case SDL_SCANCODE_POWER:
            return ROSE_KEYCODE_POWER;
            break;
        case SDL_SCANCODE_KP_EQUALS:
            return ROSE_KEYCODE_KP_EQUALS;
            break;
        case SDL_SCANCODE_F13:
            return ROSE_KEYCODE_F13;
            break;
        case SDL_SCANCODE_F14:
            return ROSE_KEYCODE_F14;
            break;
        case SDL_SCANCODE_F15:
            return ROSE_KEYCODE_F15;
            break;
        case SDL_SCANCODE_F16:
            return ROSE_KEYCODE_F16;
            break;
        case SDL_SCANCODE_F17:
            return ROSE_KEYCODE_F17;
            break;
        case SDL_SCANCODE_F18:
            return ROSE_KEYCODE_F18;
            break;
        case SDL_SCANCODE_F19:
            return ROSE_KEYCODE_F19;
            break;
        case SDL_SCANCODE_F20:
            return ROSE_KEYCODE_F20;
            break;
        case SDL_SCANCODE_F21:
            return ROSE_KEYCODE_F21;
            break;
        case SDL_SCANCODE_F22:
            return ROSE_KEYCODE_F22;
            break;
        case SDL_SCANCODE_F23:
            return ROSE_KEYCODE_F23;
            break;
        case SDL_SCANCODE_F24:
            return ROSE_KEYCODE_F24;
            break;
        case SDL_SCANCODE_EXECUTE:
            return ROSE_KEYCODE_EXECUTE;
            break;
        case SDL_SCANCODE_HELP:
            return ROSE_KEYCODE_HELP;
            break;
        case SDL_SCANCODE_MENU:
            return ROSE_KEYCODE_MENU;
            break;
        case SDL_SCANCODE_SELECT:
            return ROSE_KEYCODE_SELECT;
            break;
        case SDL_SCANCODE_STOP:
            return ROSE_KEYCODE_STOP;
            break;
        case SDL_SCANCODE_AGAIN:
            return ROSE_KEYCODE_AGAIN;
            break;
        case SDL_SCANCODE_UNDO:
            return ROSE_KEYCODE_UNDO;
            break;
        case SDL_SCANCODE_CUT:
            return ROSE_KEYCODE_CUT;
            break;
        case SDL_SCANCODE_COPY:
            return ROSE_KEYCODE_COPY;
            break;
        case SDL_SCANCODE_PASTE:
            return ROSE_KEYCODE_PASTE;
            break;
        case SDL_SCANCODE_FIND:
            return ROSE_KEYCODE_FIND;
            break;
        case SDL_SCANCODE_MUTE:
            return ROSE_KEYCODE_MUTE;
            break;
        case SDL_SCANCODE_VOLUMEUP:
            return ROSE_KEYCODE_VOLUMEUP;
            break;
        case SDL_SCANCODE_VOLUMEDOWN:
            return ROSE_KEYCODE_VOLUMEDOWN;
            break;
        case SDL_SCANCODE_KP_COMMA:
            return ROSE_KEYCODE_KP_COMMA;
            break;
        case SDL_SCANCODE_KP_EQUALSAS400:
            return ROSE_KEYCODE_KP_EQUALSAS400;
            break;
        case SDL_SCANCODE_INTERNATIONAL1:
            return ROSE_KEYCODE_INTERNATIONAL1;
            break;
        case SDL_SCANCODE_INTERNATIONAL2:
            return ROSE_KEYCODE_INTERNATIONAL2;
            break;
        case SDL_SCANCODE_INTERNATIONAL3:
            return ROSE_KEYCODE_INTERNATIONAL3;
            break;
        case SDL_SCANCODE_INTERNATIONAL4:
            return ROSE_KEYCODE_INTERNATIONAL4;
            break;
        case SDL_SCANCODE_INTERNATIONAL5:
            return ROSE_KEYCODE_INTERNATIONAL5;
            break;
        case SDL_SCANCODE_INTERNATIONAL6:
            return ROSE_KEYCODE_INTERNATIONAL6;
            break;
        case SDL_SCANCODE_INTERNATIONAL7:
            return ROSE_KEYCODE_INTERNATIONAL7;
            break;
        case SDL_SCANCODE_INTERNATIONAL8:
            return ROSE_KEYCODE_INTERNATIONAL8;
            break;
        case SDL_SCANCODE_INTERNATIONAL9:
            return ROSE_KEYCODE_INTERNATIONAL9;
            break;
        case SDL_SCANCODE_LANG1:
            return ROSE_KEYCODE_LANG1;
            break;
        case SDL_SCANCODE_LANG2:
            return ROSE_KEYCODE_LANG2;
            break;
        case SDL_SCANCODE_LANG3:
            return ROSE_KEYCODE_LANG3;
            break;
        case SDL_SCANCODE_LANG4:
            return ROSE_KEYCODE_LANG4;
            break;
        case SDL_SCANCODE_LANG5:
            return ROSE_KEYCODE_LANG5;
            break;
        case SDL_SCANCODE_LANG6:
            return ROSE_KEYCODE_LANG6;
            break;
        case SDL_SCANCODE_LANG7:
            return ROSE_KEYCODE_LANG7;
            break;
        case SDL_SCANCODE_LANG8:
            return ROSE_KEYCODE_LANG8;
            break;
        case SDL_SCANCODE_LANG9:
            return ROSE_KEYCODE_LANG9;
            break;
        case SDL_SCANCODE_ALTERASE:
            return ROSE_KEYCODE_ALTERASE;
            break;
        case SDL_SCANCODE_SYSREQ:
            return ROSE_KEYCODE_SYSREQ;
            break;
        case SDL_SCANCODE_CANCEL:
            return ROSE_KEYCODE_CANCEL;
            break;
        case SDL_SCANCODE_CLEAR:
            return ROSE_KEYCODE_CLEAR;
            break;
        case SDL_SCANCODE_PRIOR:
            return ROSE_KEYCODE_PRIOR;
            break;
        case SDL_SCANCODE_RETURN2:
            return ROSE_KEYCODE_RETURN2;
            break;
        case SDL_SCANCODE_SEPARATOR:
            return ROSE_KEYCODE_SEPARATOR;
            break;
        case SDL_SCANCODE_OUT:
            return ROSE_KEYCODE_OUT;
            break;
        case SDL_SCANCODE_OPER:
            return ROSE_KEYCODE_OPER;
            break;
        case SDL_SCANCODE_CLEARAGAIN:
            return ROSE_KEYCODE_CLEARAGAIN;
            break;
        case SDL_SCANCODE_CRSEL:
            return ROSE_KEYCODE_CRSEL;
            break;
        case SDL_SCANCODE_EXSEL:
            return ROSE_KEYCODE_EXSEL;
            break;
        case SDL_SCANCODE_KP_00:
            return ROSE_KEYCODE_KP_00;
            break;
        case SDL_SCANCODE_KP_000:
            return ROSE_KEYCODE_KP_000;
            break;
        case SDL_SCANCODE_THOUSANDSSEPARATOR:
            return ROSE_KEYCODE_THOUSANDSSEPARATOR;
            break;
        case SDL_SCANCODE_DECIMALSEPARATOR:
            return ROSE_KEYCODE_DECIMALSEPARATOR;
            break;
        case SDL_SCANCODE_CURRENCYUNIT:
            return ROSE_KEYCODE_CURRENCYUNIT;
            break;
        case SDL_SCANCODE_CURRENCYSUBUNIT:
            return ROSE_KEYCODE_CURRENCYSUBUNIT;
            break;
        case SDL_SCANCODE_KP_LEFTPAREN:
            return ROSE_KEYCODE_KP_LEFTPAREN;
            break;
        case SDL_SCANCODE_KP_RIGHTPAREN:
            return ROSE_KEYCODE_KP_RIGHTPAREN;
            break;
        case SDL_SCANCODE_KP_LEFTBRACE:
            return ROSE_KEYCODE_KP_LEFTBRACE;
            break;
        case SDL_SCANCODE_KP_RIGHTBRACE:
            return ROSE_KEYCODE_KP_RIGHTBRACE;
            break;
        case SDL_SCANCODE_KP_TAB:
            return ROSE_KEYCODE_KP_TAB;
            break;
        case SDL_SCANCODE_KP_BACKSPACE:
            return ROSE_KEYCODE_KP_BACKSPACE;
            break;
        case SDL_SCANCODE_KP_A:
            return ROSE_KEYCODE_KP_A;
            break;
        case SDL_SCANCODE_KP_B:
            return ROSE_KEYCODE_KP_B;
            break;
        case SDL_SCANCODE_KP_C:
            return ROSE_KEYCODE_KP_C;
            break;
        case SDL_SCANCODE_KP_D:
            return ROSE_KEYCODE_KP_D;
            break;
        case SDL_SCANCODE_KP_E:
            return ROSE_KEYCODE_KP_E;
            break;
        case SDL_SCANCODE_KP_F:
            return ROSE_KEYCODE_KP_F;
            break;
        case SDL_SCANCODE_KP_XOR:
            return ROSE_KEYCODE_KP_XOR;
            break;
        case SDL_SCANCODE_KP_POWER:
            return ROSE_KEYCODE_KP_POWER;
            break;
        case SDL_SCANCODE_KP_PERCENT:
            return ROSE_KEYCODE_KP_PERCENT;
            break;
        case SDL_SCANCODE_KP_LESS:
            return ROSE_KEYCODE_KP_LESS;
            break;
        case SDL_SCANCODE_KP_GREATER:
            return ROSE_KEYCODE_KP_GREATER;
            break;
        case SDL_SCANCODE_KP_AMPERSAND:
            return ROSE_KEYCODE_KP_AMPERSAND;
            break;
        case SDL_SCANCODE_KP_DBLAMPERSAND:
            return ROSE_KEYCODE_KP_DBLAMPERSAND;
            break;
        case SDL_SCANCODE_KP_VERTICALBAR:
            return ROSE_KEYCODE_KP_VERTICALBAR;
            break;
        case SDL_SCANCODE_KP_DBLVERTICALBAR:
            return ROSE_KEYCODE_KP_DBLVERTICALBAR;
            break;
        case SDL_SCANCODE_KP_COLON:
            return ROSE_KEYCODE_KP_COLON;
            break;
        case SDL_SCANCODE_KP_HASH:
            return ROSE_KEYCODE_KP_HASH;
            break;
        case SDL_SCANCODE_KP_SPACE:
            return ROSE_KEYCODE_KP_SPACE;
            break;
        case SDL_SCANCODE_KP_AT:
            return ROSE_KEYCODE_KP_AT;
            break;
        case SDL_SCANCODE_KP_EXCLAM:
            return ROSE_KEYCODE_KP_EXCLAM;
            break;
        case SDL_SCANCODE_KP_MEMSTORE:
            return ROSE_KEYCODE_KP_MEMSTORE;
            break;
        case SDL_SCANCODE_KP_MEMRECALL:
            return ROSE_KEYCODE_KP_MEMRECALL;
            break;
        case SDL_SCANCODE_KP_MEMCLEAR:
            return ROSE_KEYCODE_KP_MEMCLEAR;
            break;
        case SDL_SCANCODE_KP_MEMADD:
            return ROSE_KEYCODE_KP_MEMADD;
            break;
        case SDL_SCANCODE_KP_MEMSUBTRACT:
            return ROSE_KEYCODE_KP_MEMSUBTRACT;
            break;
        case SDL_SCANCODE_KP_MEMMULTIPLY:
            return ROSE_KEYCODE_KP_MEMMULTIPLY;
            break;
        case SDL_SCANCODE_KP_MEMDIVIDE:
            return ROSE_KEYCODE_KP_MEMDIVIDE;
            break;
        case SDL_SCANCODE_KP_PLUSMINUS:
            return ROSE_KEYCODE_KP_PLUSMINUS;
            break;
        case SDL_SCANCODE_KP_CLEAR:
            return ROSE_KEYCODE_KP_CLEAR;
            break;
        case SDL_SCANCODE_KP_CLEARENTRY:
            return ROSE_KEYCODE_KP_CLEARENTRY;
            break;
        case SDL_SCANCODE_KP_BINARY:
            return ROSE_KEYCODE_KP_BINARY;
            break;
        case SDL_SCANCODE_KP_OCTAL:
            return ROSE_KEYCODE_KP_OCTAL;
            break;
        case SDL_SCANCODE_KP_DECIMAL:
            return ROSE_KEYCODE_KP_DECIMAL;
            break;
        case SDL_SCANCODE_KP_HEXADECIMAL:
            return ROSE_KEYCODE_KP_HEXADECIMAL;
            break;
        case SDL_SCANCODE_LCTRL:
            return ROSE_KEYCODE_LCTRL;
            break;
        case SDL_SCANCODE_LSHIFT:
            return ROSE_KEYCODE_LSHIFT;
            break;
        case SDL_SCANCODE_LALT:
            return ROSE_KEYCODE_LALT;
            break;
        case SDL_SCANCODE_LGUI:
            return ROSE_KEYCODE_LGUI;
            break;
        case SDL_SCANCODE_RCTRL:
            return ROSE_KEYCODE_RCTRL;
            break;
        case SDL_SCANCODE_RSHIFT:
            return ROSE_KEYCODE_RSHIFT;
            break;
        case SDL_SCANCODE_RALT:
            return ROSE_KEYCODE_RALT;
            break;
        case SDL_SCANCODE_RGUI:
            return ROSE_KEYCODE_RGUI;
            break;
        case SDL_SCANCODE_MODE:
            return ROSE_KEYCODE_MODE;
            break;
        case SDL_SCANCODE_AUDIONEXT:
            return ROSE_KEYCODE_AUDIONEXT;
            break;
        case SDL_SCANCODE_AUDIOPREV:
            return ROSE_KEYCODE_AUDIOPREV;
            break;
        case SDL_SCANCODE_AUDIOSTOP:
            return ROSE_KEYCODE_AUDIOSTOP;
            break;
        case SDL_SCANCODE_AUDIOPLAY:
            return ROSE_KEYCODE_AUDIOPLAY;
            break;
        case SDL_SCANCODE_AUDIOMUTE:
            return ROSE_KEYCODE_AUDIOMUTE;
            break;
        case SDL_SCANCODE_MEDIASELECT:
            return ROSE_KEYCODE_MEDIASELECT;
            break;
        case SDL_SCANCODE_WWW:
            return ROSE_KEYCODE_WWW;
            break;
        case SDL_SCANCODE_MAIL:
            return ROSE_KEYCODE_MAIL;
            break;
        case SDL_SCANCODE_CALCULATOR:
            return ROSE_KEYCODE_CALCULATOR;
            break;
        case SDL_SCANCODE_COMPUTER:
            return ROSE_KEYCODE_COMPUTER;
            break;
        case SDL_SCANCODE_AC_SEARCH:
            return ROSE_KEYCODE_AC_SEARCH;
            break;
        case SDL_SCANCODE_AC_HOME:
            return ROSE_KEYCODE_AC_HOME;
            break;
        case SDL_SCANCODE_AC_BACK:
            return ROSE_KEYCODE_AC_BACK;
            break;
        case SDL_SCANCODE_AC_FORWARD:
            return ROSE_KEYCODE_AC_FORWARD;
            break;
        case SDL_SCANCODE_AC_STOP:
            return ROSE_KEYCODE_AC_STOP;
            break;
        case SDL_SCANCODE_AC_REFRESH:
            return ROSE_KEYCODE_AC_REFRESH;
            break;
        case SDL_SCANCODE_AC_BOOKMARKS:
            return ROSE_KEYCODE_AC_BOOKMARKS;
            break;
        case SDL_SCANCODE_BRIGHTNESSDOWN:
            return ROSE_KEYCODE_BRIGHTNESSDOWN;
            break;
        case SDL_SCANCODE_BRIGHTNESSUP:
            return ROSE_KEYCODE_BRIGHTNESSUP;
            break;
        case SDL_SCANCODE_DISPLAYSWITCH:
            return ROSE_KEYCODE_DISPLAYSWITCH;
            break;
        case SDL_SCANCODE_KBDILLUMTOGGLE:
            return ROSE_KEYCODE_KBDILLUMTOGGLE;
            break;
        case SDL_SCANCODE_KBDILLUMDOWN:
            return ROSE_KEYCODE_KBDILLUMDOWN;
            break;
        case SDL_SCANCODE_KBDILLUMUP:
            return ROSE_KEYCODE_KBDILLUMUP;
            break;
        case SDL_SCANCODE_EJECT:
            return ROSE_KEYCODE_EJECT;
            break;
        case SDL_SCANCODE_SLEEP:
            return ROSE_KEYCODE_SLEEP;
            break;
        case SDL_SCANCODE_APP1:
            return ROSE_KEYCODE_APP1;
            break;
        case SDL_SCANCODE_APP2:
            return ROSE_KEYCODE_APP2;
            break;
        default:
            return ROSE_KEYCODE_UNKNOWN;
    }
}