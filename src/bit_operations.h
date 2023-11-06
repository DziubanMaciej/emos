#pragma once

#include "src/types.h"
#include "src/error.h"

template<u8 bitIndex>
constexpr void setBit(u8 &value, bool bit) {
    static_assert(bitIndex < 8);
    value |= bit << bitIndex;
    value &= ~((!bit) << bitIndex);
}

template<u8 bitIndex>
constexpr bool isBitSet(u8 value) {
    static_assert(bitIndex < 8);
    return static_cast<bool>(value & (1 << bitIndex));
}

constexpr bool isSignBitSet(u8 value) {
    return isBitSet<7>(value);
}

constexpr bool isZeroBitSet(u8 value) {
    return isBitSet<0>(value);
}

