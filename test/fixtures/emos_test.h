#pragma once

#include "test/fixtures/flags_tracker.h"
#include "test/fixtures/whitebox.h"

#include <gtest/gtest.h>

struct EmosTest : ::testing::Test {
    void SetUp() override {
        // Set PC to an arbitrary value - we have to start somewhere.
        startAddress = 0xFF00;
        processor.regs.pc = startAddress;

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
        flags.tearDown();
    }

    virtual void initializeProcessor(OpCode opcode, std::optional<u8> value, std::optional<u8> loadToReg) = 0;

    void initializeForZeroPageX(OpCode opCode, u8 value) {
        processor.memory[startAddress + 0] = static_cast<u8>(opCode);
        processor.memory[startAddress + 1] = 0x05;
        processor.regs.x = 0x3;
        dummyAddressUsedForWriteValueToMemory = processor.memory[startAddress + 1] + processor.regs.x; // 0x8
        processor.memory[dummyAddressUsedForWriteValueToMemory] = value;
    }
    void initializeForZeroPageY(OpCode opCode, u8 value) {
        processor.memory[startAddress + 0] = static_cast<u8>(opCode);
        processor.memory[startAddress + 1] = 0x84;
        processor.regs.y = 0x5;
        processor.memory[0x89] = value;
    }

    void initializeForZeroPage(OpCode opCode, u8 value) {
        processor.memory[startAddress + 0] = static_cast<u8>(opCode);
        processor.memory[startAddress + 1] = 0x05;
        dummyAddressUsedForWriteValueToMemory = processor.memory[startAddress + 1];
        processor.memory[dummyAddressUsedForWriteValueToMemory] = value;
    }
    void initializeForImmediate(OpCode opCode, u8 value) {
        processor.memory[startAddress + 0] = static_cast<u8>(opCode);
        processor.memory[startAddress + 1] = value;
    }
    void initializeForAbsolute(OpCode opCode, u8 value) {
        processor.memory[startAddress + 0] = static_cast<u8>(opCode);
        processor.memory[startAddress + 1] = 0x34;
        processor.memory[startAddress + 2] = 0x12;
        dummyAddressUsedForWriteValueToMemory = (processor.memory[startAddress + 2] << 0x8) + processor.memory[startAddress + 1]; // 0x1234
        processor.memory[dummyAddressUsedForWriteValueToMemory] = value;
    }

    void initializeForAbsoluteX(OpCode opCode, u8 value) {
        processor.memory[startAddress + 0] = static_cast<u8>(opCode);
        processor.memory[startAddress + 1] = 0x34;
        processor.memory[startAddress + 2] = 0x12;
        processor.regs.x = 0x3;
        dummyAddressUsedForWriteValueToMemory = (processor.memory[startAddress + 2] << 0x8) + processor.memory[startAddress + 1] + processor.regs.x; // 0x1234
        processor.memory[dummyAddressUsedForWriteValueToMemory] = value;
    }

    void initializeForAbsoluteY(OpCode opCode, u8 value) {
        processor.memory[startAddress + 0] = static_cast<u8>(opCode);
        processor.memory[startAddress + 1] = 0x34;
        processor.memory[startAddress + 2] = 0x12;
        processor.regs.y = 0x3;
        dummyAddressUsedForWriteValueToMemory = (processor.memory[startAddress + 2] << 0x8) + processor.memory[startAddress + 1] + processor.regs.y; // 0x1234
        processor.memory[dummyAddressUsedForWriteValueToMemory] = value;
    }

    void initializeForIndirectX(OpCode opCode, u8 value) {
        processor.memory[startAddress + 0] = static_cast<u8>(opCode);
        processor.memory[startAddress + 1] = 0x54;
        processor.regs.x = 0x5;
        u16 tempAddress = processor.memory[startAddress + 1] + processor.regs.x; // 0x59
        processor.memory[tempAddress] = 0x34;
        processor.memory[tempAddress + 1] = 0x12;
        dummyAddressUsedForWriteValueToMemory = (processor.memory[tempAddress + 1] << 0x8) + processor.memory[tempAddress]; // 0x1234
        processor.memory[dummyAddressUsedForWriteValueToMemory] = value;
    }
    void initializeForIndirectY(OpCode opCode, u8 value) {
        processor.memory[startAddress + 0] = static_cast<u8>(opCode);
        processor.memory[startAddress + 1] = 0x54;
        u16 tempAddress = processor.memory[startAddress + 1];
        processor.memory[tempAddress] = 0x30;
        processor.memory[tempAddress + 1] = 0x12;
        processor.regs.y = 0x4;
        dummyAddressUsedForWriteValueToMemory = (processor.memory[tempAddress + 1] << 0x8) + processor.memory[tempAddress] + processor.regs.y; // 0x1234
        processor.memory[dummyAddressUsedForWriteValueToMemory] = value;
    }

    u16 dummyAddressUsedForWriteValueToMemory = {};
    u16 startAddress = {};
    WhiteboxProcessor processor = {};
    FlagsTracker flags{};
    u32 expectedBytesProcessed = 0u;
    u32 expectedCyclesProcessed = 0u;
};
