#include "src/error.h"
#include "test/fixtures/emos_test.h"

struct AdcTest : testing::WithParamInterface<OpCode>, EmosTest {
    void initializeProcessor(OpCode opcode, std::optional<u8> addend, std::optional<u8> regA) override {
        processor.regs.a = regA.value();
        switch (opcode) {
        case OpCode::ADC_imm:
            initializeForImmediate(opcode, addend.value());
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 2u;
            return;
        case OpCode::ADC_z:
            initializeForZeroPage(opcode, addend.value());
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 3u;
            return;
        case OpCode::ADC_zx:
            initializeForZeroPageX(opcode, addend.value());
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 4u;
            return;
        case OpCode::ADC_abs:
            initializeForAbsolute(opcode, addend.value());
            expectedBytesProcessed = 3u;
            expectedCyclesProcessed = 4u;
            return;
        case OpCode::ADC_absx:
            initializeForAbsoluteX(opcode, addend.value());
            expectedBytesProcessed = 3u;
            expectedCyclesProcessed = 4u;
            return;
        case OpCode::ADC_absy:
            initializeForAbsoluteY(opcode, addend.value());
            expectedBytesProcessed = 3u;
            expectedCyclesProcessed = 4u;
            return;
        case OpCode::ADC_ix:
            initializeForIndirectX(opcode, addend.value());
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 6u;
            return;
        case OpCode::ADC_iy:
            initializeForIndirectY(opcode, addend.value());
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 5u;
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
        default:
            FATAL_ERROR("Wrong OpCode");
        }
    }
    struct ParamsAdcTests {
        OpCode opcode;
        u8 regA;
        u8 addend;
        u8 result;
    };
    void runAdcTest(ParamsAdcTests &params) {
        initializeProcessor(params.opcode, params.addend, params.regA);
        processor.executeInstructions(1);
        EXPECT_EQ(expectedBytesProcessed, processor.counters.bytesProcessed);
        EXPECT_EQ(expectedCyclesProcessed, processor.counters.cyclesProcessed);
        EXPECT_EQ(params.result, processor.regs.a);
    }
};

TEST_P(AdcTest, givenTwoValuesThatSumSetBit7WhenCarryNotSetThenOverflowAndNegativeFlagsSet) {
    flags.expectCarryFlag(false, false);
    flags.expectOverflowFlag(true);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);

    ParamsAdcTests params{};
    params.opcode = GetParam();
    params.regA = 0b0100'0000;   // 64
    params.addend = 0b0100'0000; // 64
    params.result = 0b1000'0000; // 128
    runAdcTest(params);
}

TEST_P(AdcTest, givenTwoZeroWhenCarryNotSetThenZeroFlagSet) {
    flags.expectCarryFlag(false, false);
    flags.expectOverflowFlag(false);
    flags.expectZeroFlag(true);
    flags.expectNegativeFlag(false);

    ParamsAdcTests params{};
    params.opcode = GetParam();
    params.regA = 0b0000'0000;   // 0
    params.addend = 0b0000'0000; // 0
    params.result = 0b0000'0000; // 0
    runAdcTest(params);
}

TEST_P(AdcTest, givenTwoZeroWhenCarrySetThenZeroFlagSet) {
    flags.expectCarryFlag(true, false);
    flags.expectOverflowFlag(false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);

    ParamsAdcTests params{};
    params.opcode = GetParam();
    params.regA = 0b0000'0000;   // 0
    params.addend = 0b0000'0000; // 0
    params.result = 0b0000'0001; // 0
    runAdcTest(params);
}

TEST_P(AdcTest, givenTwoNegativeValueWhenCarryNotSetThenCarryAndNegativeFlagSet) {
    flags.expectCarryFlag(false, true);
    flags.expectOverflowFlag(false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);

    ParamsAdcTests params{};
    params.opcode = GetParam();
    params.regA = 0b1111'0000;   // -16
    params.addend = 0b1110'1010; // -22
    params.result = 0b1101'1010; // -38
    runAdcTest(params);
}

