#include "src/error.h"
#include "unit_test/fixtures/emos_test.h"

struct AndTest : testing::WithParamInterface<OpCode>, EmosTest {
    void initializeProcessor(OpCode opcode, std::optional<u8> value, std::optional<u8> regA) {
        processor.regs.a = regA.value();
        switch (opcode) {
        case OpCode::AND_imm:
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 2u;
            initializeForImmediate(opcode, value.value());
            return;
        case OpCode::AND_z:
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 3u;
            initializeForZeroPage(opcode, value.value());
            return;
        case OpCode::AND_zx:
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 4u;
            initializeForZeroPageX(opcode, value.value());
            return;
        case OpCode::AND_abs:
            expectedBytesProcessed = 3u;
            expectedCyclesProcessed = 4u;
            initializeForAbsolute(opcode, value.value());
            return;
        case OpCode::AND_absx:
            expectedBytesProcessed = 3u;
            expectedCyclesProcessed = 4u;
            initializeForAbsoluteX(opcode, value.value());
            return;
        case OpCode::AND_absy:
            expectedBytesProcessed = 3u;
            expectedCyclesProcessed = 4u;
            initializeForAbsoluteY(opcode, value.value());
            return;
        case OpCode::AND_ix:
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 6u;
            initializeForIndirectX(opcode, value.value());
            return;
        case OpCode::AND_iy:
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
        case OpCode::AND_imm:
            return "AND_imm";
        case OpCode::AND_z:
            return "AND_z";
        case OpCode::AND_zx:
            return "AND_zx";
        case OpCode::AND_abs:
            return "AND_abs";
        case OpCode::AND_absx:
            return "AND_absx";
        case OpCode::AND_absy:
            return "AND_absy";
        case OpCode::AND_ix:
            return "AND_ix";
        case OpCode::AND_iy:
            return "AND_iy";
        default:
            FATAL_ERROR("Wrong OpCode");
        }
    }
};

TEST_P(AndTest, givenDifferentValuesWhenExecuteAndThenAIsZeroAndZeroFlagSet) {
    u8 regA = 0b0110'1101;
    u8 value = 0b1001'0010;
    initializeProcessor(GetParam(), value, regA);

    flags.expectZeroFlag(true);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);

    u8 expectedResult = 0b0000'0000;
    EXPECT_EQ(expectedResult, processor.regs.a);
}

TEST_P(AndTest, givenBit7SetInBothValuesWhenExecuteAndThenNegativeFlagSet) {
    u8 regA = 0b1010'1101;
    u8 value = 0b1001'0010;
    initializeProcessor(GetParam(), value, regA);

    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);
    processor.executeInstructions(1);

    u8 expectedResult = 0b1000'0000;
    EXPECT_EQ(expectedResult, processor.regs.a);
}

TEST_P(AndTest, givenFewSameBitsInBothValuesWhenExecuteAndThenCorrectResult) {
    u8 regA = 0b0101'0001;
    u8 value = 0b0101'0010;
    initializeProcessor(GetParam(), value, regA);

    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);

    u8 expectedResult = 0b0101'0000;
    EXPECT_EQ(expectedResult, processor.regs.a);
}

TEST_P(AndTest, givenFewSameBitsInBothValuesWithBit7SetWhenExecuteAndThenNegativeFlagSetAndCorrectResult) {
    u8 regA = 0b1101'0001;
    u8 value = 0b1101'0010;
    initializeProcessor(GetParam(), value, regA);

    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);
    processor.executeInstructions(1);

    u8 expectedResult = 0b1101'0000;
    EXPECT_EQ(expectedResult, processor.regs.a);
}

OpCode opcodesAnd[] = {OpCode::AND_imm, OpCode::AND_z, OpCode::AND_zx, OpCode::AND_abs, OpCode::AND_absx, OpCode::AND_absy, OpCode::AND_ix, OpCode::AND_iy};

INSTANTIATE_TEST_SUITE_P(, AndTest,
                         ::testing::ValuesIn(opcodesAnd), AndTest::constructParamName);