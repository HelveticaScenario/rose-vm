#ifndef EDITOR_RUNTIME_HPP
#define EDITOR_RUNTIME_HPP

#include "config.hpp"
#include "cartridge.hpp"
#include "runtime_interface.hpp"

class EditorRuntime: public RuntimeInterface {
	public:
		EditorRuntime(const Cartridge* cart);
		~EditorRuntime();

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