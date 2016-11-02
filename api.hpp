static const unsigned long CART_SIZE = (256 * 256) + (256 * 256) + (256 * 3);
static const unsigned long MEM_SIZE = CART_SIZE + (256 * 256);
unsigned char rMEM[MEM_SIZE];
unsigned char rCART[CART_SIZE];

unsigned char r_peek(unsigned long addr);
void r_poke(unsigned long addr, unsigned char val);
void r_memcpy(unsigned long dest_addr, unsigned long source_addr,
              unsigned long len);
