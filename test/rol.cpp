#include "src/error.h"
#include "test/fixtures/emos_test.h"

#include <tuple>

struct RolTest : testing::WithParamInterface<OpCode>, EmosTest {
    ReferencedValue initializeProcessor(OpCode opcode, std::optional<u8> value, [[maybe_unused]] std::optional<u8> loadToReg) {
        switch (opcode) {
        case OpCode::ROL_acc:
            expectedBytesProcessed = 1u;
            expectedCyclesProcessed = 2u;
            return initializeForAccumulator(opcode, value.value());
        case OpCode::ROL_z:
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 5u;
            return initializeForZeroPage(opcode, value.value());
        case OpCode::ROL_zx:
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 6u;
            return initializeForZeroPageX(opcode, value.value());
        case OpCode::ROL_abs:
            expectedBytesProcessed = 3u;
            expectedCyclesProcessed = 6u;
            return initializeForAbsolute(opcode, value.value());
        case OpCode::ROL_absx:
            expectedBytesProcessed = 3u;
            expectedCyclesProcessed = 7u;
            return initializeForAbsoluteX(opcode, value.value());
        default:
            FATAL_ERROR("Wrong OpCode");
        }
    }

    static std::string constructParamName(const testing::TestParamInfo<OpCode> &info) {
        switch (info.param) {
        case OpCode::ROL_acc:
            return "ROL_acc";
        case OpCode::ROL_z:
            return "ROL_z";
        case OpCode::ROL_zx:
            return "ROL_zx";
        case OpCode::ROL_abs:
            return "ROL_abs";
        case OpCode::ROL_absx:
            return "ROL_absx";
        default:
            FATAL_ERROR("Wrong OpCode");
        }
    }
};

TEST_P(RolTest, givenNumberThenRotateProperly) {
    const u8 inpValue = 0b00110011;
    const u8 outValue = 0b01100110;
    ReferencedValue referencedValue = initializeProcessor(GetParam(), inpValue, {});

    flags.expectCarryFlag(false, false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);

    EXPECT_EQ(outValue, referencedValue.read());
}

TEST_P(RolTest, givenMostSignificantBitSetThenSetCarryBit) {
    const u8 inpValue = 0b10110011;
    const u8 outValue = 0b01100110;
    ReferencedValue referencedValue = initializeProcessor(GetParam(), inpValue, {});

    flags.expectCarryFlag(false, true);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);

    EXPECT_EQ(outValue, referencedValue.read());
}

TEST_P(RolTest, givenNegativeResultThenSetNegativeBit) {
    const u8 inpValue = 0b01110011;
    const u8 outValue = 0b11100110;
    ReferencedValue referencedValue = initializeProcessor(GetParam(), inpValue, {});

    flags.expectCarryFlag(false, false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);
    processor.executeInstructions(1);

    EXPECT_EQ(outValue, referencedValue.read());
}

TEST_P(RolTest, givenZeroValueThenZeroFlagSet) {
    const u8 inpValue = 0b00000000;
    const u8 outValue = 0b00000000;
    ReferencedValue referencedValue = initializeProcessor(GetParam(), inpValue, {});

    flags.expectCarryFlag(false, false);
    flags.expectZeroFlag(true);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);

    EXPECT_EQ(outValue, referencedValue.read());
}

TEST_P(RolTest, givenZeroValueAfterShiftThenZeroFlagSet) {
    const u8 inpValue = 0b10000000;
    const u8 outValue = 0b00000000;
    ReferencedValue referencedValue = initializeProcessor(GetParam(), inpValue, {});

    flags.expectCarryFlag(false, true);
    flags.expectZeroFlag(true);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);

    EXPECT_EQ(outValue, referencedValue.read());
}

TEST_P(RolTest, givenCarryFlagSetThenRotateIntoLeastSignificantBit) {
    const u8 inpValue = 0b00001110;
    const u8 outValue = 0b00011101;
    ReferencedValue referencedValue = initializeProcessor(GetParam(), inpValue, {});

    flags.expectCarryFlag(true, false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);

    EXPECT_EQ(outValue, referencedValue.read());
}

TEST_P(RolTest, givenCarryFlagAndLeastSignificantBitSetThenRotateIntoLeastSignificantBit) {
    const u8 inpValue = 0b00001111;
    const u8 outValue = 0b00011111;
    ReferencedValue referencedValue = initializeProcessor(GetParam(), inpValue, {});

    flags.expectCarryFlag(true, false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);

    EXPECT_EQ(outValue, referencedValue.read());
}

TEST_P(RolTest, givenCarryFlagAndMostSignificantBitSetThenRotateThroughCarry) {
    const u8 inpValue = 0b10000000;
    const u8 outValue = 0b00000001;
    ReferencedValue referencedValue = initializeProcessor(GetParam(), inpValue, {});

    flags.expectCarryFlag(true, true);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);

    EXPECT_EQ(outValue, referencedValue.read());
}

INSTANTIATE_TEST_SUITE_P(,
                         RolTest,
                         ::testing::ValuesIn({OpCode::ROL_acc, OpCode::ROL_z, OpCode::ROL_zx, OpCode::ROL_abs, OpCode::ROL_absx}),
                         RolTest::constructParamName);
