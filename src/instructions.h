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

    // LDX - Load X register
    LDX_imm = 0xA2,
    LDX_z = 0xA6,
    LDX_zy = 0xB6,
    LDX_abs = 0xAE,
    LDX_absy = 0xBE,

    // LDY - Load Y register
    LDY_imm = 0xA0,
    LDY_z = 0xA4,
    LDY_zx = 0xB4,
    LDY_abs = 0xAC,
    LDY_absx = 0xBC,

    // STA - store accumulator
    STA_z = 0x85,
    STA_zx = 0x95,
    STA_abs = 0x8D,
    STA_absx = 0x9D,
    STA_absy = 0x99,
    STA_ix = 0x81,
    STA_iy = 0x91,

    // INC - increment memory
    INC_z = 0xE6,
    INC_zx = 0xF6,
    INC_abs = 0xEE,
    INC_absx = 0xFE,

    // INC, INY, DEX, DEY - increment/decrement index registers
    INX = 0xE8,
    INY = 0xC8,
    DEX = 0xCA,
    DEY = 0x88,

    // DEC - decrement memory
    DEC_z = 0xC6,
    DEC_zx = 0xD6,
    DEC_abs = 0xCE,
    DEC_absx = 0xDE,

    // ASL - arithmetic shift left
    ASL_acc = 0x0A,
    ASL_z = 0x06,
    ASL_zx = 0x16,
    ASL_abs = 0x0E,
    ASL_absx = 0x1E,

    // LSR - logical shift right
    LSR_acc = 0x4A,
    LSR_z = 0x46,
    LSR_zx = 0x56,
    LSR_abs = 0x4E,
    LSR_absx = 0x5E,

    // ROL - Rotate Left
    ROL_acc = 0x2A,
    ROL_z = 0x26,
    ROL_zx = 0x36,
    ROL_abs = 0x2E,
    ROL_absx = 0x3E,

    // ROR - Rotate Right
    ROR_acc = 0x6A,
    ROR_z = 0x66,
    ROR_zx = 0x76,
    ROR_abs = 0x6E,
    ROR_absx = 0x7E,

    // TAX, TAY, TXA, TYA, TSX, TXS - register transfers
    TAX = 0xAA,
    TAY = 0xA8,
    TXA = 0x8A,
    TYA = 0x98,
    TSX = 0xBA,
    TXS = 0x9A,

    // PHA, PHP, PLA, PLP - stack operations
    PHA = 0x48,
    PHP = 0x08,
    PLA = 0x68,
    PLP = 0x28,

    // CMP - compare
    CMP_imm = 0xC9,
    CMP_z = 0xC5,
    CMP_zx = 0xD5,
    CMP_abs = 0xCD,
    CMP_absx = 0xDD,
    CMP_absy = 0xD9,
    CMP_ix = 0xC1,
    CMP_iy = 0xD1,

    // CPX - compare x register
    CPX_imm = 0xE0,
    CPX_z = 0xE4,
    CPX_abs = 0xEC,

    // CPY - compare y register
    CPY_imm = 0xC0,
    CPY_z = 0xC4,
    CPY_abs = 0xCC,

    // ADC - add with carry
    ADC_imm = 0x69,
    ADC_z = 0x65,
    ADC_zx = 0x75,
    ADC_abs = 0x6D,
    ADC_absx = 0x7D,
    ADC_absy = 0x79,
    ADC_ix = 0x61,
    ADC_iy = 0x71,

    // AND - logical and
    AND_imm = 0x29,
    AND_z = 0x25,
    AND_zx = 0x35,
    AND_abs = 0x2D,
    AND_absx = 0x3D,
    AND_absy = 0x39,
    AND_ix = 0x21,
    AND_iy = 0x31,

    // EOR - exclusive or
    EOR_imm = 0x49,
    EOR_z = 0x45,
    EOR_zx = 0x55,
    EOR_abs = 0x4D,
    EOR_absx = 0x5D,
    EOR_absy = 0x59,
    EOR_ix = 0x41,
    EOR_iy = 0x51,

    // BIT - bit test
    BIT_z = 0x24,
    BIT_abs = 0x2c,

    // ORA - logical or
    ORA_imm = 0x09,
    ORA_z = 0x05,
    ORA_zx = 0x15,
    ORA_abs = 0x0D,
    ORA_absx = 0x1D,
    ORA_absy = 0x19,
    ORA_ix = 0x01,
    ORA_iy = 0x11,

    // SEC, SED, SEI - set flags
    SEC = 0x38,
    SED = 0xF8,
    SEI = 0x78,

    // CLC, CLD, CLI, CLV - clear flags
    CLC = 0x18,
    CLD = 0xD8,
    CLI = 0x58,
    CLV = 0xB8,

    // STX, STY - store register into memory
    STX_z = 0x86,
    STX_zy = 0x96,
    STX_abs = 0x8E,
    STY_z = 0x84,
    STY_zx = 0x94,
    STY_abs = 0x8C,

    // SBC - Substrack with carry
    SBC_imm = 0xE9,
    SBC_z = 0xE5,
    SBC_zx = 0xF5,
    SBC_abs = 0xED,
    SBC_absx = 0xFD,
    SBC_absy = 0xF9,
    SBC_ix = 0xE1,
    SBC_iy = 0xF1,

    // JMP, JSR, RTS - jumps and calls
    JMP_abs = 0x4C,
    JMP_i = 0x6C,
    JSR = 0x20,
    RTS = 0x60,

    // Branches
    BCC = 0x90,
    BCS = 0xB0,
    BEQ = 0xF0,
    BMI = 0x30,
    BNE = 0xD0,
    BPL = 0x10,
    BVC = 0x50,
    BVS = 0x70,

    // NOP
    NOP = 0xEA,

    // Interrupts
    BRK = 0x00,
    RTI = 0x40,

    // Special values
    _INVALID = 0xFC,
    _MAX_VALUE = 0xFF,
};
