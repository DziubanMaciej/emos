#pragma once

#include "counters.h"
#include "instructions.h"
#include "registers.h"

constexpr u32 memorySize = 64 * 1024;

enum class AddressingMode {
    Accumulator,
    Implied,
    Immediate,
    ZeroPage,
    ZeroPageX,
    ZeroPageY,
    Absolute,
    AbsoluteX,
    AbsoluteY,
    IndexedIndirectX,
    IndirectIndexedY,
};

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
    u16 getAddress(AddressingMode mode, bool isReadOnly);
    u8 readValue(AddressingMode mode, bool isReadOnly, u16 *outAddress = nullptr);
    void writeValue(AddressingMode mode, u8 value, u16 *address = nullptr);

    // Helper functions on mathematical operations performed internally by the processor. They may
    // increase cycle counter and handle special behaviours, like value wraparounds.
    u16 sumAddresses(u16 base, u16 offset, bool isReadOnly);
    u16 sumAddressesZeroPage(u8 base, u8 offset);
    template <typename RegT>
    void registerTransfer(RegT &dst, const RegT &src);
    void aluOperation();
    bool isSignBitSet(u8 value);
    bool isZeroBitSet(u8 value);
    void setBit(u8 &value, u8 bitIndex, bool bit);

    // Helper functions for status flags.
    void updateArithmeticFlags(u8 value);
    void updateFlagsAfterComparison(u8 registerValue, u8 inputValue);
    u16 updateOverflowForSumWithCarry(u8 inputValue1, u8 inputValue2);
    void updateCarryFlagIfOverflow(u16 value);

    // Helper functions for stack operations
    void pushToStack(u8 value);
    u8 popFromStack();

    // Functions for executing instructions.
    void executeLda(AddressingMode mode);
    void executeLdx(AddressingMode mode);
    void executeLdy(AddressingMode mode);
    void executeInc(AddressingMode mode);
    void executeDec(AddressingMode mode);
    void executeInx(AddressingMode mode);
    void executeIny(AddressingMode mode);
    void executeDex(AddressingMode mode);
    void executeDey(AddressingMode mode);
    void executeAsl(AddressingMode mode);
    void executeLsr(AddressingMode mode);
    void executeRol(AddressingMode mode);
    void executeRor(AddressingMode mode);
    void executeCmp(AddressingMode mode);
    void executeCpx(AddressingMode mode);
    void executeCpy(AddressingMode mode);
    void executeTax(AddressingMode mode);
    void executeTay(AddressingMode mode);
    void executeTxa(AddressingMode mode);
    void executeTya(AddressingMode mode);
    void executeTsx(AddressingMode mode);
    void executeTxs(AddressingMode mode);
    void executePha(AddressingMode mode);
    void executePhp(AddressingMode mode);
    void executePla(AddressingMode mode);
    void executePlp(AddressingMode mode);
    void executeAdc(AddressingMode mode);
    void executeAnd(AddressingMode mode);
    void executeOra(AddressingMode mode);

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
