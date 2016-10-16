#include "runtime.hpp"

Runtime::Runtime(const Cartridge* cart) {
	this->cartridge = cart;
}

Runtime::~Runtime() {
	
}

Runtime::doThing() {
	this->cartridge->thing = 12;
}