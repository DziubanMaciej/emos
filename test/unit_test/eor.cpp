#include "src/error.h"
#include "unit_test/fixtures/emos_test.h"

struct EorTest : testing::WithParamInterface<OpCode>, EmosTest {
    void initializeProcessor(OpCode opcode, std::optional<u8> value, std::optional<u8> regA) {
        processor.regs.a = regA.value();
        switch (opcode) {
        case OpCode::EOR_imm:
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 2u;
            initializeForImmediate(opcode, value.value());
            return;
        case OpCode::EOR_z:
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 3u;
            initializeForZeroPage(opcode, value.value());
            return;
        case OpCode::EOR_zx:
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 4u;
            initializeForZeroPageX(opcode, value.value());
            return;
        case OpCode::EOR_abs:
            expectedBytesProcessed = 3u;
            expectedCyclesProcessed = 4u;
            initializeForAbsolute(opcode, value.value());
            return;
        case OpCode::EOR_absx:
            expectedBytesProcessed = 3u;
            expectedCyclesProcessed = 4u;
            initializeForAbsoluteX(opcode, value.value());
            return;
        case OpCode::EOR_absy:
            expectedBytesProcessed = 3u;
            expectedCyclesProcessed = 4u;
            initializeForAbsoluteY(opcode, value.value());
            return;
        case OpCode::EOR_ix:
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 6u;
            initializeForIndirectX(opcode, value.value());
            return;
        case OpCode::EOR_iy:
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 5u;
            initializeForIndirectY(opcode, value.value());
            return;
        default:
            FATAL_ERROR("Wrong OpCode");
        }
    }

    static std::string constructParamName(const testing::TestParamInfo<OpCode> &info) {
        OpCode opCode = info.param;
        switch (opCode) {
        case OpCode::EOR_imm:
            return "EOR_imm";
        case OpCode::EOR_z:
            return "EOR_z";
        case OpCode::EOR_zx:
            return "EOR_zx";
        case OpCode::EOR_abs:
            return "EOR_abs";
        case OpCode::EOR_absx:
            return "EOR_absx";
        case OpCode::EOR_absy:
            return "EOR_absy";
        case OpCode::EOR_ix:
            return "EOR_ix";
        case OpCode::EOR_iy:
            return "EOR_iy";
        default:
            FATAL_ERROR("Wrong OpCode");
        }
    }
};

TEST_P(EorTest, givenSameValuesWhenExecuteEorThenAIsZeroAndZeroFlagSet) {
    u8 regA = 0b0110'1100;
    u8 value = 0b0110'1100;
    initializeProcessor(GetParam(), value, regA);

    flags.expectZeroFlag(true);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);

    u8 expectedResult = 0;
    EXPECT_EQ(expectedResult, processor.regs.a);
}

TEST_P(EorTest, givenBit7IsDifferentInBothValuesWhenExecuteEorThenNegativeFlagSet) {
    u8 regA = 0b1010'1101;
    u8 value = 0b0010'1101;
    initializeProcessor(GetParam(), value, regA);

    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);
    processor.executeInstructions(1);

    u8 expectedResult = 0b1000'0000;
    EXPECT_EQ(expectedResult, processor.regs.a);
}

TEST_P(EorTest, givenFewDifferentBitsInBothValuesWhenExecuteEorThenCorrectResult) {
    u8 regA = 0b0101'0001;
    u8 value = 0b0101'0010;
    initializeProcessor(GetParam(), value, regA);

    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);

    u8 expectedResult = 0b0000'0011;
    EXPECT_EQ(expectedResult, processor.regs.a);
}

OpCode opcodesEor[] = {OpCode::EOR_imm, OpCode::EOR_z, OpCode::EOR_zx, OpCode::EOR_abs, OpCode::EOR_absx, OpCode::EOR_absy, OpCode::EOR_ix, OpCode::EOR_iy};

INSTANTIATE_TEST_SUITE_P(, EorTest,
                         ::testing::ValuesIn(opcodesEor), EorTest::constructParamName);
