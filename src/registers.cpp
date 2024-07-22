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

std::string StatusFlags::toString() const {
    const u8 value = toU8();
    char buffer[9];
    buffer[0] = isBitSet<0>(value) ? '1' : '0';
    buffer[1] = isBitSet<1>(value) ? '1' : '0';
    buffer[2] = isBitSet<2>(value) ? '1' : '0';
    buffer[3] = isBitSet<3>(value) ? '1' : '0';
    buffer[4] = isBitSet<4>(value) ? '1' : '0';
    buffer[5] = isBitSet<5>(value) ? '1' : '0';
    buffer[6] = isBitSet<6>(value) ? '1' : '0';
    buffer[7] = isBitSet<7>(value) ? '1' : '0';
    buffer[8] = '\0';
    return std::string{buffer};
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
