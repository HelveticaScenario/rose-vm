#include "runtime_editor.hpp"

RuntimeEditor::RuntimeEditor(Cartridge *cart) { this->cartridge = cart; }

RuntimeEditor::~RuntimeEditor() {}

void RuntimeEditor::update() {}

void RuntimeEditor::draw() {}

inline RuntimeMemorySegment RuntimeEditor::getScreen() { return this->screen; }

inline RuntimeMemorySegment RuntimeEditor::getPalette() {
  return this->palette;
}

void RuntimeEditor::setModeChangeCallback(
    std::function<void(ScreenMode)> callback) {}

void RuntimeEditor::updateMouseState(const MouseState &mouseState) {}
