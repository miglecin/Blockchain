#include "ai.h"

// Įkvėpta iš MurmurHash3 fmix32
uint32_t mix32(uint32_t x) {
    x ^= x >> 16;
    x *= 0x85ebca6b;   // gera 32-bit maišymo konstanta
    x ^= x >> 13;
    x *= 0xc2b2ae35;   // dar viena gera konstanta
    x ^= x >> 16;
    return x;
}
