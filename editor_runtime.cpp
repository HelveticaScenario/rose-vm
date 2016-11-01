#include "editor_runtime.hpp"

EditorRuntime::EditorRuntime(Cartridge* cart) {
	this->cartridge = cart;
}

EditorRuntime::~EditorRuntime() {
	
}

void EditorRuntime::update() {

}

void EditorRuntime::draw() {
	
}

RuntimeMemorySegment EditorRuntime::getScreen() {
	
}

RuntimeMemorySegment EditorRuntime::getPalette() {

}

void EditorRuntime::setModeChangeCallback(std::function<void(ScreenMode)> callback) {

}

void EditorRuntime::updateMouseState(const MouseState& mouseState) {
	
}