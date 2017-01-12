#pragma once

#include "enums.h"

rose_api_error rose_api_memory_poke(rose_rt* r, uint32_t idx, uint8_t val);

rose_api_error rose_api_memory_peek(rose_rt* r, uint32_t idx, uint8_t* res);

rose_api_error rose_api_memory_memcpy(rose_rt* r, uint32_t dest_addr, uint32_t source_addr, uint32_t len);

rose_api_error rose_api_memory_memset(rose_rt* r, uint32_t dest_addr, uint8_t val, uint32_t len);

rose_api_error rose_api_memory_cstore(rose_rt* r, uint32_t dest_addr, uint32_t source_addr, uint32_t len);

rose_api_error rose_api_memory_reload(rose_rt* r, uint32_t dest_addr, uint32_t source_addr, uint32_t len);

