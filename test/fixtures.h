#pragma once

#include "src/processor.h"

#include <gtest/gtest.h>

struct WhiteboxProcessor : Processor {
    using Processor::counters;
    using Processor::memory;
    using Processor::regs;
};

class FlagsTracker {
public:
    void setUp(WhiteboxProcessor &processor) {
        actualFlags = &processor.regs.flags;

        actualFlags->c = 0;
        actualFlags->z = 0;
        actualFlags->i = 0;
        actualFlags->d = 0;
        actualFlags->b = 0;
        actualFlags->o = 0;
        actualFlags->n = 0;
        actualFlags->r = 0;
        expectedFlags = *actualFlags;
    }

    void tearDown() {
        EXPECT_EQ(expectedFlags.c, actualFlags->c);
        EXPECT_EQ(expectedFlags.z, actualFlags->z);
        EXPECT_EQ(expectedFlags.i, actualFlags->i);
        EXPECT_EQ(expectedFlags.d, actualFlags->d);
        EXPECT_EQ(expectedFlags.b, actualFlags->b);
        EXPECT_EQ(expectedFlags.o, actualFlags->o);
        EXPECT_EQ(expectedFlags.r, actualFlags->r);
    }

#define EXPECT_FUNC(flagName, fieldName)                   \
    void expect##flagName##Flag(bool before, bool after) { \
        actualFlags->fieldName = before;                   \
        expectedFlags.fieldName = after;                   \
    }                                                      \
                                                           \
    void expect##flagName##Flag(bool after) {              \
        actualFlags->fieldName = !after;                   \
        expectedFlags.fieldName = after;                   \
    }

    EXPECT_FUNC(Carry, c)
    EXPECT_FUNC(Zero, z)
    EXPECT_FUNC(Interrupt, i)
    EXPECT_FUNC(Decimal, d)
    EXPECT_FUNC(Break, b)
    EXPECT_FUNC(Overflow, o)
    EXPECT_FUNC(Negative, n)
    EXPECT_FUNC(Reserved, r)
#undef EXPECT_FUNC

private:
    StatusFlags *actualFlags = nullptr;
    StatusFlags expectedFlags{};
};

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
