#include "src/error.h"
#include "test/fixtures/emos_test.h"

#include <tuple>

struct AslTest : testing::WithParamInterface<OpCode>, EmosTest {
    ReferencedValue initializeProcessor(OpCode opcode, std::optional<u8> value, [[maybe_unused]] std::optional<u8> loadToReg) {
        switch (opcode) {
        case OpCode::ASL_acc:
            expectedBytesProcessed = 1u;
            expectedCyclesProcessed = 2u;
            return initializeForAccumulator(opcode, value.value());
        case OpCode::ASL_z:
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 5u;
            return initializeForZeroPage(opcode, value.value());
        case OpCode::ASL_zx:
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 6u;
            return initializeForZeroPageX(opcode, value.value());
        case OpCode::ASL_abs:
            expectedBytesProcessed = 3u;
            expectedCyclesProcessed = 6u;
            return initializeForAbsolute(opcode, value.value());
        case OpCode::ASL_absx:
            expectedBytesProcessed = 3u;
            expectedCyclesProcessed = 7u;
            return initializeForAbsoluteX(opcode, value.value());
        default:
            FATAL_ERROR("Wrong OpCode");
        }
    }

    static std::string constructParamName(const testing::TestParamInfo<OpCode> &info) {
        switch (info.param) {
        case OpCode::ASL_acc:
            return "ASL_acc";
        case OpCode::ASL_z:
            return "ASL_z";
        case OpCode::ASL_zx:
            return "ASL_zx";
        case OpCode::ASL_abs:
            return "ASL_abs";
        case OpCode::ASL_absx:
            return "ASL_absx";
        default:
            FATAL_ERROR("Wrong OpCode");
        }
    }
};

TEST_P(AslTest, givenNumberThenShiftProperly) {
    const u8 inpValue = 0b00110011;
    const u8 outValue = 0b01100110;
    ReferencedValue referencedValue = initializeProcessor(GetParam(), inpValue, {});

    flags.expectCarryFlag(false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);

    EXPECT_EQ(outValue, referencedValue.read());
}

TEST_P(AslTest, givenMostSignificantBitSetThenSetCarryBit) {
    const u8 inpValue = 0b10110011;
    const u8 outValue = 0b01100110;
    ReferencedValue referencedValue = initializeProcessor(GetParam(), inpValue, {});

    flags.expectCarryFlag(true);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);

    EXPECT_EQ(outValue, referencedValue.read());
}

TEST_P(AslTest, givenNegativeResultThenSetNegativeBit) {
    const u8 inpValue = 0b01110011;
    const u8 outValue = 0b11100110;
    ReferencedValue referencedValue = initializeProcessor(GetParam(), inpValue, {});

    flags.expectCarryFlag(false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);
    processor.executeInstructions(1);

    EXPECT_EQ(outValue, referencedValue.read());
}

TEST_P(AslTest, givenZeroValueThenZeroFlagSet) {
    const u8 inpValue = 0b00000000;
    const u8 outValue = 0b00000000;
    ReferencedValue referencedValue = initializeProcessor(GetParam(), inpValue, {});

    flags.expectCarryFlag(false);
    flags.expectZeroFlag(true);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);

    EXPECT_EQ(outValue, referencedValue.read());
}

TEST_P(AslTest, givenZeroValueAfterShiftThenZeroFlagSet) {
    const u8 inpValue = 0b10000000;
    const u8 outValue = 0b00000000;
    ReferencedValue referencedValue = initializeProcessor(GetParam(), inpValue, {});

    flags.expectCarryFlag(true);
    flags.expectZeroFlag(true);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);

    EXPECT_EQ(outValue, referencedValue.read());
}

INSTANTIATE_TEST_SUITE_P(,
                         AslTest,
                         ::testing::ValuesIn({OpCode::ASL_acc, OpCode::ASL_z, OpCode::ASL_zx, OpCode::ASL_abs, OpCode::ASL_absx}),
                         AslTest::constructParamName);
