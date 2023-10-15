#pragma once

#include "counters.h"
#include "instructions.h"
#include "registers.h"

constexpr u32 memorySize = 64 * 1024;

class Processor {

public:
    Processor() = default;

    void executeInstructions(u32 maxInstructionCount);

protected:
    // Helper functions to fetch from instruction stream. They increase cycle counter and program counter.
    u8 fetchInstructionByte();
    u16 fetchInstructionTwoBytes();

    // Helper functions to read from memory. They increase cycle counter.
    u8 readByteFromMemory(u16 address);
    u16 readTwoBytesFromMemory(u16 address);

    // Helper functions to read values for different addressing mode
    u8 getValueImmediate();
    u8 getValueZeroPage();
    u8 getValueZeroPageX();
    u8 getValueAbsolute();
    u8 getValueAbsoluteX();
    u8 getValueAbsoluteY();
    u8 getValueIndexedIndirectX();
    u8 getValueIndirectIndexedY();

    // Helper functions on mathematical operations performed internally by the processor. They may
    // increase cycle counter and handle special behaviours, like value wraparounds.
    u16 sumAddresses(u16 base, u16 offset);
    u16 sumAddressesZeroPage(u8 base, u8 offset);

    // Helper functions for flags register
    void updateArithmeticFlags(u8 value);

    void executeInstruction(OpCode opCode);

    Counters counters = {};
    Registers regs = {};
    u8 memory[memorySize] = {};
};
