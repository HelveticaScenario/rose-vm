#include "rose.h"

rose_api_error rose_api_memory_poke(rose_rt* r, uint32_t idx, uint8_t val) {
    if (idx >= r->mem->size()) {
        return ROSE_API_ERR_OUT_OF_BOUNDS;
    }
    (*r->mem)[idx] = val;
    return ROSE_API_ERR_NONE;
}

rose_api_error rose_api_memory_peek(rose_rt* r, uint32_t idx, uint8_t* res) {
    if (idx >= r->mem->size()) {
        return ROSE_API_ERR_OUT_OF_BOUNDS;
    }
    *res = (*r->mem)[idx];
    return ROSE_API_ERR_NONE;
}


rose_api_error rose_api_memory_memcpy(rose_rt* r, uint32_t dest_addr, uint32_t source_addr, uint32_t len) {
    if (dest_addr >= r->mem->size() || source_addr >= r->mem->size() || (dest_addr + len) >= r->mem->size() || (source_addr + len) >= r->mem->size()) {
        return ROSE_API_ERR_OUT_OF_BOUNDS;
    }
    memcpy(r->mem->data() + dest_addr, r->mem->data() + source_addr, len);
    return ROSE_API_ERR_NONE;
}

rose_api_error rose_api_memory_memset(rose_rt* r, uint32_t dest_addr, uint8_t val, uint32_t len) {
    if (dest_addr >= r->mem->size() || (dest_addr + len) >= r->mem->size()) {
        return ROSE_API_ERR_OUT_OF_BOUNDS;
    }
    memset(r->mem->data() + dest_addr, val, len);
    return ROSE_API_ERR_NONE;
}

rose_api_error rose_api_memory_cstore(rose_rt* r, uint32_t dest_addr, uint32_t source_addr, uint32_t len) {
    if ((source_addr + len) > ROSE_MEMORY_SIZE) {
        return ROSE_API_ERR_OUT_OF_BOUNDS;
    }
    rose_file* data_file = rose_fs_fetch_cart_data_file(r->target_cart);
    if ((dest_addr + len) > data_file->buffer_len) {
        return ROSE_API_ERR_OUT_OF_BOUNDS;
    }
    memcpy(data_file->buffer + dest_addr, r->mem->data() + source_addr, len);
    return ROSE_API_ERR_NONE;
}

rose_api_error rose_api_memory_reload(rose_rt* r, uint32_t dest_addr, uint32_t source_addr, uint32_t len) {
    rose_file* data_file = rose_fs_fetch_cart_data_file(r->target_cart);
    if ((source_addr + len) > data_file->buffer_len) {
        return ROSE_API_ERR_OUT_OF_BOUNDS;
    }
    if ((dest_addr + len) > (ROSE_MEMORY_SIZE - ROSE_RUNTIME_RESERVED_MEMORY_SIZE)) {
        return ROSE_API_ERR_OUT_OF_BOUNDS;
    }

    memcpy(r->mem->data() + dest_addr, data_file->buffer + source_addr, len);
    return ROSE_API_ERR_NONE;
}
