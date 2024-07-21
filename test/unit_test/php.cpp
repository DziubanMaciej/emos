#include "src/error.h"
#include "unit_test/fixtures/emos_test.h"

#include <tuple>

struct PhpTest : EmosTest {
    void initializeProcessor(OpCode opcode, [[maybe_unused]] std::optional<u8> value, [[maybe_unused]] std::optional<u8> loadToReg) {
        expectedBytesProcessed = 1u;
        expectedCyclesProcessed = 3u;
        initializeForImplied(opcode);
    }
};

TEST_F(PhpTest, givenZeroFlagsThenValueIsPushedOnStack) {
    initializeProcessor(OpCode::PHP, {}, {});

    flags.expectCarryFlag(false, false);
    flags.expectZeroFlag(false, false);
    flags.expectInterruptFlag(false, false);
    flags.expectDecimalFlag(false, false);
    flags.expectBreakFlag(false, false);
    flags.expectOverflowFlag(false, false);
    flags.expectNegativeFlag(false, false);
    flags.expectReservedFlag(false, false);

    processor.regs.sp = 0xFF;
    processor.executeInstructions(1);

    EXPECT_EQ(0xFE, processor.regs.sp);
    EXPECT_EQ(0x00, processor.memory[0x1FF]);
}

TEST_F(PhpTest, givenOneFlagsThenValueIsPushedOnStack) {
    initializeProcessor(OpCode::PHP, {}, {});

    flags.expectCarryFlag(true, true);
    flags.expectZeroFlag(true, true);
    flags.expectInterruptFlag(true, true);
    flags.expectDecimalFlag(true, true);
    flags.expectBreakFlag(true, true);
    flags.expectOverflowFlag(true, true);
    flags.expectNegativeFlag(true, true);
    flags.expectReservedFlag(true, true);

    processor.regs.sp = 0xFF;
    processor.executeInstructions(1);

    EXPECT_EQ(0xFE, processor.regs.sp);
    EXPECT_EQ(0xFF, processor.memory[0x1FF]);
}

TEST_F(PhpTest, givenVariousFlagsThenValueIsPushedOnStack) {
    initializeProcessor(OpCode::PHP, {}, {});

    flags.expectCarryFlag(true, true);
    flags.expectZeroFlag(false, false);
    flags.expectInterruptFlag(true, true);
    flags.expectDecimalFlag(true, true);
    flags.expectBreakFlag(true, true);
    flags.expectOverflowFlag(false, false);
    flags.expectNegativeFlag(false, false);
    flags.expectReservedFlag(true, true);

    processor.regs.sp = 0xFF;
    processor.executeInstructions(1);

    EXPECT_EQ(0xFE, processor.regs.sp);
    EXPECT_EQ(0b10111001, processor.memory[0x1FF]);
}
