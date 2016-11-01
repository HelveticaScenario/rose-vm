#include "cartridge.hpp"

Cartridge::Cartridge() {
	this->code = "";
	this->data = new unsigned char[256*256];
}

Cartridge::~Cartridge() {
	delete this->data;
}