#ifndef EDITOR_RUNTIME_HPP
#define EDITOR_RUNTIME_HPP

#include "config.hpp"
#include "cartridge.hpp"
#include "runtime_interface.hpp"

class EditorRuntime: public RuntimeInterface {
	public:
		EditorRuntime(Cartridge* cart);
		~EditorRuntime();

		void update();
		void draw();
		RuntimeMemorySegment getScreen();
		RuntimeMemorySegment getPalette();
		void setModeChangeCallback(std::function<void(ScreenMode)> callback);
		void updateMouseState(const MouseState& mouseState);
	private:
		Cartridge* cartridge;
};

#endif