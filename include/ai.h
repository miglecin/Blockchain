#ifndef AI_H
#define AI_H

#include <cstdint>

// Bitų rotacija į kairę
inline uint32_t rotl32(uint32_t x, int r) {
    return (x << r) | (x >> (32 - r));
}

// Patobulintas 32-bit maišymo (mix) finalizeris
uint32_t mix32(uint32_t x);

#endif // AI_H