TEST_P(AdcTest, givenTwoPositiveValuesWhenCarryNotSetThenOverflowAndNegativFlagsSet) {
    flags.expectCarryFlag(false, false);
    flags.expectOverflowFlag(true);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);

    ParamsAdcTests params{};
    params.opcode = GetParam();
    params.regA = 0b0111'1111;   // 127
    params.addend = 0b0000'0001; // 1
    params.result = 0b1000'0000; // 128
    runAdcTest(params);
}

TEST_P(AdcTest, givenTwoPositiveValuesWhenCarrySetThenNoFlagsSet) {
    flags.expectCarryFlag(true, false);
    flags.expectOverflowFlag(false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);

    ParamsAdcTests params{};
    params.opcode = GetParam();
    params.regA = 0b0000'0011;   // 3
    params.addend = 0b0000'0001; // 1
    params.result = 0b0000'0101; // 5
    runAdcTest(params);
}

TEST_P(AdcTest, givenTwoValuesWithDifferenSignWhenCarrySetThenCarryFlagSet) {
    flags.expectCarryFlag(true, true);
    flags.expectOverflowFlag(false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);

    ParamsAdcTests params{};
    params.opcode = GetParam();
    params.regA = 0b0011'1000;   // 56
    params.addend = 0b1111'0010; // -14
    params.result = 0b0010'1011; // 43
    runAdcTest(params);
}

TEST_P(AdcTest, givenTwoSameValuesWithDifferenSignWhenCarryNoSetThenZeroAndCarrySet) {
    flags.expectCarryFlag(false, true);
    flags.expectOverflowFlag(false);
    flags.expectZeroFlag(true);
    flags.expectNegativeFlag(false);

    ParamsAdcTests params{};
    params.opcode = GetParam();
    params.regA = 0b1111'1101;   // -3
    params.addend = 0b0000'0011; // 3
    params.result = 0b0000'0000; // 0
    runAdcTest(params);
}

TEST_P(AdcTest, givenZeroAndNegativeValueWhenCarryNotSetThenNegativeFlagSet) {
    flags.expectCarryFlag(false, false);
    flags.expectOverflowFlag(false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);

    ParamsAdcTests params{};
    params.opcode = GetParam();
    params.regA = 0b0000'0000;   // 0
    params.addend = 0b1111'1111; // -1
    params.result = 0b1111'1111; // -1
    runAdcTest(params);
}

TEST_P(AdcTest, givenTwoNegativeValuesWhenCarryNotSetThenCarryAndOverflowFlagSet) {
    flags.expectCarryFlag(false, true);
    flags.expectOverflowFlag(true);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);

    ParamsAdcTests params{};
    params.opcode = GetParam();
    params.regA = 0b1000'0000;   // -128
    params.addend = 0b1111'1111; // -1
    params.result = 0b0111'1111; // 127
    runAdcTest(params);
}

TEST_P(AdcTest, givenTwoNegativeValuesWhenCarrySetThenCarryAndOverflowFlagSet) {
    flags.expectCarryFlag(true, true);
    flags.expectOverflowFlag(false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);

    ParamsAdcTests params{};
    params.opcode = GetParam();
    params.regA = 0b1000'0000;   // -128
    params.addend = 0b1111'1111; // -1
    params.result = 0b1000'0000; // -128
    runAdcTest(params);
}

OpCode opcodesAdc[] = {OpCode::ADC_imm, OpCode::ADC_z, OpCode::ADC_zx, OpCode::ADC_abs, OpCode::ADC_absx, OpCode::ADC_absy, OpCode::ADC_ix, OpCode::ADC_iy};

INSTANTIATE_TEST_SUITE_P(, AdcTest,
                         ::testing::ValuesIn(opcodesAdc), AdcTest::constructParamName);