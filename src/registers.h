#pragma once

#include "types.h"

struct StatusFlags {
    u8 c : 1; // carry flag
    u8 z : 1; // zero flag
    u8 i : 1; // interrupt disable
    u8 d : 1; // decimal mode
    u8 b : 1; // break command
    u8 o : 1; // overflow flag
    u8 n : 1; // negative flag
    u8 r : 1; // reserved

    u8 toU8() const;
    static StatusFlags fromU8(u8 value);
};

struct Registers {
    u8 a; // accumulator
    u8 x; // index register
    u8 y; // index register

    u16 pc; // program counter
    u8 sp;  // stack pointer

    StatusFlags flags;
};
