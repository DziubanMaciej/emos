#include "src/error.h"
#include "unit_test/fixtures/emos_test.h"

struct AdcSbcTest : testing::WithParamInterface<OpCode>, EmosTest {
    void initializeProcessor(OpCode opcode, std::optional<u8> memValue, std::optional<u8> regA) {
        processor.regs.a = regA.value();
        switch (opcode) {
        case OpCode::SBC_imm:
        case OpCode::ADC_imm:
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 2u;
            initializeForImmediate(opcode, memValue.value());
            return;
        case OpCode::SBC_z:
        case OpCode::ADC_z:
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 3u;
            initializeForZeroPage(opcode, memValue.value());
            return;
        case OpCode::SBC_zx:
        case OpCode::ADC_zx:
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 4u;
            initializeForZeroPageX(opcode, memValue.value());
            return;
        case OpCode::SBC_abs:
        case OpCode::ADC_abs:
            expectedBytesProcessed = 3u;
            expectedCyclesProcessed = 4u;
            initializeForAbsolute(opcode, memValue.value());
            return;
        case OpCode::SBC_absx:
        case OpCode::ADC_absx:
            expectedBytesProcessed = 3u;
            expectedCyclesProcessed = 4u;
            initializeForAbsoluteX(opcode, memValue.value());
            return;
        case OpCode::SBC_absy:
        case OpCode::ADC_absy:
            expectedBytesProcessed = 3u;
            expectedCyclesProcessed = 4u;
            initializeForAbsoluteY(opcode, memValue.value());
            return;
        case OpCode::SBC_ix:
        case OpCode::ADC_ix:
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 6u;
            initializeForIndirectX(opcode, memValue.value());
            return;
        case OpCode::SBC_iy:
        case OpCode::ADC_iy:
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 5u;
            initializeForIndirectY(opcode, memValue.value());
            return;
        default:
            FATAL_ERROR("Wrong OpCode");
        }
    }
    static std::string constructParamName(const testing::TestParamInfo<OpCode> &info) {
        OpCode opCode = info.param;
        switch (opCode) {
        case OpCode::ADC_imm:
            return "ADC_imm";
        case OpCode::ADC_z:
            return "ADC_z";
        case OpCode::ADC_zx:
            return "ADC_zx";
        case OpCode::ADC_abs:
            return "ADC_abs";
        case OpCode::ADC_absx:
            return "ADC_absx";
        case OpCode::ADC_absy:
            return "ADC_absy";
        case OpCode::ADC_ix:
            return "ADC_ix";
        case OpCode::ADC_iy:
            return "ADC_iy";
        case OpCode::SBC_imm:
            return "SBC_imm";
        case OpCode::SBC_z:
            return "SBC_z";
        case OpCode::SBC_zx:
            return "SBC_zx";
        case OpCode::SBC_abs:
            return "SBC_abs";
        case OpCode::SBC_absx:
            return "SBC_absx";
        case OpCode::SBC_absy:
            return "SBC_absy";
        case OpCode::SBC_ix:
            return "SBC_ix";
        case OpCode::SBC_iy:
            return "SBC_iy";
        default:
            FATAL_ERROR("Wrong OpCode");
        }
    }
    struct ParamsAdcSbcTests {
        OpCode opcode;
        u8 regA;
        u8 memValue;
        u8 result;
    };
    void runAdcSbcTest(ParamsAdcSbcTests &params) {
        initializeProcessor(params.opcode, params.memValue, params.regA);
        processor.executeInstructions(1);
        EXPECT_EQ(expectedBytesProcessed, processor.counters.bytesProcessed);
        EXPECT_EQ(expectedCyclesProcessed, processor.counters.cyclesProcessed);
        EXPECT_EQ(params.result, processor.regs.a);
    }
};

using AdcTest = AdcSbcTest;
TEST_P(AdcTest, givenTwoValuesThatSumSetBit7WhenCarryNotSetThenOverflowAndNegativeFlagsSet) {
    flags.expectCarryFlag(false, false);
    flags.expectOverflowFlag(true);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);

    ParamsAdcSbcTests params{};
    params.opcode = GetParam();
    params.regA = 0b0100'0000;     // 64
    params.memValue = 0b0100'0000; // 64
    params.result = 0b1000'0000;   // 128
    runAdcSbcTest(params);
}

