#include "registers.h"

#include "src/error.h"

// TODO move into common file. Tthere are some bit manipulations in Processor class as well.
void setBit(u8 &value, u8 bitIndex, bool bit) {
    FATAL_ERROR_IF(bitIndex >= 8, "bit index for u8 must be within <0,7>")
    value |= bit << bitIndex;
    value &= ~((!bit) << bitIndex);
}

bool isBitSet(u8 value, u8 bitIndex) {
    FATAL_ERROR_IF(bitIndex >= 8, "bit index for u8 must be within <0,7>")
    return static_cast<bool>(value & (1 << bitIndex));
}

u8 StatusFlags::toU8() const {
    u8 result = {};
    setBit(result, 7, c);
    setBit(result, 6, z);
    setBit(result, 5, i);
    setBit(result, 4, d);
    setBit(result, 3, b);
    setBit(result, 2, o);
    setBit(result, 1, n);
    setBit(result, 0, r);
    return result;
}

StatusFlags StatusFlags::fromU8(u8 value) {
    StatusFlags result;
    result.c = isBitSet(value, 7);
    result.z = isBitSet(value, 6);
    result.i = isBitSet(value, 5);
    result.d = isBitSet(value, 4);
    result.b = isBitSet(value, 3);
    result.o = isBitSet(value, 2);
    result.n = isBitSet(value, 1);
    result.r = isBitSet(value, 0);
    return result;
}
