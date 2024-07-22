#pragma once

#include "src/counters.h"
#include "src/hang_detector.h"
#include "src/instruction_tracer.h"
#include "src/instructions.h"
#include "src/registers.h"

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
    Indirect,
    Relative,
};

class Processor {

public:
    Processor();

    void loadMemory(u32 start, u32 length, const u8 *data);
    void loadProgramCounter(u16 newPc);
    void activateHangDetector();
    void activateInstructionTracing();
    bool executeInstructions(u32 maxInstructionCount);

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

    // Helper functions for expressing additional cycles used by some instructions
    void aluOperation();
    void idleCycle();
    void hiddenLatencyCycle();

    // Helper functions for status flags.
    void updateArithmeticFlags(u8 value);
    void updateFlagsAfterComparison(u8 registerValue, u8 inputValue);
    u16 updateOverflowForSumWithCarry(u8 addend);
    void updateCarryFlagIfOverflow(u16 value, bool OverflowValue);

    void sumWithCarry(u8 addend, bool OverflowValue);

    // Helper functions for stack operations
    void pushToStack(u8 value);
    void pushToStack16(u16 value);
    u8 popFromStack();
    u16 popFromStack16();

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
    void executeEor(AddressingMode mode);
    void executeBit(AddressingMode mode);
    void executeOra(AddressingMode mode);
    void executeSec(AddressingMode mode);
    void executeSed(AddressingMode mode);
    void executeSei(AddressingMode mode);
    void executeClc(AddressingMode mode);
    void executeCld(AddressingMode mode);
    void executeCli(AddressingMode mode);
    void executeClv(AddressingMode mode);
    void executeSta(AddressingMode mode);
    void executeStx(AddressingMode mode);
    void executeSty(AddressingMode mode);
    void executeSbc(AddressingMode mode);
    void executeJmp(AddressingMode mode);
    void executeJsr(AddressingMode mode);
    void executeRts(AddressingMode mode);
    void executeBranch(AddressingMode mode, bool take);
    void executeBcc(AddressingMode mode);
    void executeBcs(AddressingMode mode);
    void executeBeq(AddressingMode mode);
    void executeBmi(AddressingMode mode);
    void executeBne(AddressingMode mode);
    void executeBpl(AddressingMode mode);
    void executeBvc(AddressingMode mode);
    void executeBvs(AddressingMode mode);
    void executeNop(AddressingMode mode);
    void executeBrk(AddressingMode mode);
    void executeRti(AddressingMode mode);

    struct DebugFeatures {
        bool hangDetectionActive = false;
        HangDetector hangDetector = {};

        bool instructionTracingActive = false;
        InstructionTracer instructionTracer = {};
    } debugFeatures;

    // State of the CPU.
    Counters counters = {};
    Registers regs = {};
    u8 memory[memorySize] = {};

    // Metadata for instruction executing.
    struct InstructionData {
        using ExecFunction = void (Processor::*)(AddressingMode);
        const char *mnemonic = {};
        AddressingMode addressingMode = {};
        ExecFunction exec = nullptr;
    };
    InstructionData instructionData[static_cast<u8>(OpCode::_MAX_VALUE)];
    void setInstructionData(const char *mnemonic, OpCode opCode, AddressingMode addressingMode, InstructionData::ExecFunction exec) {
        u8 index = static_cast<u8>(opCode);
        instructionData[index].mnemonic = mnemonic;
        instructionData[index].addressingMode = addressingMode;
        instructionData[index].exec = exec;
    }
};
