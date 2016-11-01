#ifndef CARTRIDGE_HPP
#define CARTRIDGE_HPP

#include "config.hpp"
#include <string>

class Cartridge {
	public:
		Cartridge();
		~Cartridge();

		std::string code;
		unsigned char* data;
};

#endif