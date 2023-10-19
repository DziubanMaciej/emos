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

    // Helper functions to access the memory. They increase cycle counter.
    u8 readByteFromMemory(u16 address);
    u16 readTwoBytesFromMemory(u16 address);
    void writeByteToMemory(u16 address, u8 byte);
    void writeTwoBytesToMemory(u16 address, u16 bytes);

    // Helper functions to resolve addresses for different addressing modes.
    enum class AddressingMode {
        Implied,
        Immediate,
        ZeroPage,
        ZeroPageX,
        Absolute,
        AbsoluteX,
        AbsoluteY,
        IndexedIndirectX,
        IndirectIndexedY,
    };
    u16 getAddress(AddressingMode mode, bool isReadOnly);
    u8 readValue(AddressingMode mode, bool isReadOnly);
    void writeValue(AddressingMode mode, u8 value);

    // Helper functions on mathematical operations performed internally by the processor. They may
    // increase cycle counter and handle special behaviours, like value wraparounds.
    u16 sumAddresses(u16 base, u16 offset, bool isReadOnly);
    u16 sumAddressesZeroPage(u8 base, u8 offset);
    void registerTransfer(u8 &dst, const u8 &src);
    void aluOperation();

    // Helper functions for status flags.
    void updateArithmeticFlags(u8 value);
    void updateFlagsAfterComparison(u8 registerValue, u8 inputValue);

    // Functions for executing instructions.
    void executeLda(AddressingMode mode);
    void executeInc(AddressingMode mode);
    void executeDec(AddressingMode mode);
    void executeCmp(AddressingMode mode);
    void executeCpx(AddressingMode mode);
    void executeCpy(AddressingMode mode);
    void executeTax(AddressingMode mode);
    void executeTay(AddressingMode mode);
    void executeTxa(AddressingMode mode);
    void executeTya(AddressingMode mode);

    // State of the CPU.
    Counters counters = {};
    Registers regs = {};
    u8 memory[memorySize] = {};

    // Metadata for instruction executing.
    struct InstructionData {
        using ExecFunction = void (Processor::*)(AddressingMode);
        AddressingMode addressingMode;
        ExecFunction exec = nullptr;
    };
    InstructionData instructionData[static_cast<u8>(OpCode::_MAX_VALUE)];
    void setInstructionData(OpCode opCode, AddressingMode addressingMode, InstructionData::ExecFunction exec) {
        u8 index = static_cast<u8>(opCode);
        instructionData[index].addressingMode = addressingMode;
        instructionData[index].exec = exec;
    }
};