TEST_P(AdcTest, givenTwoZeroWhenCarryNotSetThenZeroFlagSet) {
    flags.expectCarryFlag(false, false);
    flags.expectOverflowFlag(false);
    flags.expectZeroFlag(true);
    flags.expectNegativeFlag(false);

    ParamsAdcSbcTests params{};
    params.opcode = GetParam();
    params.regA = 0b0000'0000;     // 0
    params.memValue = 0b0000'0000; // 0
    params.result = 0b0000'0000;   // 0
    runAdcSbcTest(params);
}

TEST_P(AdcTest, givenTwoZeroWhenCarrySetThenOne) {
    flags.expectCarryFlag(true, false);
    flags.expectOverflowFlag(false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);

    ParamsAdcSbcTests params{};
    params.opcode = GetParam();
    params.regA = 0b0000'0000;     // 0
    params.memValue = 0b0000'0000; // 0
    params.result = 0b0000'0001;   // 1
    runAdcSbcTest(params);
}

TEST_P(AdcTest, givenTwoNegativeValueWhenCarryNotSetThenCarryAndNegativeFlagSet) {
    flags.expectCarryFlag(false, true);
    flags.expectOverflowFlag(false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);

    ParamsAdcSbcTests params{};
    params.opcode = GetParam();
    params.regA = 0b1111'0000;     // -16
    params.memValue = 0b1110'1010; // -22
    params.result = 0b1101'1010;   // -38
    runAdcSbcTest(params);
}

TEST_P(AdcTest, givenTwoPositiveValuesWhenCarryNotSetThenOverflowAndNegativeFlagsSet) {
    flags.expectCarryFlag(false, false);
    flags.expectOverflowFlag(true);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);

    ParamsAdcSbcTests params{};
    params.opcode = GetParam();
    params.regA = 0b0111'1111;     // 127
    params.memValue = 0b0000'0001; // 1
    params.result = 0b1000'0000;   // 128
    runAdcSbcTest(params);
}

TEST_P(AdcTest, givenTwoPositiveValuesWhenCarrySetThenNoFlagsSet) {
    flags.expectCarryFlag(true, false);
    flags.expectOverflowFlag(false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);

    ParamsAdcSbcTests params{};
    params.opcode = GetParam();
    params.regA = 0b0000'0011;     // 3
    params.memValue = 0b0000'0001; // 1
    params.result = 0b0000'0101;   // 5
    runAdcSbcTest(params);
}

TEST_P(AdcTest, givenTwoValuesWithDifferenSignWhenCarrySetThenCarryFlagSet) {
    flags.expectCarryFlag(true, true);
    flags.expectOverflowFlag(false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);

    ParamsAdcSbcTests params{};
    params.opcode = GetParam();
    params.regA = 0b0011'1000;     // 56
    params.memValue = 0b1111'0010; // -14
    params.result = 0b0010'1011;   // 43
    runAdcSbcTest(params);
}

TEST_P(AdcTest, givenTwoSameValuesWithDifferenSignWhenCarryNoSetThenZeroAndCarrySet) {
    flags.expectCarryFlag(false, true);
    flags.expectOverflowFlag(false);
    flags.expectZeroFlag(true);
    flags.expectNegativeFlag(false);

    ParamsAdcSbcTests params{};
    params.opcode = GetParam();
    params.regA = 0b1111'1101;     // -3
    params.memValue = 0b0000'0011; // 3
    params.result = 0b0000'0000;   // 0
    runAdcSbcTest(params);
}

TEST_P(AdcTest, givenZeroAndNegativeValueWhenCarryNotSetThenNegativeFlagSet) {
    flags.expectCarryFlag(false, false);
    flags.expectOverflowFlag(false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);

    ParamsAdcSbcTests params{};
    params.opcode = GetParam();
    params.regA = 0b0000'0000;     // 0
    params.memValue = 0b1111'1111; // -1
    params.result = 0b1111'1111;   // -1
    runAdcSbcTest(params);
}

TEST_P(AdcTest, givenTwoNegativeValuesWhenCarryNotSetThenCarryAndOverflowFlagSet) {
    flags.expectCarryFlag(false, true);
    flags.expectOverflowFlag(true);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);

    ParamsAdcSbcTests params{};
    params.opcode = GetParam();
    params.regA = 0b1000'0000;     // -128
    params.memValue = 0b1111'1111; // -1
    params.result = 0b0111'1111;   // 127
    runAdcSbcTest(params);
}

