#pragma once

#include "src/error.h"
#include "src/types.h"

template <u8 bitIndex>
constexpr void setBit(u8 &value, bool bit) {
    static_assert(bitIndex < 8);
    value |= bit << bitIndex;
    value &= ~((!bit) << bitIndex);
}

template <typename ValueT>
constexpr bool isBitSet(ValueT value, u8 bitIndex) {
    return static_cast<bool>(value & (1 << bitIndex));
}

template <u8 bitIndex>
constexpr bool isBitSet(u8 value) {
    static_assert(bitIndex < 8);
    return isBitSet(value, bitIndex);
}

template <u8 bitIndex>
constexpr bool isBitSet(u16 value) {
    static_assert(bitIndex < 15);
    return isBitSet(value, bitIndex);
}

constexpr bool isSignBitSet(u8 value) {
    return isBitSet<7>(value);
}

constexpr bool isZeroBitSet(u8 value) {
    return isBitSet<0>(value);
}

constexpr u8 lo(u16 value) {
    return value & 0xFF;
}

constexpr u8 hi(u16 value) {
    return (value & 0xFF00) >> 8;
}

constexpr u16 constructU16(u8 hi, u8 lo) {
    return (u16(hi) << 8) | lo;
}

constexpr u8 loNibble(u8 value) {
    return value & 0x0F;
}

constexpr u8 hiNibble(u8 value) {
    return (value & 0xF0) >> 4;
}

constexpr u8 constructU8(u8 hiNibble, u8 loNibble) {
    return (u8(hiNibble) << 4) | loNibble;
}
