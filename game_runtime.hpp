#ifndef GAME_RUNTIME_HPP
#define GAME_RUNTIME_HPP

#include "config.hpp"
#include "cartridge.hpp"
#include "runtime_interface.hpp"

class GameRuntime: public RuntimeInterface {
	public:
		GameRuntime(const Cartridge* cart);
		~GameRuntime();

		void update();
		void draw();
		unsigned char* getScreen();
		unsigned char* getPalette();
		void setModeChangeCallback(std::function<void(ScreenMode)> callback);
		void updateMouseState(const MouseState& mouseState);
	private:
		const Cartridge* cartridge;

};

#endif