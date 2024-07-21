#include "bit_operations.h"
#include "registers.h"

#include "src/error.h"

u8 StatusFlags::toU8() const {
    u8 result = {};
    setBit<7>(result, n);
    setBit<6>(result, o);
    setBit<5>(result, b);
    setBit<4>(result, r);
    setBit<3>(result, d);
    setBit<2>(result, i);
    setBit<1>(result, z);
    setBit<0>(result, c);
    return result;
}

StatusFlags StatusFlags::fromU8(u8 value) {
    StatusFlags result = {};
    result.n = isBitSet<7>(value);
    result.o = isBitSet<6>(value);
    result.b = isBitSet<5>(value);
    result.r = isBitSet<4>(value);
    result.d = isBitSet<3>(value);
    result.i = isBitSet<2>(value);
    result.z = isBitSet<1>(value);
    result.c = isBitSet<0>(value);
    return result;
}
