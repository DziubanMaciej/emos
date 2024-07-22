#include "src/bit_operations.h"
#include "src/error.h"
#include "unit_test/fixtures/emos_test.h"

struct BrkRtiTest : EmosTest {};

TEST_F(BrkRtiTest, whenProcessingBrkThenPushStateOnStackAndJump) {
    processor.regs.sp = 0xFF;

    // Set some flags to verify pushing flags to stack
    flags.expectCarryFlag(true, true);
    flags.expectOverflowFlag(true, true);

    // Interrupt disable flag should be set
    flags.expectInterruptFlag(true);

    // BRK instruction. It's actually two bytes, but the second one is unused
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::BRK);

    // Interrupt vector
    processor.memory[0xFFFE] = 0x20;
    processor.memory[0xFFFF] = 0x12;

    const u16 pushedPc = startAddress + 2;
    StatusFlags pushedFlags = processor.regs.flags;
    pushedFlags.b = 1;
    pushedFlags.r = 1;

    processor.executeInstructions(1);

    EXPECT_EQ(hi(pushedPc), processor.memory[0x1FF]);
    EXPECT_EQ(lo(pushedPc), processor.memory[0x1FE]);
    EXPECT_EQ(pushedFlags.toU8(), processor.memory[0x1FD]);
    EXPECT_EQ(0xFC, processor.regs.sp);
    EXPECT_EQ(0x1220, processor.regs.pc);

    expectedCyclesProcessed = 7;
    expectedBytesProcessed = 1;
}

TEST_F(BrkRtiTest, whenProcessingRtiThenTakeValuesFromStack) {
    processor.regs.sp = 0xF0;

    StatusFlags flagsOnStack = {};
    flagsOnStack.b = 1;
    flagsOnStack.c = 1;
    flagsOnStack.o = 1;
    flagsOnStack.r = 1;
    flags.expectCarryFlag(flagsOnStack.c);
    flags.expectZeroFlag(flagsOnStack.z);
    flags.expectInterruptFlag(flagsOnStack.i);
    flags.expectDecimalFlag(flagsOnStack.d);
    flags.expectBreakFlag(false, false); // RTI ignores break flag
    flags.expectOverflowFlag(flagsOnStack.o);
    flags.expectNegativeFlag(flagsOnStack.n);
    flags.expectReservedFlag(false, false); // RTI ignore reserved flag
    const u16 pcOnStack = 0x2030;

    processor.memory[0x1F1] = flagsOnStack.toU8();
    processor.memory[0x1F2] = lo(pcOnStack);
    processor.memory[0x1F3] = hi(pcOnStack);

    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::RTI);

    processor.executeInstructions(1);
    EXPECT_EQ(0xF3, processor.regs.sp);
    EXPECT_EQ(pcOnStack, processor.regs.pc);

    expectedCyclesProcessed = 6;
    expectedBytesProcessed = 1;
}

TEST_F(BrkRtiTest, givenInterruptDisableFlagOnStackIsSetWhenProcessingRtiThenLeaveItAsSet) {
    processor.regs.sp = 0xF0;

    StatusFlags flagsOnStack = {};
    flagsOnStack.i = 1;
    flags.expectInterruptFlag(true, true);
    flags.expectBreakFlag(false, false);    // RTI ignores break flag
    flags.expectReservedFlag(false, false); // RTI ignore reserved flag
    const u16 pcOnStack = 0x2030;

    processor.memory[0x1F1] = flagsOnStack.toU8();
    processor.memory[0x1F2] = lo(pcOnStack);
    processor.memory[0x1F3] = hi(pcOnStack);

    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::RTI);

    processor.executeInstructions(1);
    EXPECT_EQ(0xF3, processor.regs.sp);
    EXPECT_EQ(pcOnStack, processor.regs.pc);

    expectedCyclesProcessed = 6;
    expectedBytesProcessed = 1;
}

TEST_F(BrkRtiTest, whenProcessingInterruptThenExecuteItAndReturn) {
    processor.regs.sp = 0xF0;
    processor.regs.x = 0xC0;
    processor.regs.y = 0xD0;

    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::BRK); // 7 cycles
    processor.memory[startAddress + 1] = 0;
    processor.memory[startAddress + 2] = static_cast<u8>(OpCode::INX); // 2 cycles
    processor.memory[startAddress + 3] = static_cast<u8>(OpCode::INX); // 2 cycles

    processor.memory[0xFFFE] = 0x20;
    processor.memory[0xFFFF] = 0x12;

    processor.memory[0x1220] = static_cast<u8>(OpCode::INY); // 2 cycles
    processor.memory[0x1221] = static_cast<u8>(OpCode::INY); // 2 cycles
    processor.memory[0x1222] = static_cast<u8>(OpCode::RTI); // 6 cycles

    processor.executeInstructions(6);

    EXPECT_EQ(0xF0, processor.regs.sp);
    EXPECT_EQ(startAddress + 4, processor.regs.pc);
    EXPECT_EQ(0xC2, processor.regs.x);
    EXPECT_EQ(0xD2, processor.regs.y);

    expectedCyclesProcessed = 21;
    expectedBytesProcessed = 6;
}
