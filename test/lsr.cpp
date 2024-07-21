#include "src/error.h"
#include "test/fixtures/emos_test.h"

#include <tuple>

struct LsrTest : testing::WithParamInterface<OpCode>, EmosTest {
    ReferencedValue initializeProcessor(OpCode opcode, std::optional<u8> value, [[maybe_unused]] std::optional<u8> loadToReg) {
        switch (opcode) {
        case OpCode::LSR_acc:
            expectedBytesProcessed = 1u;
            expectedCyclesProcessed = 2u;
            return initializeForAccumulator(opcode, value.value());
        case OpCode::LSR_z:
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 5u;
            return initializeForZeroPage(opcode, value.value());
        case OpCode::LSR_zx:
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 6u;
            return initializeForZeroPageX(opcode, value.value());
        case OpCode::LSR_abs:
            expectedBytesProcessed = 3u;
            expectedCyclesProcessed = 6u;
            return initializeForAbsolute(opcode, value.value());
        case OpCode::LSR_absx:
            expectedBytesProcessed = 3u;
            expectedCyclesProcessed = 7u;
            return initializeForAbsoluteX(opcode, value.value());
        default:
            FATAL_ERROR("Wrong OpCode");
        }
    }

    static std::string constructParamName(const testing::TestParamInfo<OpCode> &info) {
        switch (info.param) {
        case OpCode::LSR_acc:
            return "LSR_acc";
        case OpCode::LSR_z:
            return "LSR_z";
        case OpCode::LSR_zx:
            return "LSR_zx";
        case OpCode::LSR_abs:
            return "LSR_abs";
        case OpCode::LSR_absx:
            return "LSR_absx";
        default:
            FATAL_ERROR("Wrong OpCode");
        }
    }
};

TEST_P(LsrTest, givenNumberThenShiftProperly) {
    const u8 inpValue = 0b00110010;
    const u8 outValue = 0b00011001;
    ReferencedValue referencedValue = initializeProcessor(GetParam(), inpValue, {});

    flags.expectCarryFlag(false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);

    EXPECT_EQ(outValue, referencedValue.read());
}

TEST_P(LsrTest, givenLeastSignificantBitSetThenSetCarryBit) {
    const u8 inpValue = 0b00010001;
    const u8 outValue = 0b00001000;
    ReferencedValue referencedValue = initializeProcessor(GetParam(), inpValue, {});

    flags.expectCarryFlag(true);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);

    EXPECT_EQ(outValue, referencedValue.read());
}

TEST_P(LsrTest, givenZeroValueThenZeroFlagSet) {
    const u8 inpValue = 0b00000000;
    const u8 outValue = 0b00000000;
    ReferencedValue referencedValue = initializeProcessor(GetParam(), inpValue, {});

    flags.expectCarryFlag(false);
    flags.expectZeroFlag(true);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);

    EXPECT_EQ(outValue, referencedValue.read());
}

TEST_P(LsrTest, givenZeroValueAfterShiftThenZeroFlagSet) {
    const u8 inpValue = 0b00000001;
    const u8 outValue = 0b00000000;
    ReferencedValue referencedValue = initializeProcessor(GetParam(), inpValue, {});

    flags.expectCarryFlag(true);
    flags.expectZeroFlag(true);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);

    EXPECT_EQ(outValue, referencedValue.read());
}

INSTANTIATE_TEST_SUITE_P(,
                         LsrTest,
                         ::testing::ValuesIn({OpCode::LSR_acc, OpCode::LSR_z, OpCode::LSR_zx, OpCode::LSR_abs, OpCode::LSR_absx}),
                         LsrTest::constructParamName);
