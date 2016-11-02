#ifndef CARTRIDGE_HPP
#define CARTRIDGE_HPP

#include "config.hpp"
#include <string>
#include <array>

class Cartridge {
public:
  Cartridge();
  ~Cartridge();

  std::string code;
  std::array<unsigned char,
             ROSE_MEMORY_SIZE - ROSE_RUNTIME_RESERVED_MEMORY_SIZE>
      data;
};

#endif
