#ifndef RUNTIME_EDITOR_HPP
#define RUNTIME_EDITOR_HPP

#include "config.hpp"
#include "cartridge.hpp"
#include "runtime_interface.hpp"

class RuntimeEditor { //: public RuntimeInterface {
public:
  RuntimeEditor(Cartridge *cart);
  ~RuntimeEditor();

  void update();
  void draw();
  RuntimeMemorySegment getScreen();
  RuntimeMemorySegment getPalette();
  void setModeChangeCallback(std::function<void(ScreenMode)> callback);
  void updateMouseState(const MouseState &mouseState);

private:
  Cartridge *cartridge;
  RuntimeMemorySegment screen;
  RuntimeMemorySegment palette;
};

#endif
