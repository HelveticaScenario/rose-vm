#include "system_sdl2.hpp"
#include <iostream>

SystemSdl2::SystemSdl2() {

  // Initialize
  window = NULL;
  renderer = NULL;
  texture = NULL;
  cartridge = NULL;
  editor = NULL;
  game = NULL;
  screenMode = GameMode;
  pixels = NULL;
  pitch = 0;
  windowWidth = 0;
  windowHeight = 0;
  widthMult = 0;
  heightMult = 0;
}

bool SystemSdl2::init(int argc, char *argv[]) {
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
  window = SDL_CreateWindow(
      "Rosebud", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      ROSE_INITIAL_WINDOW_WIDTH, ROSE_INITIAL_WINDOW_HEIGHT,
      SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
  if (window == NULL) {
    printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
    return false;
  }

  SDL_GL_GetDrawableSize(this->window, &windowWidth, &windowHeight);
  widthMult = windowWidth / ROSE_SCREEN_WIDTH;
  heightMult = windowHeight / ROSE_SCREEN_HEIGHT;

  // Create renderer for window
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                                SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) {
    printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
    return false;
  }

  // Initialize renderer color
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);

  // Create uninitialized texture
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24,
                              SDL_TEXTUREACCESS_STREAMING, ROSE_SCREEN_WIDTH,
                              ROSE_SCREEN_HEIGHT);
  if (texture == NULL) {
    printf("Unable to create blank texture! SDL Error: %s\n", SDL_GetError());
    return false;
  }

  cartridge = new Cartridge();
  cartridge->code = R"(
function _init()
	n = 100
	print(reduce(operator.add, 0, map(function(x) return x^2 end, range(n))))
end

function _update()
	print('update')
end

function _draw()
	print('draw')
end
)";

  game = new RuntimeGame(cartridge);
  editor = new RuntimeEditor(cartridge);
  game->init();

  return true;
}

void SystemSdl2::run() {
  Uint32 windowID = SDL_GetWindowID(this->window);

  // Main loop flag
  bool quit = false;

  // Event handler
  SDL_Event event;
  // While application is running
  SDL_StartTextInput();

  while (!quit) {
    // Handle events on queue
    while (SDL_PollEvent(&event) != 0) {
      switch (event.type) {
      case SDL_QUIT: {
        quit = true;
        break;
      }
      case SDL_WINDOWEVENT: {
        if (event.window.windowID == windowID) {
          switch (event.window.event) {

          case SDL_WINDOWEVENT_SIZE_CHANGED: {
            SDL_GL_GetDrawableSize(this->window, &windowWidth, &windowHeight);
            widthMult = windowWidth / ROSE_SCREEN_WIDTH;
            heightMult = windowHeight / ROSE_SCREEN_HEIGHT;
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
        printf("%d %d %d %d %d\n", event.motion.x, event.motion.y,
               event.motion.xrel, event.motion.yrel, event.motion.state);
        break;
      }
      case SDL_MOUSEBUTTONDOWN: {

        break;
      }
      case SDL_MOUSEBUTTONUP: {

        break;
      }
      case SDL_MOUSEWHEEL: {
      }
      }
      // User requests quit
      if (event.type == SDL_QUIT) {
        quit = true;
      }
    }
    this->game->update();
    this->game->draw();
    if (this->lockTexture()) {
      Uint8 *pixels = (Uint8 *)this->pixels;
      int pitch = this->pitch;

      int pixelCount = (pitch)*ROSE_SCREEN_HEIGHT;
      auto screen = game->getScreen();
      auto palette = game->getPalette();
      for (auto it = screen.first; it != screen.second; ++it) {
        auto i = std::distance(screen.first, it);
        auto c = *it;
        pixels[(i * 3) + 0] = *(palette.first + (c * 3) + 0);
        pixels[(i * 3) + 1] = *(palette.first + (c * 3) + 1);
        pixels[(i * 3) + 2] = *(palette.first + (c * 3) + 2);
      }
      this->unlockTexture();
    }
    this->render();
    // return;
  }
}

SystemSdl2::~SystemSdl2() {
  // Deallocate

  // Free texture if it exists
  if (texture != NULL) {
    SDL_DestroyTexture(texture);
    texture = NULL;
    pixels = NULL;
    pitch = 0;
  }

  if (renderer != NULL) {
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
  }

  if (window != NULL) {
    SDL_DestroyWindow(window);
    window = NULL;
  }
  SDL_Quit();
}

void SystemSdl2::render() {
  SDL_RenderClear(renderer);
  unsigned int mult = widthMult < heightMult ? widthMult : heightMult;

  SDL_Rect rect;

  rect.w = ROSE_SCREEN_WIDTH * mult;
  rect.h = ROSE_SCREEN_HEIGHT * mult;
  rect.x = (windowWidth - rect.w) / 2;
  rect.y = (windowHeight - rect.h) / 2;
  SDL_RenderCopy(renderer, texture, NULL, &rect);
  SDL_RenderPresent(renderer);
}

bool SystemSdl2::lockTexture() {
  // Texture is already locked
  if (pixels != NULL) {
    printf("Texture is already locked!\n");
    return false;
  }

  if (SDL_LockTexture(texture, NULL, &pixels, &pitch) != 0) {
    printf("Unable to lock texture! %s\n", SDL_GetError());
    return false;
  }

  return true;
}

bool SystemSdl2::unlockTexture() {
  // Texture is not locked
  if (pixels == NULL) {
    printf("Texture is not locked!\n");
    return false;
  }

  SDL_UnlockTexture(texture);
  pixels = NULL;
  pitch = 0;

  return true;
}

void SystemSdl2::copyPixels(void *newPixels) {
  // Texture is locked
  if (pixels != NULL) {
    // Copy to locked pixels
    memcpy(pixels, newPixels, pitch * ROSE_SCREEN_HEIGHT);
  }
}
