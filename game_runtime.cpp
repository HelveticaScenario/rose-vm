#include "game_runtime.hpp"

GameRuntime::GameRuntime(Cartridge* cart) {
	this->cartridge = cart;
}

GameRuntime::~GameRuntime() {
	
}

void GameRuntime::update() {

}

void GameRuntime::draw() {
	
}

RuntimeMemorySegment GameRuntime::getScreen() {

}

RuntimeMemorySegment GameRuntime::getPalette() {
	
}

void GameRuntime::setModeChangeCallback(std::function<void(ScreenMode)> callback) {

}

void GameRuntime::updateMouseState(const MouseState& mouseState) {
	
}