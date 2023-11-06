#include "bit_operations.h"
#include "registers.h"

#include "src/error.h"

u8 StatusFlags::toU8() const {
    u8 result = {};
    setBit<7>(result, c);
    setBit<6>(result, z);
    setBit<5>(result, i);
    setBit<4>(result, d);
    setBit<3>(result, b);
    setBit<2>(result, o);
    setBit<1>(result, n);
    setBit<0>(result, r);
    return result;
}

StatusFlags StatusFlags::fromU8(u8 value) {
    StatusFlags result = {};
    result.c = isBitSet<7>(value);
    result.z = isBitSet<6>(value);
    result.i = isBitSet<5>(value);
    result.d = isBitSet<4>(value);
    result.b = isBitSet<3>(value);
    result.o = isBitSet<2>(value);
    result.n = isBitSet<1>(value);
    result.r = isBitSet<0>(value);
    return result;
}
