#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "config.hpp"
#include "cartridge.hpp"

class Editor {
	public:
		Editor(const Cartridge* cart);
		~Editor();
	private:
		const Cartridge* cartridge;
};

#endif