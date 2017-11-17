#include "vm.h"

rose_api_error rose_vm::poke(uint32_t idx, uint8_t val) {
    if (idx >= ROSE_MEMORY_SIZE) {
        return ROSE_API_ERR_OUT_OF_BOUNDS;
    }
    mem[idx] = val;
    return ROSE_API_ERR_NONE;
}

rose_api_error rose_vm::peek(uint32_t idx, uint8_t* res) {
    if (idx >= ROSE_MEMORY_SIZE) {
        return ROSE_API_ERR_OUT_OF_BOUNDS;
    }
    *res = mem[idx];
    return ROSE_API_ERR_NONE;
}


rose_api_error rose_vm::rose_memcpy(uint32_t dest_addr, uint32_t source_addr, uint32_t len) {
    if (dest_addr >= ROSE_MEMORY_SIZE || source_addr >= ROSE_MEMORY_SIZE || (dest_addr + len) >= ROSE_MEMORY_SIZE || (source_addr + len) >= ROSE_MEMORY_SIZE) {
        return ROSE_API_ERR_OUT_OF_BOUNDS;
    }
    memcpy(mem + dest_addr, mem + source_addr, len);
    return ROSE_API_ERR_NONE;
}

rose_api_error rose_vm::rose_memset(uint32_t dest_addr, uint8_t val, uint32_t len) {
    if (dest_addr >= ROSE_MEMORY_SIZE || (dest_addr + len) >= ROSE_MEMORY_SIZE) {
        return ROSE_API_ERR_OUT_OF_BOUNDS;
    }
    memset(mem + dest_addr, val, len);
    return ROSE_API_ERR_NONE;
}

// rose_api_error rose_vm::cstore(uint32_t dest_addr, uint32_t source_addr, uint32_t len) {
//     if ((source_addr + len) > ROSE_MEMORY_SIZE) {
//         return ROSE_API_ERR_OUT_OF_BOUNDS;
//     }
//     rose_file* data_file = rose_fs_fetch_cart_data_file(target_cart);
//     if ((dest_addr + len) > data_file->buffer_len) {
//         return ROSE_API_ERR_OUT_OF_BOUNDS;
//     }
//     memcpy(data_file->buffer + dest_addr, mem + source_addr, len);
//     return ROSE_API_ERR_NONE;
// }

// rose_api_error rose_vm::reload(uint32_t dest_addr, uint32_t source_addr, uint32_t len) {
//     rose_file* data_file = rose_fs_fetch_cart_data_file(target_cart);
//     if ((source_addr + len) > data_file->buffer_len) {
//         return ROSE_API_ERR_OUT_OF_BOUNDS;
//     }
//     if ((dest_addr + len) > (ROSE_MEMORY_SIZE - ROSE_RUNTIME_RESERVED_MEMORY_SIZE)) {
//         return ROSE_API_ERR_OUT_OF_BOUNDS;
//     }

//     memcpy(mem + dest_addr, data_file->buffer + source_addr, len);
//     return ROSE_API_ERR_NONE;
// }
