#include "src/error.h"
#include "unit_test/fixtures/emos_test.h"

struct PlaTest : EmosTest {
    void initializeProcessor(OpCode opcode, [[maybe_unused]] std::optional<u8> value, [[maybe_unused]] std::optional<u8> loadToReg) {
        expectedBytesProcessed = 1u;
        expectedCyclesProcessed = 4u;
        initializeForImplied(opcode);
    }
};

TEST_F(PlaTest, givenZeroSpThenValuePoppedFromStack) {
    initializeProcessor(OpCode::PLA, {}, {});
    processor.regs.sp = 0x00;
    processor.memory[0x100] = 0x40;
    processor.memory[0x101] = 0x41; // this is the last value pushed on stack
    processor.executeInstructions(1);

    EXPECT_EQ(0x01, processor.regs.sp);
    EXPECT_EQ(0x41, processor.regs.a);
}

TEST_F(PlaTest, givenSpThenValuePoppedFromStack) {
    initializeProcessor(OpCode::PLA, {}, {});
    processor.regs.sp = 0xCC;
    processor.memory[0x1CB] = 0x60;
    processor.memory[0x1CC] = 0x61;
    processor.memory[0x1CD] = 0x62;
    processor.executeInstructions(1);

    EXPECT_EQ(0xCD, processor.regs.sp);
    EXPECT_EQ(0x62, processor.regs.a);
}

TEST_F(PlaTest, givenMaximumSpThenValuePoppedFromStack) {
    initializeProcessor(OpCode::PLA, {}, {});
    processor.regs.sp = 0xFF;
    processor.memory[0x1EF] = 0x50;
    processor.memory[0x1FF] = 0x51;
    processor.memory[0x100] = 0x52;
    processor.executeInstructions(1);

    EXPECT_EQ(0x00, processor.regs.sp);
    EXPECT_EQ(0x52, processor.regs.a);
}

TEST_F(PlaTest, givenZeroValueOnStackThenSetZeroFlag) {
    flags.expectZeroFlag(true);

    initializeProcessor(OpCode::PLA, {}, {});
    processor.regs.sp = 0xCC;
    processor.memory[0x1CB] = 0x60;
    processor.memory[0x1CC] = 0x61;
    processor.memory[0x1CD] = 0x00;
    processor.executeInstructions(1);

    EXPECT_EQ(0xCD, processor.regs.sp);
    EXPECT_EQ(0x00, processor.regs.a);
}

TEST_F(PlaTest, givenNegativeValueOnStackThenSetNegativeFlag) {
    flags.expectNegativeFlag(true);

    initializeProcessor(OpCode::PLA, {}, {});
    processor.regs.sp = 0xCC;
    processor.memory[0x1CB] = 0x60;
    processor.memory[0x1CC] = 0x61;
    processor.memory[0x1CD] = 0x84;
    processor.executeInstructions(1);

    EXPECT_EQ(0xCD, processor.regs.sp);
    EXPECT_EQ(0x84, processor.regs.a);
}
