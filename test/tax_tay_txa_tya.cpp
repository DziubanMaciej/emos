#include "fixtures.h"

using TaxTest = EmosTest;

TEST_F(TaxTest, givenNegativeValueThenProcessInstruction) {
    processor.regs.a = 0x80;
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::TAX);

    processor.executeInstructions(1);
    EXPECT_EQ(1, processor.counters.bytesProcessed);
    EXPECT_EQ(2, processor.counters.cyclesProcessed);
    EXPECT_EQ(0x80, processor.regs.x);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_F(TaxTest, givenPositiveValueThenProcessInstruction) {
    processor.regs.a = 0x70;
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::TAX);

    processor.executeInstructions(1);
    EXPECT_EQ(1, processor.counters.bytesProcessed);
    EXPECT_EQ(2, processor.counters.cyclesProcessed);
    EXPECT_EQ(0x70, processor.regs.x);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

TEST_F(TaxTest, givenZeroValueThenProcessInstruction) {
    processor.regs.a = 0x00;
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::TAX);

    processor.executeInstructions(1);
    EXPECT_EQ(1, processor.counters.bytesProcessed);
    EXPECT_EQ(2, processor.counters.cyclesProcessed);
    EXPECT_EQ(0x00, processor.regs.x);
    EXPECT_TRUE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

using TayTest = EmosTest;

TEST_F(TayTest, givenNegativeValueThenProcessInstruction) {
    processor.regs.a = 0x80;
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::TAY);

    processor.executeInstructions(1);
    EXPECT_EQ(1, processor.counters.bytesProcessed);
    EXPECT_EQ(2, processor.counters.cyclesProcessed);
    EXPECT_EQ(0x80, processor.regs.y);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_F(TayTest, givenPositiveValueThenProcessInstruction) {
    processor.regs.a = 0x70;
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::TAY);

    processor.executeInstructions(1);
    EXPECT_EQ(1, processor.counters.bytesProcessed);
    EXPECT_EQ(2, processor.counters.cyclesProcessed);
    EXPECT_EQ(0x70, processor.regs.y);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

TEST_F(TayTest, givenZeroValueThenProcessInstruction) {
    processor.regs.a = 0x00;
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::TAY);

    processor.executeInstructions(1);
    EXPECT_EQ(1, processor.counters.bytesProcessed);
    EXPECT_EQ(2, processor.counters.cyclesProcessed);
    EXPECT_EQ(0x00, processor.regs.y);
    EXPECT_TRUE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

using TxaTest = EmosTest;

TEST_F(TxaTest, givenNegativeValueThenProcessInstruction) {
    processor.regs.x = 0x80;
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::TXA);

    processor.executeInstructions(1);
    EXPECT_EQ(1, processor.counters.bytesProcessed);
    EXPECT_EQ(2, processor.counters.cyclesProcessed);
    EXPECT_EQ(0x80, processor.regs.a);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_F(TxaTest, givenPositiveValueThenProcessInstruction) {
    processor.regs.x = 0x70;
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::TXA);

    processor.executeInstructions(1);
    EXPECT_EQ(1, processor.counters.bytesProcessed);
    EXPECT_EQ(2, processor.counters.cyclesProcessed);
    EXPECT_EQ(0x70, processor.regs.a);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

TEST_F(TxaTest, givenZeroValueThenProcessInstruction) {
    processor.regs.x = 0x00;
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::TXA);

    processor.executeInstructions(1);
    EXPECT_EQ(1, processor.counters.bytesProcessed);
    EXPECT_EQ(2, processor.counters.cyclesProcessed);
    EXPECT_EQ(0x00, processor.regs.a);
    EXPECT_TRUE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

using TyaTest = EmosTest;

TEST_F(TyaTest, givenNegativeValueThenProcessInstruction) {
    processor.regs.y = 0x80;
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::TYA);

    processor.executeInstructions(1);
    EXPECT_EQ(1, processor.counters.bytesProcessed);
    EXPECT_EQ(2, processor.counters.cyclesProcessed);
    EXPECT_EQ(0x80, processor.regs.a);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_F(TyaTest, givenPositiveValueThenProcessInstruction) {
    processor.regs.y = 0x70;
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::TYA);

    processor.executeInstructions(1);
    EXPECT_EQ(1, processor.counters.bytesProcessed);
    EXPECT_EQ(2, processor.counters.cyclesProcessed);
    EXPECT_EQ(0x70, processor.regs.a);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

TEST_F(TyaTest, givenZeroValueThenProcessInstruction) {
    processor.regs.y = 0x00;
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::TYA);

    processor.executeInstructions(1);
    EXPECT_EQ(1, processor.counters.bytesProcessed);
    EXPECT_EQ(2, processor.counters.cyclesProcessed);
    EXPECT_EQ(0x00, processor.regs.a);
    EXPECT_TRUE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}
