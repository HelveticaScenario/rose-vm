#include "rt/api/api_memory.h"

rose_runtime_api_error rose_api_memory_poke(rose_runtime_base* r, uint32_t idx, uint8_t val) {
    if (idx >= r->mem_size) {
        return ROSE_API_ERR_OUT_OF_BOUNDS;
    }
    r->mem[idx] = val;
    return ROSE_API_ERR_NONE;
}

rose_runtime_api_error rose_api_memory_peek(rose_runtime_base* r, uint32_t idx, uint8_t* res) {
    if (idx >= r->mem_size) {
        return ROSE_API_ERR_OUT_OF_BOUNDS;
    }
    *res = r->mem[idx];
    return ROSE_API_ERR_NONE;
}


rose_runtime_api_error rose_api_memory_memcpy(rose_runtime_base* r, uint32_t dest_addr, uint32_t source_addr, uint32_t len) {
    if (dest_addr >= r->mem_size || source_addr >= r->mem_size || (dest_addr + len) >= r->mem_size || (source_addr + len) >= r->mem_size) {
        return ROSE_API_ERR_OUT_OF_BOUNDS;
    }
    memcpy(r->mem + dest_addr, r->mem + source_addr, len);
    return ROSE_API_ERR_NONE;
}

rose_runtime_api_error rose_api_memory_memset(rose_runtime_base* r, uint32_t dest_addr, uint8_t val, uint32_t len) {
    if (dest_addr >= r->mem_size || (dest_addr + len) >= r->mem_size) {
        return ROSE_API_ERR_OUT_OF_BOUNDS;
    }
    memset(r->mem + dest_addr, val, len);
    return ROSE_API_ERR_NONE;
}
