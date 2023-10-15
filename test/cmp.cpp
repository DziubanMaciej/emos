#include "fixtures.h"

using CmpTest = EmosTest;

TEST_F(CmpTest, givenImmediateModeAndGreaterValueThenProcessInstruction) {
    processor.regs.a = 0xCC;
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::CMP_imm);
    processor.memory[startAddress + 1] = 0xCD;

    processor.executeInstructions(1);
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(2, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.c);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_F(CmpTest, givenImmediateModeAndEqualValueThenProcessInstruction) {
    processor.regs.a = 0xCC;
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::CMP_imm);
    processor.memory[startAddress + 1] = 0xCC;

    processor.executeInstructions(1);
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(2, processor.counters.cyclesProcessed);
    EXPECT_TRUE(processor.regs.flags.c);
    EXPECT_TRUE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

TEST_F(CmpTest, givenImmediateModeAndLessValueThenProcessInstruction) {
    processor.regs.a = 0xCC;
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::CMP_imm);
    processor.memory[startAddress + 1] = 0xCB;

    processor.executeInstructions(1);
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(2, processor.counters.cyclesProcessed);
    EXPECT_TRUE(processor.regs.flags.c);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

TEST_F(CmpTest, givenZeroPageModeThenProcessInstruction) {
    processor.regs.a = 0xCC;
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::CMP_z);
    processor.memory[startAddress + 1] = 0x05;
    processor.memory[0x05] = 0xCD;

    processor.executeInstructions(1);
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(3, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.c);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_F(CmpTest, givenZeroPageModeXThenProcessInstruction) {
    processor.regs.a = 0xCC;
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::CMP_zx);
    processor.memory[startAddress + 1] = 0x05;
    processor.regs.x = 0x3;
    processor.memory[0x08] = 0xCD;

    processor.executeInstructions(1);
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(4, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.c);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_F(CmpTest, givenAbsoluteModeThenProcessInstruction) {
    processor.regs.a = 0xCC;
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::CMP_abs);
    processor.memory[startAddress + 1] = 0x34;
    processor.memory[startAddress + 2] = 0x12;
    processor.memory[0x1234] = 0xCD;

    processor.executeInstructions(1);
    EXPECT_EQ(3, processor.counters.bytesProcessed);
    EXPECT_EQ(4, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.c);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_F(CmpTest, givenAbsoluteModeXThenProcessInstruction) {
    processor.regs.a = 0xCC;
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::CMP_absx);
    processor.memory[startAddress + 1] = 0x34;
    processor.memory[startAddress + 2] = 0x12;
    processor.regs.x = 0x3;
    processor.memory[0x1237] = 0xCD;

    processor.executeInstructions(1);
    EXPECT_EQ(3, processor.counters.bytesProcessed);
    EXPECT_EQ(4, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.c);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_F(CmpTest, givenAbsoluteModeYThenProcessInstruction) {
    processor.regs.a = 0xCC;
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::CMP_absy);
    processor.memory[startAddress + 1] = 0x34;
    processor.memory[startAddress + 2] = 0x12;
    processor.regs.y = 0x3;
    processor.memory[0x1237] = 0xCD;

    processor.executeInstructions(1);
    EXPECT_EQ(3, processor.counters.bytesProcessed);
    EXPECT_EQ(4, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.c);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_F(CmpTest, givenIndexedIndirectXModeThenProcessInstruction) {
    processor.regs.a = 0xCC;
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::CMP_ix);
    processor.memory[startAddress + 1] = 0x54;
    processor.regs.x = 0x5;
    processor.memory[0x59] = 0x34;
    processor.memory[0x5A] = 0x12;
    processor.memory[0x1234] = 0xCD;

    processor.executeInstructions(1);
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(6, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.c);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_F(CmpTest, givenIndirectIndexedYModeThenProcessInstruction) {
    processor.regs.a = 0xCC;
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::CMP_iy);
    processor.memory[startAddress + 1] = 0x54;
    processor.memory[0x54] = 0x30;
    processor.memory[0x55] = 0x12;
    processor.regs.y = 0x4;
    processor.memory[0x1234] = 0xCD;

    processor.executeInstructions(1);
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(5, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.c);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}
