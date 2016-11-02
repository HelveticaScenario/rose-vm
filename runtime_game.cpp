#include "runtime_game.hpp"
#include "fun.h"

RuntimeGame::RuntimeGame(Cartridge *cart) {
  this->cartridge = cart;
  this->mem = RuntimeMemory{};
  auto end = this->mem.end();
  auto beg_screen = end - (ROSE_SCREEN_SIZE);
  auto beg_palette = beg_screen - (ROSE_PALETTE_SIZE);
  this->screen = RuntimeMemorySegment(beg_screen, end);
  this->palette = RuntimeMemorySegment(beg_palette, beg_screen);
  auto default_palette = make_default_palette();
  std::memcpy(beg_palette, default_palette.data(), default_palette.size());
  this->reload();
}

RuntimeGame::~RuntimeGame() {}

void RuntimeGame::reload() {
  this->lua = sol::state();
  this->lua.open_libraries();
  std::memcpy(this->mem.begin(), this->cartridge->data.begin(),
              this->cartridge->data.size());
  sol::load_result lua_fun = this->lua.load_buffer((const char *)luaJIT_BC_fun,
                                                   luaJIT_BC_fun_SIZE, "fun");
  lua_fun();
  sol::load_result code = this->lua.load(this->cartridge->code);
  code(); // execute
}

void RuntimeGame::init() {
  sol::optional<sol::protected_function> init = this->lua["_init"];
  init && init->call();
}

void RuntimeGame::update() {
  sol::optional<sol::protected_function> update = this->lua["_update"];
  update && update->call();
}

void RuntimeGame::draw() {
  sol::optional<sol::protected_function> draw = this->lua["_draw"];
  draw && draw->call();
}

inline RuntimeMemorySegment RuntimeGame::getScreen() { return this->screen; }

inline RuntimeMemorySegment RuntimeGame::getPalette() { return this->palette; }

void RuntimeGame::setModeChangeCallback(
    std::function<void(ScreenMode)> callback) {
  this->modeChangeCallback = callback;
}

void RuntimeGame::updateMouseState(const MouseState &mouseState) {}