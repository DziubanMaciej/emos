#include "src/error.h"
#include "test/fixtures/emos_test.h"

struct JmpTest : EmosTest {
};

TEST_F(JmpTest, givenIndirectAddressingThenJumpCorrectly) {
    processor.memory[0x0120] = 0xFC;
    processor.memory[0x0121] = 0xBA;

    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::JMP_i);
    processor.memory[startAddress + 1] = 0x20;
    processor.memory[startAddress + 2] = 0x01;

    processor.executeInstructions(1);
    EXPECT_EQ(0xBAFC, processor.regs.pc);
    expectedCyclesProcessed = 5;
    expectedBytesProcessed = 3;
}

TEST_F(JmpTest, givenAbsoluteAddressingThenJumpCorrectly) {
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::JMP_abs);
    processor.memory[startAddress + 1] = 0x20;
    processor.memory[startAddress + 2] = 0x12;

    processor.executeInstructions(1);
    EXPECT_EQ(0x1220, processor.regs.pc);
    expectedCyclesProcessed = 3;
    expectedBytesProcessed = 3;
}
