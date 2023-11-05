#include "src/error.h"
#include "test/fixtures/emos_test.h"

#include <tuple>

struct PlpTest : EmosTest {
    void initializeProcessor(OpCode opcode, [[maybe_unused]] std::optional<u8> value, [[maybe_unused]] std::optional<u8> loadToReg) override {
        initializeForImplied(opcode);
        expectedBytesProcessed = 1u;
        expectedCyclesProcessed = 4u;
    }
};

TEST_F(PlpTest, givenZeroSpThenValuePoppedFromStackToFlags) {
    flags.expectCarryFlag(false);
    flags.expectZeroFlag(false);
    flags.expectInterruptFlag(false);
    flags.expectDecimalFlag(false);
    flags.expectBreakFlag(false);
    flags.expectOverflowFlag(false);
    flags.expectNegativeFlag(false);
    flags.expectReservedFlag(false);

    initializeProcessor(OpCode::PLP, {}, {});
    processor.regs.sp = 0x00;
    processor.memory[0x100] = 0x40;
    processor.memory[0x101] = 0x00; // this is the last value pushed on stack
    processor.executeInstructions(1);

    EXPECT_EQ(0x01, processor.regs.sp);
}

TEST_F(PlpTest, givenSpThenValuePoppedFromStackToFlags) {
    flags.expectCarryFlag(false);
    flags.expectZeroFlag(false);
    flags.expectInterruptFlag(false);
    flags.expectDecimalFlag(false);
    flags.expectBreakFlag(false);
    flags.expectOverflowFlag(false);
    flags.expectNegativeFlag(false);
    flags.expectReservedFlag(false);

    initializeProcessor(OpCode::PLP, {}, {});
    processor.regs.sp = 0xCC;
    processor.memory[0x1CB] = 0x60;
    processor.memory[0x1CC] = 0x61;
    processor.memory[0x1CD] = 0x00;
    processor.executeInstructions(1);

    EXPECT_EQ(0xCD, processor.regs.sp);
}

TEST_F(PlpTest, givenSpAndOneFlagsThenValuePoppedFromStackToFlags) {
    flags.expectCarryFlag(true);
    flags.expectZeroFlag(true);
    flags.expectInterruptFlag(true);
    flags.expectDecimalFlag(true);
    flags.expectBreakFlag(true);
    flags.expectOverflowFlag(true);
    flags.expectNegativeFlag(true);
    flags.expectReservedFlag(true);

    initializeProcessor(OpCode::PLP, {}, {});
    processor.regs.sp = 0xCC;
    processor.memory[0x1CB] = 0x60;
    processor.memory[0x1CC] = 0x61;
    processor.memory[0x1CD] = 0xFF;
    processor.executeInstructions(1);

    EXPECT_EQ(0xCD, processor.regs.sp);
}

TEST_F(PlpTest, givenSpAndVariousFlagsThenValuePoppedFromStackToFlags) {
    flags.expectCarryFlag(true);
    flags.expectZeroFlag(false);
    flags.expectInterruptFlag(true);
    flags.expectDecimalFlag(false);
    flags.expectBreakFlag(true);
    flags.expectOverflowFlag(true);
    flags.expectNegativeFlag(false);
    flags.expectReservedFlag(false);

    initializeProcessor(OpCode::PLP, {}, {});
    processor.regs.sp = 0xCC;
    processor.memory[0x1CB] = 0x60;
    processor.memory[0x1CC] = 0x61;
    processor.memory[0x1CD] = 0b10101100;
    processor.executeInstructions(1);

    EXPECT_EQ(0xCD, processor.regs.sp);
}

TEST_F(PlpTest, givenMaximumSpThenValuePoppedFromStackToFlags) {
    flags.expectCarryFlag(false);
    flags.expectZeroFlag(false);
    flags.expectInterruptFlag(false);
    flags.expectDecimalFlag(false);
    flags.expectBreakFlag(false);
    flags.expectOverflowFlag(false);
    flags.expectNegativeFlag(false);
    flags.expectReservedFlag(false);

    initializeProcessor(OpCode::PLP, {}, {});
    processor.regs.sp = 0xFF;
    processor.memory[0x1EF] = 0x50;
    processor.memory[0x1FF] = 0x51;
    processor.memory[0x100] = 0x00;
    processor.executeInstructions(1);

    EXPECT_EQ(0x00, processor.regs.sp);
}
