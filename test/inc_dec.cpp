#include "fixtures.h"

using IncTest = EmosTest;

TEST_F(IncTest, givenZeroPageModeThenProcessInstruction) {
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::INC_z);
    processor.memory[startAddress + 1] = 0x90;
    processor.memory[0x90] = 0x70;

    processor.executeInstructions(1);
    EXPECT_EQ(0x71, processor.memory[0x90]);
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(5, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

TEST_F(IncTest, givenZeroPageModeAndNegativeValueThenProcessInstruction) {
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::INC_z);
    processor.memory[startAddress + 1] = 0x90;
    processor.memory[0x90] = 0x7F;

    processor.executeInstructions(1);
    EXPECT_EQ(0x80, processor.memory[0x90]);
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(5, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_F(IncTest, givenZeroPageModeAndWrapAroundThenProcessInstruction) {
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::INC_z);
    processor.memory[startAddress + 1] = 0x90;
    processor.memory[0x90] = 0xFF;

    processor.executeInstructions(1);
    EXPECT_EQ(0x00, processor.memory[0x90]);
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(5, processor.counters.cyclesProcessed);
    EXPECT_TRUE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

TEST_F(IncTest, givenZeroPageXModeThenProcessInstruction) {
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::INC_zx);
    processor.memory[startAddress + 1] = 0x90;
    processor.regs.x = 0x5;
    processor.memory[0x95] = 0x70;

    processor.executeInstructions(1);
    EXPECT_EQ(0x71, processor.memory[0x95]);
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(6, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

TEST_F(IncTest, givenAbsoluteModeThenProcessInstruction) {
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::INC_abs);
    processor.memory[startAddress + 1] = 0x90;
    processor.memory[startAddress + 2] = 0x80;
    processor.memory[0x8090] = 0x70;

    processor.executeInstructions(1);
    EXPECT_EQ(0x71, processor.memory[0x8090]);
    EXPECT_EQ(3, processor.counters.bytesProcessed);
    EXPECT_EQ(6, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

TEST_F(IncTest, givenAbsoluteXModeThenProcessInstruction) {
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::INC_absx);
    processor.memory[startAddress + 1] = 0x90;
    processor.memory[startAddress + 2] = 0x80;
    processor.regs.x = 0x5;
    processor.memory[0x8095] = 0x70;

    processor.executeInstructions(1);
    EXPECT_EQ(0x71, processor.memory[0x8095]);
    EXPECT_EQ(3, processor.counters.bytesProcessed);
    EXPECT_EQ(7, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

using DecTest = EmosTest;

TEST_F(DecTest, givenZeroPageModeThenProcessInstruction) {
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::DEC_z);
    processor.memory[startAddress + 1] = 0x90;
    processor.memory[0x90] = 0x80;

    processor.executeInstructions(1);
    EXPECT_EQ(0x7F, processor.memory[0x90]);
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(5, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

TEST_F(DecTest, givenZeroPageModeAndNegativeValueThenProcessInstruction) {
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::DEC_z);
    processor.memory[startAddress + 1] = 0x90;
    processor.memory[0x90] = 0x81;

    processor.executeInstructions(1);
    EXPECT_EQ(0x80, processor.memory[0x90]);
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(5, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_F(DecTest, givenZeroPageModeAndResultIsZeroThenProcessInstruction) {
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::DEC_z);
    processor.memory[startAddress + 1] = 0x90;
    processor.memory[0x90] = 0x01;

    processor.executeInstructions(1);
    EXPECT_EQ(0x00, processor.memory[0x90]);
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(5, processor.counters.cyclesProcessed);
    EXPECT_TRUE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

TEST_F(DecTest, givenZeroPageModeAndWrapAroundThenProcessInstruction) {
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::DEC_z);
    processor.memory[startAddress + 1] = 0x90;
    processor.memory[0x90] = 0x00;

    processor.executeInstructions(1);
    EXPECT_EQ(0xFF, processor.memory[0x90]);
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(5, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_F(DecTest, givenZeroPageXModeThenProcessInstruction) {
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::DEC_zx);
    processor.memory[startAddress + 1] = 0x90;
    processor.regs.x = 0x5;
    processor.memory[0x95] = 0x70;

    processor.executeInstructions(1);
    EXPECT_EQ(0x6F, processor.memory[0x95]);
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(6, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

TEST_F(DecTest, givenAbsoluteModeThenProcessInstruction) {
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::DEC_abs);
    processor.memory[startAddress + 1] = 0x90;
    processor.memory[startAddress + 2] = 0x80;
    processor.memory[0x8090] = 0x70;

    processor.executeInstructions(1);
    EXPECT_EQ(0x6F, processor.memory[0x8090]);
    EXPECT_EQ(3, processor.counters.bytesProcessed);
    EXPECT_EQ(6, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

TEST_F(DecTest, givenAbsoluteXModeThenProcessInstruction) {
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::DEC_absx);
    processor.memory[startAddress + 1] = 0x90;
    processor.memory[startAddress + 2] = 0x80;
    processor.regs.x = 0x5;
    processor.memory[0x8095] = 0x70;

    processor.executeInstructions(1);
    EXPECT_EQ(0x6F, processor.memory[0x8095]);
    EXPECT_EQ(3, processor.counters.bytesProcessed);
    EXPECT_EQ(7, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}
