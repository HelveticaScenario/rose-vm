#include "editor_runtime.hpp"

EditorRuntime::EditorRuntime(const Cartridge* cart) {
	this->cartridge = cart;
}

EditorRuntime::~EditorRuntime() {
	
}

void EditorRuntime::update() {

}

void EditorRuntime::draw() {
	
}

unsigned char* EditorRuntime::getScreen() {

}

unsigned char* EditorRuntime::getPalette() {

}

void EditorRuntime::setModeChangeCallback(std::function<void(ScreenMode)> callback) {

}

void EditorRuntime::updateMouseState(const MouseState& mouseState) {
	
}