#pragma once

#include "counters.h"
#include "instructions.h"
#include "registers.h"

constexpr u32 memorySize = 64 * 1024;

class Processor {

public:
    Processor();

    void executeInstructions(u32 maxInstructionCount);

protected:
    // Helper functions to fetch from instruction stream. They increase cycle counter and program counter.
    u8 fetchInstructionByte();
    u16 fetchInstructionTwoBytes();

    // Helper functions to read from memory. They increase cycle counter.
    u8 readByteFromMemory(u16 address);
    u16 readTwoBytesFromMemory(u16 address);

    // Helper functions to read values for different addressing mode
    u8 getValueImplied();
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
    void registerTransfer(u8 &dst, const u8 &src);

    // Helper functions for flags register
    void updateArithmeticFlags(u8 value);
    void updateFlagsAfterComparison(u8 registerValue, u8 inputValue);

    // Functions for actually executing instructions
    void executeLda(u8 value);
    void executeCmp(u8 value);
    void executeTax(u8 value);
    void executeTay(u8 value);
    void executeTxa(u8 value);
    void executeTya(u8 value);

    // State of the CPU
    Counters counters = {};
    Registers regs = {};
    u8 memory[memorySize] = {};

    // Metadata for instruction executing
    struct InstructionData {
        using GetFunction = u8 (Processor::*)();
        using ExecFunction = void (Processor::*)(u8);
        GetFunction get = nullptr;
        ExecFunction exec = nullptr;
    };
    InstructionData instructionData[static_cast<u8>(OpCode::_MAX_VALUE)];
    void setInstructionData(OpCode opCode, InstructionData::GetFunction get, InstructionData::ExecFunction exec) {
        u8 index = static_cast<u8>(opCode);
        instructionData[index].get = get;
        instructionData[index].exec = exec;
    }
};
