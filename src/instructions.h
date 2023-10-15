#pragma once

#include "types.h"

// Each instruction has its own opcode for each addressing mode it supports. This enum contains hardcoded
// values of all the opcodes supported by the emulator. Name of the opcode should contain instruction name,
// an underscore and an addressing mode if neccessary.
enum class OpCode : u8 {
    // LDA - load accumulator
    LDA_imm = 0xA9,
    LDA_z = 0xA5,
    LDA_zx = 0xB5,
    LDA_abs = 0xAD,
    LDA_absx = 0xBD,
    LDA_absy = 0xB9,
    LDA_ix = 0xA1,
    LDA_iy = 0xB1,

    // STA - store accumulator
    STA_z = 0x85,
    STA_zx = 0x95,
    STA_abs = 0x8D,
    STA_absx = 0x9D,
    STA_absy = 0x99,
    STA_ix = 0x81,
    STA_iy = 0x91,

    // TAX, TAY, TXA, TYA - register transfers
    TAX = 0xAA,
    TAY = 0xA8,
    TXA = 0xBA,
    TYA = 0x8A,

    // CMP - compare
    CMP_imm = 0xC9,
    CMP_z = 0xC5,
    CMP_zx = 0xD5,
    CMP_abs = 0xCD,
    CMP_absx = 0xDD,
    CMP_absy = 0xD9,
    CMP_ix = 0xC1,
    CMP_iy = 0xD1,

    // Special values
    _MAX_VALUE = 0xFF,
};
