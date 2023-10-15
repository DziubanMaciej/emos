#include "fixtures.h"

using LdaTest = EmosTest;

TEST_F(LdaTest, givenImmediateModeAndNegativeValueThenProcessInstruction) {
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::LDA_imm);
    processor.memory[startAddress + 1] = 0x84;

    processor.executeInstructions(1);
    EXPECT_EQ(0x84, processor.regs.a);
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(2, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_F(LdaTest, givenImmediateModeAndZeroValueThenProcessInstruction) {
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::LDA_imm);
    processor.memory[startAddress + 1] = 0x0;

    processor.executeInstructions(1);
    EXPECT_EQ(0x0, processor.regs.a);
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(2, processor.counters.cyclesProcessed);
    EXPECT_TRUE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

TEST_F(LdaTest, givenImmediateModeAndPositiveValueThenProcessInstruction) {
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::LDA_imm);
    processor.memory[startAddress + 1] = 0x74;

    processor.executeInstructions(1);
    EXPECT_EQ(0x74, processor.regs.a);
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(2, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

TEST_F(LdaTest, givenZeroPageModeThenProcessInstruction) {
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::LDA_z);
    processor.memory[startAddress + 1] = 0x84;
    processor.memory[0x84] = 0xDD;

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, processor.regs.a);
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(3, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_F(LdaTest, givenZeroPageXModeThenProcessInstruction) {
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::LDA_zx);
    processor.memory[startAddress + 1] = 0x84;
    processor.regs.x = 0x5;
    processor.memory[0x89] = 0xDD;

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, processor.regs.a);
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(4, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_F(LdaTest, givenZeroPageXModeAndWrapAroundThenProcessInstruction) {
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::LDA_zx);
    processor.memory[startAddress + 1] = 0xF0;
    processor.regs.x = 0x30;
    processor.memory[0x20] = 0xDD; // address addition should wrap around to zero page address

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, processor.regs.a);
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(4, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_F(LdaTest, givenAbsoluteModeThenProcessInstruction) {
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::LDA_abs);
    processor.memory[startAddress + 1] = 0x34;
    processor.memory[startAddress + 2] = 0x12;
    processor.memory[0x1234] = 0xDD;

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, processor.regs.a);
    EXPECT_EQ(3, processor.counters.bytesProcessed);
    EXPECT_EQ(4, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_F(LdaTest, givenAbsoluteXModeThenProcessInstruction) {
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::LDA_absx);
    processor.memory[startAddress + 1] = 0x34;
    processor.memory[startAddress + 2] = 0x12;
    processor.regs.x = 0x5;
    processor.memory[0x1239] = 0xDD;

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, processor.regs.a);
    EXPECT_EQ(3, processor.counters.bytesProcessed);
    EXPECT_EQ(4, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_F(LdaTest, givenAbsoluteXModeAndPageCrossThenProcessInstruction) {
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::LDA_absx);
    processor.memory[startAddress + 1] = 0xF1;
    processor.memory[startAddress + 2] = 0x12;
    processor.regs.x = 0xF;
    processor.memory[0x1300] = 0xDD;

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, processor.regs.a);
    EXPECT_EQ(3, processor.counters.bytesProcessed);
    EXPECT_EQ(5, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_F(LdaTest, givenAbsoluteYModeThenProcessInstruction) {
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::LDA_absy);
    processor.memory[startAddress + 1] = 0x34;
    processor.memory[startAddress + 2] = 0x12;
    processor.regs.y = 0x5;
    processor.memory[0x1239] = 0xDD;

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, processor.regs.a);
    EXPECT_EQ(3, processor.counters.bytesProcessed);
    EXPECT_EQ(4, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_F(LdaTest, givenAbsoluteYModeAndPageCrossThenProcessInstruction) {
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::LDA_absy);
    processor.memory[startAddress + 1] = 0xF1;
    processor.memory[startAddress + 2] = 0x12;
    processor.regs.y = 0xF;
    processor.memory[0x1300] = 0xDD;

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, processor.regs.a);
    EXPECT_EQ(3, processor.counters.bytesProcessed);
    EXPECT_EQ(5, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_F(LdaTest, givenIndexedIndirectXModeThenProcessInstruction) {
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::LDA_ix);
    processor.memory[startAddress + 1] = 0x54;
    processor.regs.x = 0x5;
    processor.memory[0x59] = 0x34;
    processor.memory[0x5A] = 0x12;
    processor.memory[0x1234] = 0xDD;

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, processor.regs.a);
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(6, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_F(LdaTest, givenIndexedIndirectXModeAndWraparoundThenProcessInstruction) {
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::LDA_ix);
    processor.memory[startAddress + 1] = 0xF0;
    processor.regs.x = 0x30;
    processor.memory[0x20] = 0x34;
    processor.memory[0x21] = 0x12;
    processor.memory[0x1234] = 0xDD;

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, processor.regs.a);
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(6, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_F(LdaTest, givenIndirectIndexedYModeThenProcessInstruction) {
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::LDA_iy);
    processor.memory[startAddress + 1] = 0x54;
    processor.memory[0x54] = 0x30;
    processor.memory[0x55] = 0x12;
    processor.regs.y = 0x4;
    processor.memory[0x1234] = 0xDD;

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, processor.regs.a);
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(5, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_F(LdaTest, givenIndirectIndexedYModeAndPageCrossedThenProcessInstruction) {
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::LDA_iy);
    processor.memory[startAddress + 1] = 0x54;
    processor.memory[0x54] = 0xFF;
    processor.memory[0x55] = 0x12;
    processor.regs.y = 0x4;
    processor.memory[0x1303] = 0xDD;

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, processor.regs.a);
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(6, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}
