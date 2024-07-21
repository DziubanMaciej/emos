#pragma once

#include "test/fixtures/flags_tracker.h"
#include "test/fixtures/referenced_value.h"
#include "test/fixtures/whitebox.h"

#include <gtest/gtest.h>

struct EmosTest : ::testing::Test {
    void SetUp() override {
        // Set PC to an arbitrary value - we have to start somewhere.
        setStartAddress(0xFF00);

        // Set registers to bogus values, to make sure they are not used accidentally.
        processor.regs.a = 0x13;
        processor.regs.x = 0x23;
        processor.regs.y = 0x33;
        processor.regs.sp = 0x43;
        flags.setUp(processor);
    }

    void TearDown() override {
        EXPECT_EQ(expectedBytesProcessed, processor.counters.bytesProcessed);
        EXPECT_EQ(expectedCyclesProcessed, processor.counters.cyclesProcessed);
        flags.expect();
    }

    void setStartAddress(u16 arg) {
        startAddress = arg;
        processor.regs.pc = arg;
    }

    void initializeForAccumulator(OpCode opCode, u8 value) {
        processor.memory[startAddress + 0] = static_cast<u8>(opCode);
        referencedValue.setRegisterA(processor);
        processor.regs.a = value;
    }
    void initializeForZeroPageX(OpCode opCode, u8 value) {
        processor.memory[startAddress + 0] = static_cast<u8>(opCode);
        processor.memory[startAddress + 1] = 0x05;
        processor.regs.x = 0x3;
        const u8 address = processor.memory[startAddress + 1] + processor.regs.x;
        processor.memory[address] = value;

        referencedValue.setMemory(processor, address);
    }
    void initializeForZeroPageY(OpCode opCode, u8 value) {
        processor.memory[startAddress + 0] = static_cast<u8>(opCode);
        processor.memory[startAddress + 1] = 0x84;
        processor.regs.y = 0x5;
        const u8 address = processor.memory[startAddress + 1] + processor.regs.y;
        processor.memory[address] = value;

        referencedValue.setMemory(processor, address);
    }

    void initializeForZeroPage(OpCode opCode, u8 value) {
        processor.memory[startAddress + 0] = static_cast<u8>(opCode);
        processor.memory[startAddress + 1] = 0x05;
        const u8 address = processor.memory[startAddress + 1];
        processor.memory[address] = value;

        referencedValue.setMemory(processor, address);
    }
    void initializeForImplied(OpCode opCode) {
        processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    }
    void initializeForImmediate(OpCode opCode, u8 value) {
        processor.memory[startAddress + 0] = static_cast<u8>(opCode);
        processor.memory[startAddress + 1] = value;
    }
    void initializeForAbsolute(OpCode opCode, u8 value) {
        processor.memory[startAddress + 0] = static_cast<u8>(opCode);
        processor.memory[startAddress + 1] = 0x34;
        processor.memory[startAddress + 2] = 0x12;
        const u16 address = (processor.memory[startAddress + 2] << 8) | (processor.memory[startAddress + 1]); // 0x1234
        processor.memory[address] = value;

        referencedValue.setMemory(processor, address);
    }

    void initializeForAbsoluteX(OpCode opCode, u8 value) {
        processor.memory[startAddress + 0] = static_cast<u8>(opCode);
        processor.memory[startAddress + 1] = 0x34;
        processor.memory[startAddress + 2] = 0x12;
        processor.regs.x = 0x3;
        const u16 address = (processor.memory[startAddress + 2] << 8) | (processor.memory[startAddress + 1] + processor.regs.x); // 0x1234
        processor.memory[address] = value;

        referencedValue.setMemory(processor, address);
    }

    void initializeForAbsoluteY(OpCode opCode, u8 value) {
        processor.memory[startAddress + 0] = static_cast<u8>(opCode);
        processor.memory[startAddress + 1] = 0x34;
        processor.memory[startAddress + 2] = 0x12;
        processor.regs.y = 0x3;
        const u16 address = (processor.memory[startAddress + 2] << 8) | (processor.memory[startAddress + 1] + processor.regs.y); // 0x1234
        processor.memory[address] = value;

        referencedValue.setMemory(processor, address);
    }

    void initializeForIndirectX(OpCode opCode, u8 value) {
        processor.memory[startAddress + 0] = static_cast<u8>(opCode);
        processor.memory[startAddress + 1] = 0x54;
        processor.regs.x = 0x5;
        u16 tempAddress = processor.memory[startAddress + 1] + processor.regs.x; // 0x59
        processor.memory[tempAddress] = 0x34;
        processor.memory[tempAddress + 1] = 0x12;
        const u16 address = (processor.memory[tempAddress + 1] << 8) | (processor.memory[tempAddress]); // 0x1234
        processor.memory[address] = value;

        referencedValue.setMemory(processor, address);
    }
    void initializeForIndirectY(OpCode opCode, u8 value) {
        processor.memory[startAddress + 0] = static_cast<u8>(opCode);
        processor.memory[startAddress + 1] = 0x54;
        u16 tempAddress = processor.memory[startAddress + 1];
        processor.memory[tempAddress] = 0x30;
        processor.memory[tempAddress + 1] = 0x12;
        processor.regs.y = 0x4;
        const u16 address = (processor.memory[tempAddress + 1] << 8) + (processor.memory[tempAddress] + processor.regs.y); // 0x1234
        processor.memory[address] = value;

        referencedValue.setMemory(processor, address);
    }

    u16 startAddress = {};
    WhiteboxProcessor processor = {};
    FlagsTracker flags{};
    ReferencedValue referencedValue = {};
    u32 expectedBytesProcessed = 0u;
    u32 expectedCyclesProcessed = 0u;
};
