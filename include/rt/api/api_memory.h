#ifndef ROSE_API_MEMORY_H
#define ROSE_API_MEMORY_H

#include "config.h"
#include "rt/rt_base.h"

Rose_RuntimeApiError rose_api_memory_poke(Rose_RuntimeBase* r, uint32_t idx, uint8_t val);

Rose_RuntimeApiError rose_api_memory_peek(Rose_RuntimeBase* r, uint32_t idx, uint8_t* res);

Rose_RuntimeApiError rose_api_memory_memcpy(Rose_RuntimeBase* r, uint32_t dest_addr, uint32_t source_addr, uint32_t len);

Rose_RuntimeApiError rose_api_memory_memset(Rose_RuntimeBase* r, uint32_t dest_addr, uint8_t val, uint32_t len);


#endif