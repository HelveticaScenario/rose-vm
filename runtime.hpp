#ifndef RUNTIME_HPP
#define RUNTIME_HPP

#include "config.hpp"
#include "cartridge.hpp"

class Runtime {
	public:
		Runtime(const Cartridge* cart);
		~Runtime();
	
	private:
		const Cartridge* cartridge;

};

#endif