#include "cartridge.h"

Rose_Cartridge* rose_cartridge_create() {
    Rose_Cartridge* cart = (Rose_Cartridge*) malloc(sizeof(Rose_Cartridge));
    cart->code_size = 0;
    cart->code = (char *) malloc((cart->code_size + 1) * sizeof(char));
    cart->code[cart->code_size] = '\0';
    cart->data_size = ROSE_MEMORY_SIZE - ROSE_RUNTIME_RESERVED_MEMORY_SIZE;
    cart->data = (uint8_t *) malloc(cart->data_size);
    memset(cart->data, 0, cart->data_size);
    memcpy(cart->data + cart->data_size - ROSE_PALETTE_SIZE, rose_default_palette, sizeof(rose_default_palette));
    return cart;
}

void rose_cartridge_free(Rose_Cartridge* cart) {
    free(cart->code);
    free(cart->data);
    free(cart);
} 