#pragma once

#include "src/processor.h"

#include <gtest/gtest.h>

struct WhiteboxProcessor : Processor {
    using Processor::counters;
    using Processor::memory;
    using Processor::regs;
};

struct EmosTest : ::testing::Test {
    void SetUp() {
        // Set PC to an arbitrary value - we have to start somewhere.
        startAddress = 0xFF00;
        processor.regs.pc = startAddress;

        // Set registers to bogus values, to make sure they are not used accidentally.
        processor.regs.a = 0x13;
        processor.regs.x = 0x23;
        processor.regs.y = 0x33;
        processor.regs.sp = 0x43;
        processor.regs.flags.c = 0;
        processor.regs.flags.z = 0;
        processor.regs.flags.i = 0;
        processor.regs.flags.d = 0;
        processor.regs.flags.b = 0;
        processor.regs.flags.o = 0;
        processor.regs.flags.n = 0;
        processor.regs.flags.r = 0;
    }
    void initializeForZeroPageX(OpCode opCode, u8 value) {
        processor.memory[startAddress + 0] = static_cast<u8>(opCode);
        processor.memory[startAddress + 1] = 0x05;
        processor.regs.x = 0x3;
        processor.memory[0x08] = value;
    }
    void initializeForZeroPage(OpCode opCode, u8 value) {
        processor.memory[startAddress + 0] = static_cast<u8>(opCode);
        processor.memory[startAddress + 1] = 0x05;
        processor.memory[0x05] = value;
    }
    void initializeForImmediate(OpCode opCode, u8 value) {
        processor.memory[startAddress + 0] = static_cast<u8>(opCode);
        processor.memory[startAddress + 1] = value;
    }
    void initializeForAbsolute(OpCode opCode, u8 value) {
        processor.memory[startAddress + 0] = static_cast<u8>(opCode);
        processor.memory[startAddress + 1] = 0x34;
        processor.memory[startAddress + 2] = 0x12;
        processor.memory[0x1234] = value;
    }

    void initializeForAbsoluteX(OpCode opCode, u8 value) {
        processor.memory[startAddress + 0] = static_cast<u8>(opCode);
        processor.memory[startAddress + 1] = 0x34;
        processor.memory[startAddress + 2] = 0x12;
        processor.regs.x = 0x3;
        processor.memory[0x1237] = value;
    }

    void initializeForAbsoluteY(OpCode opCode, u8 value) {
        processor.memory[startAddress + 0] = static_cast<u8>(opCode);
        processor.memory[startAddress + 1] = 0x34;
        processor.memory[startAddress + 2] = 0x12;
        processor.regs.y = 0x3;
        processor.memory[0x1237] = value;
    }

    void initializeForIndirectX(OpCode opCode, u8 value) {
        processor.memory[startAddress + 0] = static_cast<u8>(opCode);
        processor.memory[startAddress + 1] = 0x54;
        processor.regs.x = 0x5;
        processor.memory[0x59] = 0x34;
        processor.memory[0x5A] = 0x12;
        processor.memory[0x1234] = value;
    }
    void initializeForIndirectY(OpCode opCode, u8 value) {
        processor.memory[startAddress + 0] = static_cast<u8>(opCode);
        processor.memory[startAddress + 1] = 0x54;
        processor.memory[0x54] = 0x30;
        processor.memory[0x55] = 0x12;
        processor.regs.y = 0x4;
        processor.memory[0x1234] = value;
    }

    u16 startAddress = {};
    WhiteboxProcessor processor = {};
};
