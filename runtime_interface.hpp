#ifndef RUNTIME_INTERFACE_HPP
#define RUNTIME_INTERFACE_HPP

#include <functional>
#include "system_interface.hpp"

struct MouseState {
	short x;
	short y;
	short relx;
	short rely;
	bool leftBtnDown;
	bool middleBtnDown;
	bool rightBtnDown;
	bool x1BtnDown;
	bool x2BtnDown;
};

class RuntimeInterface {
	public:
		virtual void update() = 0;
		virtual void draw() = 0;
		virtual unsigned char* getScreen() = 0;
		virtual unsigned char* getPalette() = 0;
		virtual void setModeChangeCallback(std::function<void(ScreenMode)> callback) = 0;
		virtual void updateMouseState(const MouseState& mouseState) = 0;
};

#endif