TEST_P(AdcTest, givenTwoNegativeValuesWhenCarrySetThenCarryAndOverflowFlagSet) {
    flags.expectCarryFlag(true, true);
    flags.expectOverflowFlag(false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);

    ParamsAdcSbcTests params{};
    params.opcode = GetParam();
    params.regA = 0b1000'0000;     // -128
    params.memValue = 0b1111'1111; // -1
    params.result = 0b1000'0000;   // -128
    runAdcSbcTest(params);
}

TEST_P(AdcTest, givenAllSetBitInAccumulatorAndZeroInMemoryWhenCarryFlagSetThenCarryAndZeroFlagSet) {
    flags.expectCarryFlag(true, true);
    flags.expectOverflowFlag(false);
    flags.expectZeroFlag(true);
    flags.expectNegativeFlag(false);

    ParamsAdcSbcTests params{};
    params.opcode = GetParam();
    params.regA = 0b1111'1111;     // -1
    params.memValue = 0b0000'0000; // 0
    params.result = 0b0000'0000;   // 0
    runAdcSbcTest(params);
}

OpCode opcodesAdc[] = {OpCode::ADC_imm, OpCode::ADC_z, OpCode::ADC_zx, OpCode::ADC_abs, OpCode::ADC_absx, OpCode::ADC_absy, OpCode::ADC_ix, OpCode::ADC_iy};

INSTANTIATE_TEST_SUITE_P(, AdcTest,
                         ::testing::ValuesIn(opcodesAdc), AdcSbcTest::constructParamName);

using SbcTest = AdcSbcTest;
TEST_P(SbcTest, givenTwoValuesThatSubstactSetBit7WhenCarryNotSetThenOverflowAndNegativeFlagsSet) {
    flags.expectCarryFlag(false, true);
    flags.expectOverflowFlag(true);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);

    ParamsAdcSbcTests params{};
    params.opcode = GetParam();
    params.regA = 0b0100'0000;     // 64
    params.memValue = 0b1011'1111; // 191
    params.result = 0b1000'0000;   // 64 - 191 - (1-0) = -127
    runAdcSbcTest(params);
}

