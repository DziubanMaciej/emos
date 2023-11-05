#include "src/error.h"
#include "test/fixtures/emos_test.h"

#include <tuple>

struct LsrTest : testing::WithParamInterface<OpCode>, EmosTest {
    void initializeProcessor(OpCode opcode, std::optional<u8> value, [[maybe_unused]] std::optional<u8> loadToReg) override {
        switch (opcode) {
        case OpCode::LSR_acc:
            initializeForAccumulator(opcode, value.value());
            expectedBytesProcessed = 1u;
            expectedCyclesProcessed = 2u;
            return;
        case OpCode::LSR_z:
            initializeForZeroPage(opcode, value.value());
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 5u;
            return;
        case OpCode::LSR_zx:
            initializeForZeroPageX(opcode, value.value());
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 6u;
            return;
        case OpCode::LSR_abs:
            initializeForAbsolute(opcode, value.value());
            expectedBytesProcessed = 3u;
            expectedCyclesProcessed = 6u;
            return;
        case OpCode::LSR_absx:
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
    initializeProcessor(GetParam(), inpValue, {});

    flags.expectCarryFlag(false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);

    EXPECT_EQ(outValue, referencedValue.read());
}

TEST_P(LsrTest, givenLeastSignificantBitSetThenSetCarryBit) {
    const u8 inpValue = 0b00010001;
    const u8 outValue = 0b00001000;
    initializeProcessor(GetParam(), inpValue, {});

    flags.expectCarryFlag(true);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);

    EXPECT_EQ(outValue, referencedValue.read());
}

TEST_P(LsrTest, givenZeroValueThenZeroFlagSet) {
    const u8 inpValue = 0b00000000;
    const u8 outValue = 0b00000000;
    initializeProcessor(GetParam(), inpValue, {});

    flags.expectCarryFlag(false);
    flags.expectZeroFlag(true);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);

    EXPECT_EQ(outValue, referencedValue.read());
}

TEST_P(LsrTest, givenZeroValueAfterShiftThenZeroFlagSet) {
    const u8 inpValue = 0b00000001;
    const u8 outValue = 0b00000000;
    initializeProcessor(GetParam(), inpValue, {});

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
