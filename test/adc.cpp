#include "fixtures.h"

#include "src/error.h"

struct AdcTest : testing::WithParamInterface<OpCode>, EmosTest {
    void checkNotAffectedFlags() {
        EXPECT_FALSE(processor.regs.flags.i);
        EXPECT_FALSE(processor.regs.flags.d);
        EXPECT_FALSE(processor.regs.flags.b);
    }

    void initializeProcessor(OpCode opcode, u8 regA, u8 addend) {
        processor.regs.a = regA;
        switch (opcode) {
        case OpCode::ADC_imm:
            initializeForImmediate(OpCode::ADC_imm, addend);
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 2u;
            return;
        case OpCode::ADC_z:
            initializeForZeroPage(OpCode::ADC_z, addend);
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 3u;
            return;
        case OpCode::ADC_zx:
            initializeForZeroPageX(OpCode::ADC_zx, addend);
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 4u;
            return;
        case OpCode::ADC_abs:
            initializeForAbsolute(OpCode::ADC_abs, addend);
            expectedBytesProcessed = 3u;
            expectedCyclesProcessed = 4u;
            return;
        case OpCode::ADC_absx:
            initializeForAbsoluteX(OpCode::ADC_absx, addend);
            expectedBytesProcessed = 3u;
            expectedCyclesProcessed = 4u;
            return;
        case OpCode::ADC_absy:
            initializeForAbsoluteY(OpCode::ADC_absy, addend);
            expectedBytesProcessed = 3u;
            expectedCyclesProcessed = 4u;
            return;
        case OpCode::ADC_ix:
            initializeForIndirectX(OpCode::ADC_ix, addend);
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 6u;
            return;
        case OpCode::ADC_iy:
            initializeForIndirectY(OpCode::ADC_iy, addend);
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
        u8 inCarryFlag;
        u8 expectedOverflowFlag;
        u8 expectedCarryFlag;
        u8 expectedZeroFlag;
        u8 expectedNegativeFlag;
    };
    void runAdcTest(ParamsAdcTests &params) {
        initializeProcessor(params.opcode, params.regA, params.addend);
        processor.regs.flags.c = params.inCarryFlag;
        processor.executeInstructions(1);
        EXPECT_EQ(expectedBytesProcessed, processor.counters.bytesProcessed);
        EXPECT_EQ(expectedCyclesProcessed, processor.counters.cyclesProcessed);

        EXPECT_EQ(params.expectedOverflowFlag, processor.regs.flags.o);
        EXPECT_EQ(params.result, processor.regs.a);
        EXPECT_EQ(params.expectedCarryFlag, processor.regs.flags.c);
        EXPECT_EQ(params.expectedZeroFlag, processor.regs.flags.z);
        EXPECT_EQ(params.expectedNegativeFlag, processor.regs.flags.n);

        checkNotAffectedFlags();
    }
    u32 expectedBytesProcessed = 0u;
    u32 expectedCyclesProcessed = 0u;
};

TEST_P(AdcTest, givenTwoValueThatSumSetBit7WhenCarryNotSetThenOverflowAndNegativeFlagsSet) {
    ParamsAdcTests params{};
    params.opcode = GetParam();
    params.regA = 0b0100'0000;   // 64
    params.addend = 0b0100'0000; // 64
    params.result = 0b1000'0000; // 128
    params.inCarryFlag = false;
    params.expectedOverflowFlag = true;
    params.expectedCarryFlag = false;
    params.expectedZeroFlag = false;
    params.expectedNegativeFlag = true;
    runAdcTest(params);
}

TEST_P(AdcTest, givenTwoZeroWhenCarryNotSetThenZeroFlagSet) {
    ParamsAdcTests params{};
    params.opcode = GetParam();
    params.regA = 0b0000'0000;   // 0
    params.addend = 0b0000'0000; // 0
    params.result = 0b0000'0000; // 0
    params.inCarryFlag = false;
    params.expectedOverflowFlag = false;
    params.expectedCarryFlag = false;
    params.expectedZeroFlag = true;
    params.expectedNegativeFlag = false;
    runAdcTest(params);
}

TEST_P(AdcTest, givenTwoZeroWhenCarrySetThenZeroFlagSet) {
    ParamsAdcTests params{};
    params.opcode = GetParam();
    params.regA = 0b0000'0000;   // 0
    params.addend = 0b0000'0000; // 0
    params.result = 0b0000'0001; // 0
    params.inCarryFlag = true;
    params.expectedOverflowFlag = false;
    params.expectedCarryFlag = false;
    params.expectedZeroFlag = false;
    params.expectedNegativeFlag = false;
    runAdcTest(params);
}

TEST_P(AdcTest, givenTwoNegativeValueWhenCarryNotSetThenCarryAndNegativeFlagSet) {
    ParamsAdcTests params{};
    params.opcode = GetParam();
    params.regA = 0b1111'0000;   // -16
    params.addend = 0b1110'1010; // -22
    params.result = 0b1101'1010; // -38
    params.inCarryFlag = false;
    params.expectedOverflowFlag = false;
    params.expectedCarryFlag = true;
    params.expectedZeroFlag = false;
    params.expectedNegativeFlag = true;
    runAdcTest(params);
}

TEST_P(AdcTest, givenTwoPositiveValuesWhenCarryNotSetThenOverflowAndNegativFlagsSet) {
    ParamsAdcTests params{};
    params.opcode = GetParam();
    params.regA = 0b0111'1111;   // 127
    params.addend = 0b0000'0001; // 1
    params.result = 0b1000'0000; // 128
    params.inCarryFlag = false;
    params.expectedOverflowFlag = true;
    params.expectedCarryFlag = false;
    params.expectedZeroFlag = false;
    params.expectedNegativeFlag = true;
    runAdcTest(params);
}

TEST_P(AdcTest, givenTwoPositiveValuesWhenCarrySetThenNoFlagsSet) {
    ParamsAdcTests params{};
    params.opcode = GetParam();
    params.regA = 0b0000'0011;   // 3
    params.addend = 0b0000'0001; // 1
    params.result = 0b0000'0101; // 5
    params.inCarryFlag = true;
    params.expectedOverflowFlag = false;
    params.expectedCarryFlag = false;
    params.expectedZeroFlag = false;
    params.expectedNegativeFlag = false;
    runAdcTest(params);
}

TEST_P(AdcTest, givenTwoValuesWithDifferenSignWhenCarrySetThenCarryFlagSet) {
    ParamsAdcTests params{};
    params.opcode = GetParam();
    params.regA = 0b0011'1000;   // 56
    params.addend = 0b1111'0010; // -14
    params.result = 0b0010'1011; // 43
    params.inCarryFlag = true;
    params.expectedOverflowFlag = false;
    params.expectedCarryFlag = true;
    params.expectedZeroFlag = false;
    params.expectedNegativeFlag = false;
    runAdcTest(params);
}

TEST_P(AdcTest, givenTwoSameValuesWithDifferenSignWhenCarryNoSetThenZeroAndCarrySet) {
    ParamsAdcTests params{};
    params.opcode = GetParam();
    params.regA = 0b1111'1101;   // -3
    params.addend = 0b0000'0011; // 3
    params.result = 0b0000'0000; // 0
    params.inCarryFlag = false;
    params.expectedOverflowFlag = false;
    params.expectedCarryFlag = true;
    params.expectedZeroFlag = true;
    params.expectedNegativeFlag = false;
    runAdcTest(params);
}

TEST_P(AdcTest, givenZeroAndNegativeValueWhenCarryNotSetThenNegativeFlagSet) {
    ParamsAdcTests params{};
    params.opcode = GetParam();
    params.regA = 0b0000'0000;   // 0
    params.addend = 0b1111'1111; // -1
    params.result = 0b1111'1111; // -1
    params.inCarryFlag = false;
    params.expectedOverflowFlag = false;
    params.expectedCarryFlag = false;
    params.expectedZeroFlag = false;
    params.expectedNegativeFlag = true;
    runAdcTest(params);
}

TEST_P(AdcTest, givenTwoNegativeValuesWhenCarryNotSetThenCarryAndOverflowFlagSet) {
    ParamsAdcTests params{};
    params.opcode = GetParam();
    params.regA = 0b1000'0000;   // -128
    params.addend = 0b1111'1111; // -1
    params.result = 0b0111'1111; // 127
    params.inCarryFlag = false;
    params.expectedOverflowFlag = true;
    params.expectedCarryFlag = true;
    params.expectedZeroFlag = false;
    params.expectedNegativeFlag = false;
    runAdcTest(params);
}

TEST_P(AdcTest, givenTwoNegativeValuesWhenCarrySetThenCarryAndOverflowFlagSet) {
    ParamsAdcTests params{};
    params.opcode = GetParam();
    params.regA = 0b1000'0000;   // -128
    params.addend = 0b1111'1111; // -1
    params.result = 0b1000'0000; // -128
    params.inCarryFlag = true;
    params.expectedOverflowFlag = false;
    params.expectedCarryFlag = true;
    params.expectedZeroFlag = false;
    params.expectedNegativeFlag = true;
    runAdcTest(params);
}

OpCode opcodesAdc[] = {OpCode::ADC_imm, OpCode::ADC_z, OpCode::ADC_zx, OpCode::ADC_abs, OpCode::ADC_absx, OpCode::ADC_absy, OpCode::ADC_ix, OpCode::ADC_iy};

INSTANTIATE_TEST_SUITE_P(, AdcTest,
                         ::testing::ValuesIn(opcodesAdc), AdcTest::constructParamName);