TEST_P(SbcTest, givenTwoZeroWhenCarryNotSetThenNotZero) {
    flags.expectCarryFlag(false, true);
    flags.expectOverflowFlag(false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    ParamsAdcSbcTests params{};
    params.opcode = GetParam();
    params.regA = 0b0000'0000;     // 0
    params.memValue = 0b0000'0000; // 0
    params.result = 0b1111'1111;   // 0 - 0 - (1-0) = -1
    runAdcSbcTest(params);
}

TEST_P(SbcTest, givenTwoZeroWhenCarrySetThenZeroFlagSet) {
    flags.expectCarryFlag(true, false);
    flags.expectOverflowFlag(false);
    flags.expectZeroFlag(true);
    flags.expectNegativeFlag(false);
    ParamsAdcSbcTests params{};

    params.opcode = GetParam();
    params.regA = 0b0000'0000;     // 0
    params.memValue = 0b0000'0000; // 0
    params.result = 0b0000'0000;   // 0 - 0 - (1-1) = 0
    runAdcSbcTest(params);
}

TEST_P(SbcTest, givenPositiveValueWhenSubstractNegativeFlagIfCarryNotSetThenCarryAndNegativeFlagSet) {
    flags.expectCarryFlag(false, false);
    flags.expectOverflowFlag(false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);
    ParamsAdcSbcTests params{};

    params.opcode = GetParam();
    params.regA = 0b1111'0000;     // -16
    params.memValue = 0b0001'0101; // 21
    params.result = 0b1101'1010;   // -16 - 21 - (1-0) = -38
    runAdcSbcTest(params);
}

TEST_P(SbcTest, givenTwoPositiveValuesWhenCarryNotSetThenOverflowAndCarryFlagsSet) {

    flags.expectCarryFlag(false, false);
    flags.expectOverflowFlag(true);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    ParamsAdcSbcTests params{};

    params.opcode = GetParam();
    params.regA = 0b1000'0000;     // 128 or -128
    params.memValue = 0b0000'0001; // 1
    params.result = 0b0111'1110;   //  128 - 1 - (1-0) =  126
    runAdcSbcTest(params);
}

TEST_P(SbcTest, givenTwoPositiveValuesWhenCarrySetThenClearCarryFlag) {
    flags.expectCarryFlag(true, false);
    flags.expectOverflowFlag(false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    ParamsAdcSbcTests params{};

    params.opcode = GetParam();
    params.regA = 0b0000'0011;     // 3
    params.memValue = 0b0000'0001; // 1
    params.result = 0b0000'0010;   // 3 - 1 - (1-1) = 2
    runAdcSbcTest(params);
}

TEST_P(SbcTest, givenTwoValuesWithDifferenSignWhenCarrySetThenBit7OverflowAndCarryFlagClear) {

    flags.expectCarryFlag(true, false);
    flags.expectOverflowFlag(false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    ParamsAdcSbcTests params{};

    params.opcode = GetParam();
    params.regA = 0b0011'1000;     // 56
    params.memValue = 0b0000'1101; // 13
    params.result = 0b0010'1011;   // 56 - 13 - (1-1) = 43
    runAdcSbcTest(params);
}

TEST_P(SbcTest, givenTwoSameValuesWithDifferenSignWhenCarryNotSetThenBit7OverflowAndCarryFlagClear) {
    flags.expectCarryFlag(false, false);
    flags.expectOverflowFlag(false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);

    ParamsAdcSbcTests params{};
    params.opcode = GetParam();
    params.regA = 0b1111'1101;     // -3
    params.memValue = 0b0000'0011; // 3
    params.result = 0b1111'1001;   // -3 - 3 - (1-0) = -7

    runAdcSbcTest(params);
}

TEST_P(SbcTest, givenTwoSameValuesWhenCarryNotSetThenMinusOne) {
    flags.expectCarryFlag(false, true);
    flags.expectOverflowFlag(false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);

    ParamsAdcSbcTests params{};
    params.opcode = GetParam();
    params.regA = 0b0000'0011;     // 3
    params.memValue = 0b0000'0011; // 3
    params.result = 0b1111'1111;   // 3 - 3 - (1-0) = -1

    runAdcSbcTest(params);
}

TEST_P(SbcTest, givenZeroAndNegativeValueWhenCarryNotSetThenNegativeFlagSet) {
    flags.expectCarryFlag(false, true);
    flags.expectOverflowFlag(false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);

    ParamsAdcSbcTests params{};
    params.opcode = GetParam();
    params.regA = 0b0000'0000;     // 0
    params.memValue = 0b0000'0001; // 1
    params.result = 0b1111'1110;   //  0 - 1 - (1-0) = -2
    runAdcSbcTest(params);
}

TEST_P(SbcTest, givenTwoNegativeValuesWhenCarryNotSetThenProperResult) {
    flags.expectCarryFlag(false, true);
    flags.expectOverflowFlag(false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);

    ParamsAdcSbcTests params{};
    params.opcode = GetParam();
    params.regA = 0b1000'0000;     // -128
    params.memValue = 0b1111'1110; // -2
    params.result = 0b1000'0001;   // -128 - (-2) - (1-0) = -127
    runAdcSbcTest(params);
}

TEST_P(SbcTest, givenTwoNegativeValuesWhenCarrySetThenProperResult) {
    flags.expectCarryFlag(true, true);
    flags.expectOverflowFlag(false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);

    ParamsAdcSbcTests params{};
    params.opcode = GetParam();
    params.regA = 0b1000'0000;     // -128
    params.memValue = 0b1111'1110; // -1
    params.result = 0b1000'0010;   // -128 - (-1) - (1-1) = -126
    runAdcSbcTest(params);
}

OpCode opcodesSbc[] = {OpCode::SBC_imm, OpCode::SBC_z, OpCode::SBC_zx, OpCode::SBC_abs, OpCode::SBC_absx, OpCode::SBC_absy, OpCode::SBC_ix, OpCode::SBC_iy};

INSTANTIATE_TEST_SUITE_P(, SbcTest,
                         ::testing::ValuesIn(opcodesSbc), AdcSbcTest::constructParamName);