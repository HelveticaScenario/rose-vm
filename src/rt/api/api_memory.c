#include "rt/api/api_memory.h"

Rose_RuntimeApiError rose_api_memory_poke(Rose_RuntimeBase* r, uint32_t idx, uint8_t val) {
    if (idx >= r->mem_size) {
        return ROSE_API_ERR_OUT_OF_BOUNDS;
    }
    r->mem[idx] = val;
    return ROSE_API_ERR_NONE;
}

Rose_RuntimeApiError rose_api_memory_peek(Rose_RuntimeBase* r, uint32_t idx, uint8_t* res) {
    if (idx >= r->mem_size) {
        return ROSE_API_ERR_OUT_OF_BOUNDS;
    }
    *res = r->mem[idx];
    return ROSE_API_ERR_NONE;
}


Rose_RuntimeApiError rose_api_memory_memcpy(Rose_RuntimeBase* r, uint32_t dest_addr, uint32_t source_addr, uint32_t len) {
    if (dest_addr >= r->mem_size || source_addr >= r->mem_size || (dest_addr + len) >= r->mem_size || (source_addr + len) >= r->mem_size) {
        return ROSE_API_ERR_OUT_OF_BOUNDS;
    }
    memcpy(r->mem + dest_addr, r->mem + source_addr, len);
    return ROSE_API_ERR_NONE;
}

Rose_RuntimeApiError rose_api_memory_memset(Rose_RuntimeBase* r, uint32_t dest_addr, uint8_t val, uint32_t len) {
    if (dest_addr >= r->mem_size || (dest_addr + len) >= r->mem_size) {
        return ROSE_API_ERR_OUT_OF_BOUNDS;
    }
    memset(r->mem + dest_addr, val, len);
    return ROSE_API_ERR_NONE;
}
