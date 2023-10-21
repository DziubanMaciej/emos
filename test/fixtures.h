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
        processor.regs.flags.c = flagsOnStart.c = 1;
        processor.regs.flags.z = flagsOnStart.z = 1;
        processor.regs.flags.i = flagsOnStart.i = 1;
        processor.regs.flags.d = flagsOnStart.d = 1;
        processor.regs.flags.b = flagsOnStart.b = 1;
        processor.regs.flags.o = flagsOnStart.o = 1;
        processor.regs.flags.n = flagsOnStart.n = 1;
        processor.regs.flags.r = flagsOnStart.r = 1;
    }
    void TearDown() {
        checkNotAffectedFlags();
    }
    virtual void checkNotAffectedFlags() {
        EXPECT_EQ(flagsOnStart.c, processor.regs.flags.c);
        EXPECT_EQ(flagsOnStart.i, processor.regs.flags.i);
        EXPECT_EQ(flagsOnStart.d, processor.regs.flags.d);
        EXPECT_EQ(flagsOnStart.b, processor.regs.flags.b);
        EXPECT_EQ(flagsOnStart.o, processor.regs.flags.o);
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
    StatusFlags flagsOnStart;
};
