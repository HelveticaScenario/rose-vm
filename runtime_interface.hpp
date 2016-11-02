#ifndef RUNTIME_INTERFACE_HPP
#define RUNTIME_INTERFACE_HPP

#include "config.hpp"
#include <functional>
#include <iterator>
#include <utility>
#include <array>
#include "system_interface.hpp"

struct MouseState {
  short x;
  short y;
  short relX;
  short relY;
  bool leftBtnDown;
  bool middleBtnDown;
  bool rightBtnDown;
  bool x1BtnDown;
  bool x2BtnDown;
};

std::array<unsigned char, 16 * 3> make_default_palette();

using RuntimeMemory = std::array<unsigned char, ROSE_MEMORY_SIZE>;
using RuntimeMemorySegment =
    std::pair<RuntimeMemory::iterator, RuntimeMemory::iterator>;

class RuntimeInterface {
public:
  virtual void reload() = 0;
  virtual void init() = 0;
  virtual void update() = 0;
  virtual void draw() = 0;
  virtual RuntimeMemorySegment getScreen() = 0;
  virtual RuntimeMemorySegment getPalette() = 0;
  virtual void
  setModeChangeCallback(std::function<void(ScreenMode)> callback) = 0;
  virtual void updateMouseState(const MouseState &mouseState) = 0;
};
#endif
