#include "src/error.h"
#include "test/fixtures/emos_test.h"

#include <tuple>

struct RorTest : testing::WithParamInterface<OpCode>, EmosTest {
    void initializeProcessor(OpCode opcode, std::optional<u8> value, [[maybe_unused]] std::optional<u8> loadToReg) override {
        switch (opcode) {
        case OpCode::ROR_acc:
            UNREACHABLE_CODE; // TODO!!!
            expectedBytesProcessed = 1u;
            expectedCyclesProcessed = 2u;
            return;
        case OpCode::ROR_z:
            initializeForZeroPage(opcode, value.value());
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 5u;
            return;
        case OpCode::ROR_zx:
            initializeForZeroPageX(opcode, value.value());
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 6u;
            return;
        case OpCode::ROR_abs:
            initializeForAbsolute(opcode, value.value());
            expectedBytesProcessed = 3u;
            expectedCyclesProcessed = 6u;
            return;
        case OpCode::ROR_absx:
            initializeForAbsoluteX(opcode, value.value());
            expectedBytesProcessed = 3u;
            expectedCyclesProcessed = 7u;
            return;
        default:
            FATAL_ERROR("Wrong OpCode");
        }
    }

    static std::string constructParamName(const testing::TestParamInfo<OpCode> &info) {
        switch (info.param) {
        case OpCode::ROR_acc:
            return "ROR_acc";
        case OpCode::ROR_z:
            return "ROR_z";
        case OpCode::ROR_zx:
            return "ROR_zx";
        case OpCode::ROR_abs:
            return "ROR_abs";
        case OpCode::ROR_absx:
            return "ROR_absx";
        default:
            FATAL_ERROR("Wrong OpCode");
        }
    }
};

TEST_P(RorTest, givenNumberThenShiftProperly) {
    const u8 inpValue = 0b00110010;
    const u8 outValue = 0b00011001;
    initializeProcessor(GetParam(), inpValue, {});

    flags.expectCarryFlag(false, false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);

    EXPECT_EQ(outValue, referencedValue.read());
}

TEST_P(RorTest, givenLeastSignificantBitSetThenSetCarryBit) {
    const u8 inpValue = 0b00010001;
    const u8 outValue = 0b00001000;
    initializeProcessor(GetParam(), inpValue, {});

    flags.expectCarryFlag(false, true);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);

    EXPECT_EQ(outValue, referencedValue.read());
}

TEST_P(RorTest, givenZeroValueThenZeroFlagSet) {
    const u8 inpValue = 0b00000000;
    const u8 outValue = 0b00000000;
    initializeProcessor(GetParam(), inpValue, {});

    flags.expectCarryFlag(false, false);
    flags.expectZeroFlag(true);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);

    EXPECT_EQ(outValue, referencedValue.read());
}

TEST_P(RorTest, givenZeroValueAfterShiftThenZeroFlagSet) {
    const u8 inpValue = 0b00000001;
    const u8 outValue = 0b00000000;
    initializeProcessor(GetParam(), inpValue, {});

    flags.expectCarryFlag(false, true);
    flags.expectZeroFlag(true);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);

    EXPECT_EQ(outValue, referencedValue.read());
}

TEST_P(RorTest, givenCarryFlagSetThenRotateIntoMostSignificantBit) {
    const u8 inpValue = 0b00001110;
    const u8 outValue = 0b10000111;
    initializeProcessor(GetParam(), inpValue, {});

    flags.expectCarryFlag(true, false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);

    EXPECT_EQ(outValue, referencedValue.read());
}

TEST_P(RorTest, givenCarryFlagAndMostSignificantBitSetThenRotateIntoMostSignificantBit) {
    const u8 inpValue = 0b10001110;
    const u8 outValue = 0b11000111;
    initializeProcessor(GetParam(), inpValue, {});

    flags.expectCarryFlag(true, false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);

    EXPECT_EQ(outValue, referencedValue.read());
}

TEST_P(RorTest, givenCarryFlagAndLeastSignificantBitSetThenRotateThroughCarry) {
    const u8 inpValue = 0b00000001;
    const u8 outValue = 0b10000000;
    initializeProcessor(GetParam(), inpValue, {});

    flags.expectCarryFlag(true, true);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);

    EXPECT_EQ(outValue, referencedValue.read());
}

INSTANTIATE_TEST_SUITE_P(,
                         RorTest,
                         ::testing::ValuesIn({OpCode::ROR_z, OpCode::ROR_zx, OpCode::ROR_abs, OpCode::ROR_absx}),
                         RorTest::constructParamName);
