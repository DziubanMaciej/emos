#include "fixtures.h"

#include "src/error.h"

struct InxInyDexDeyTest : testing::WithParamInterface<OpCode>, EmosTest {

    u8 &getReg(OpCode opCode) {
        switch (opCode) {
        case OpCode::INX:
        case OpCode::DEX:
            return processor.regs.x;
        case OpCode::INY:
        case OpCode::DEY:
            return processor.regs.y;
        default:
            FATAL_ERROR("Wrong OpCode");
        }
    }
    static std::string constructParamName(const testing::TestParamInfo<OpCode> &info) {
        OpCode opCode = info.param;
        switch (opCode) {
        case OpCode::INX:
            return "INX";
        case OpCode::INY:
            return "INY";
        case OpCode::DEX:
            return "DEX";
        case OpCode::DEY:
            return "DEY";
        default:
            FATAL_ERROR("Wrong OpCode");
        }
    }
};

using InxInyTest = InxInyDexDeyTest;

TEST_P(InxInyTest, givenIncrementOperationAndPositiveOutputThenProcessInstruction) {
    const OpCode opCode = GetParam();
    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    getReg(opCode) = 0x10;

    processor.executeInstructions(1);
    EXPECT_EQ(0x11, getReg(opCode));
    EXPECT_EQ(1, processor.counters.bytesProcessed);
    EXPECT_EQ(2, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

TEST_P(InxInyTest, givenNegativeOutputThenProcessInstruction) {
    const OpCode opCode = GetParam();
    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    getReg(opCode) = 0x7F;

    processor.executeInstructions(1);
    EXPECT_EQ(0x80, getReg(opCode));
    EXPECT_EQ(1, processor.counters.bytesProcessed);
    EXPECT_EQ(2, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_P(InxInyTest, givenZeroOutputThenProcessInstruction) {
    const OpCode opCode = GetParam();
    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    getReg(opCode) = 0xFF;

    processor.executeInstructions(1);
    EXPECT_EQ(0x00, getReg(opCode));
    EXPECT_EQ(1, processor.counters.bytesProcessed);
    EXPECT_EQ(2, processor.counters.cyclesProcessed);
    EXPECT_TRUE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

INSTANTIATE_TEST_SUITE_P(, InxInyTest, ::testing::ValuesIn({OpCode::INX, OpCode::INY}), InxInyTest::constructParamName);

using DexDeyTest = InxInyDexDeyTest;

TEST_P(DexDeyTest, givenDecrementOperationAndPositiveOutputThenProcessInstruction) {
    const OpCode opCode = GetParam();
    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    getReg(opCode) = 0x10;

    processor.executeInstructions(1);
    EXPECT_EQ(0x0F, getReg(opCode));
    EXPECT_EQ(1, processor.counters.bytesProcessed);
    EXPECT_EQ(2, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

TEST_P(DexDeyTest, givenDecrementOperationAndZeroOutputThenProcessInstruction) {
    const OpCode opCode = GetParam();
    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    getReg(opCode) = 0x01;

    processor.executeInstructions(1);
    EXPECT_EQ(0x00, getReg(opCode));
    EXPECT_EQ(1, processor.counters.bytesProcessed);
    EXPECT_EQ(2, processor.counters.cyclesProcessed);
    EXPECT_TRUE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

TEST_P(DexDeyTest, givenDecrementOperationAndNegativeOutputThenProcessInstruction) {
    const OpCode opCode = GetParam();
    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    getReg(opCode) = 0x81;

    processor.executeInstructions(1);
    EXPECT_EQ(0x80, getReg(opCode));
    EXPECT_EQ(1, processor.counters.bytesProcessed);
    EXPECT_EQ(2, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_P(DexDeyTest, givenDecrementOperationWrapAroundThenProcessInstruction) {
    const OpCode opCode = GetParam();
    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    getReg(opCode) = 0x00;

    processor.executeInstructions(1);
    EXPECT_EQ(0xFF, getReg(opCode));
    EXPECT_EQ(1, processor.counters.bytesProcessed);
    EXPECT_EQ(2, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

INSTANTIATE_TEST_SUITE_P(, DexDeyTest, ::testing::ValuesIn({OpCode::DEX, OpCode::DEY}), DexDeyTest::constructParamName);
