#include "src/error.h"
#include "unit_test/fixtures/emos_test.h"

struct OraTest : testing::WithParamInterface<OpCode>, EmosTest {
    void initializeProcessor(OpCode opcode, std::optional<u8> value, std::optional<u8> loadToReg) {
        processor.regs.a = loadToReg.value();
        switch (opcode) {
        case OpCode::ORA_imm:
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 2u;
            initializeForImmediate(OpCode::ORA_imm, value.value());
            return;
        case OpCode::ORA_z:
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 3u;
            initializeForZeroPage(OpCode::ORA_z, value.value());
            return;
        case OpCode::ORA_zx:
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 4u;
            initializeForZeroPageX(OpCode::ORA_zx, value.value());
            return;
        case OpCode::ORA_abs:
            expectedBytesProcessed = 3u;
            expectedCyclesProcessed = 4u;
            initializeForAbsolute(OpCode::ORA_abs, value.value());
            return;
        case OpCode::ORA_absx:
            expectedBytesProcessed = 3u;
            expectedCyclesProcessed = 4u;
            initializeForAbsoluteX(OpCode::ORA_absx, value.value());
            return;
        case OpCode::ORA_absy:
            expectedBytesProcessed = 3u;
            expectedCyclesProcessed = 4u;
            initializeForAbsoluteY(OpCode::ORA_absy, value.value());
            return;
        case OpCode::ORA_ix:
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 6u;
            initializeForIndirectX(OpCode::ORA_ix, value.value());
            return;
        case OpCode::ORA_iy:
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 5u;
            initializeForIndirectY(OpCode::ORA_iy, value.value());
            return;
        default:
            FATAL_ERROR("Wrong OpCode");
        }
    }

    static std::string constructParamName(const testing::TestParamInfo<OpCode> &info) {
        OpCode opCode = info.param;
        switch (opCode) {
        case OpCode::ORA_imm:
            return "ORA_imm";
        case OpCode::ORA_z:
            return "ORA_z";
        case OpCode::ORA_zx:
            return "ORA_zx";
        case OpCode::ORA_abs:
            return "ORA_abs";
        case OpCode::ORA_absx:
            return "ORA_absx";
        case OpCode::ORA_absy:
            return "ORA_absy";
        case OpCode::ORA_ix:
            return "ORA_ix";
        case OpCode::ORA_iy:
            return "ORA_iy";
        default:
            FATAL_ERROR("Wrong OpCode");
        }
    }
};

TEST_P(OraTest, givenZeroValuesWhenExecuteOraThenAIsZeroAndZeroFlagSet) {
    u8 regA = 0b0000'0000;
    u8 value = 0b0000'0000;
    initializeProcessor(GetParam(), value, regA);

    flags.expectZeroFlag(true);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);

    u8 expectedResult = 0b0000'0000;
    EXPECT_EQ(expectedResult, processor.regs.a);
}

TEST_P(OraTest, givenBit7SetInBothValuesWhenExecuteOraThenNegativeFlagSet) {
    u8 regA = 0b1000'1101;
    u8 value = 0b1001'0010;
    initializeProcessor(GetParam(), value, regA);

    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);
    processor.executeInstructions(1);

    u8 expectedResult = 0b1001'1111;
    EXPECT_EQ(expectedResult, processor.regs.a);
}

TEST_P(OraTest, givenFewSameBitsInBothValuesWhenExecuteOraThenCorrectResult) {
    u8 regA = 0b0101'0001;
    u8 value = 0b0101'0010;
    initializeProcessor(GetParam(), value, regA);

    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);

    u8 expectedResult = 0b0101'0011;
    EXPECT_EQ(expectedResult, processor.regs.a);
}

TEST_P(OraTest, givenValuesWithBit7SetWhenExecuteOraThenNegativeFlagSetAndCorrectResult) {
    u8 regA = 0b1101'0001;
    u8 value = 0b1101'0010;
    initializeProcessor(GetParam(), value, regA);

    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);
    processor.executeInstructions(1);

    u8 expectedResult = 0b1101'0011;
    EXPECT_EQ(expectedResult, processor.regs.a);
}

OpCode opcodesOra[] = {OpCode::ORA_imm, OpCode::ORA_z, OpCode::ORA_zx, OpCode::ORA_abs, OpCode::ORA_absx, OpCode::ORA_absy, OpCode::ORA_ix, OpCode::ORA_iy};

INSTANTIATE_TEST_SUITE_P(, OraTest,
                         ::testing::ValuesIn(opcodesOra), OraTest::constructParamName);