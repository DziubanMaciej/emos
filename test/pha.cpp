#include "src/error.h"
#include "test/fixtures/emos_test.h"

#include <tuple>

struct PhaTest : EmosTest {
    void initializeProcessor(OpCode opcode, [[maybe_unused]] std::optional<u8> value, [[maybe_unused]] std::optional<u8> loadToReg) {
        expectedBytesProcessed = 1u;
        expectedCyclesProcessed = 3u;
        initializeForImplied(opcode);
    }
};

TEST_F(PhaTest, givenMaximumSpThenValueIsPushedOnStack) {
    initializeProcessor(OpCode::PHA, {}, {});
    processor.regs.a = 0x55;
    processor.regs.sp = 0xFF;
    processor.executeInstructions(1);

    EXPECT_EQ(0xFE, processor.regs.sp);
    EXPECT_EQ(0x55, processor.memory[0x1FF]);
    EXPECT_EQ(0x55, processor.regs.a);
}

TEST_F(PhaTest, givenZeroSpThenValueIsPushedOnStack) {
    initializeProcessor(OpCode::PHA, {}, {});
    processor.regs.a = 0x55;
    processor.regs.sp = 0x00;
    processor.executeInstructions(1);

    EXPECT_EQ(0xFF, processor.regs.sp); // wrap around
    EXPECT_EQ(0x55, processor.memory[0x100]);
    EXPECT_EQ(0x55, processor.regs.a);
}
