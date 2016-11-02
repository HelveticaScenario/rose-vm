#ifndef RUNTIME_GAME_HPP
#define RUNTIME_GAME_HPP

#include "config.hpp"
#include "cartridge.hpp"
#include "runtime_interface.hpp"
#include "sol.hpp"
#include <vector>
#include <cstring>
#include <iostream>

class RuntimeGame : public RuntimeInterface {
public:
  RuntimeGame(Cartridge *cart);
  ~RuntimeGame();
  void reload();
  void init();
  void update();
  void draw();
  RuntimeMemorySegment getScreen();
  RuntimeMemorySegment getPalette();
  void setModeChangeCallback(std::function<void(ScreenMode)> callback);
  void updateMouseState(const MouseState &mouseState);

private:
  Cartridge *cartridge;
  RuntimeMemory mem;
  RuntimeMemorySegment screen;
  RuntimeMemorySegment palette;
  sol::state lua;
  std::function<void(ScreenMode)> modeChangeCallback;
};

#endif
