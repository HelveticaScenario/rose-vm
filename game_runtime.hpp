#ifndef GAME_RUNTIME_HPP
#define GAME_RUNTIME_HPP

#include "config.hpp"
#include "cartridge.hpp"
#include "runtime_interface.hpp"
#include <vector>

class GameRuntime: public RuntimeInterface {
	public:
		GameRuntime(Cartridge* cart);
		~GameRuntime();

		void update();
		void draw();
		RuntimeMemorySegment getScreen();
		RuntimeMemorySegment getPalette();
		void setModeChangeCallback(std::function<void(ScreenMode)> callback);
		void updateMouseState(const MouseState& mouseState);
	private:
		Cartridge* cartridge;
		RuntimeMemory* mem;


};

#endif