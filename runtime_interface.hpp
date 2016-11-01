#ifndef RUNTIME_INTERFACE_HPP
#define RUNTIME_INTERFACE_HPP

#include <functional>
#include <iterator>
#include <utility>
#include <array>
#include "system_interface.hpp"

struct MouseState {
	short x;
	short y;
	short relX;
	short relY;
	bool leftBtnDown;
	bool middleBtnDown;
	bool rightBtnDown;
	bool x1BtnDown;
	bool x2BtnDown;
};

using RuntimeMemory = std::array<unsigned char, 1024*1024>;
using RuntimeMemorySegment = std::pair<RuntimeMemory::iterator, RuntimeMemory::size_type>;

class RuntimeInterface {
	public:
		virtual void update() = 0;
		virtual void draw() = 0;
		virtual RuntimeMemorySegment getScreen() = 0;
		virtual RuntimeMemorySegment getPalette() = 0;
		virtual void setModeChangeCallback(std::function<void(ScreenMode)> callback) = 0;
		virtual void updateMouseState(const MouseState& mouseState) = 0;
};

#endif