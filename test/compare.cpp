#include "fixtures.h"

#include "src/error.h"

struct CompareTest : testing::WithParamInterface<OpCode>, EmosTest {
    void initializeProcessor(OpCode opcode, std::optional<u8> addend, std::optional<u8> regVal) override {
        setRegisterForOpCode(opcode, regVal.value());
        switch (opcode) {
        case OpCode::CMP_imm:
        case OpCode::CPX_imm:
        case OpCode::CPY_imm:
            initializeForImmediate(opcode, addend.value());
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 2u;
            return;
        case OpCode::CMP_z:
        case OpCode::CPX_z:
        case OpCode::CPY_z:
            initializeForZeroPage(opcode, addend.value());
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 3u;
            return;
        case OpCode::CMP_abs:
        case OpCode::CPX_abs:
        case OpCode::CPY_abs:
            initializeForAbsolute(opcode, addend.value());
            expectedBytesProcessed = 3u;
            expectedCyclesProcessed = 4u;
            return;
        case OpCode::CMP_zx:
            initializeForZeroPageX(opcode, addend.value());
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 4u;
            return;
        case OpCode::CMP_absx:
            initializeForAbsoluteX(opcode, addend.value());
            expectedBytesProcessed = 3u;
            expectedCyclesProcessed = 4u;
            return;
        case OpCode::CMP_absy:
            initializeForAbsoluteY(opcode, addend.value());
            expectedBytesProcessed = 3u;
            expectedCyclesProcessed = 4u;
            return;
        case OpCode::CMP_ix:
            initializeForIndirectX(opcode, addend.value());
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 6u;
            return;
        case OpCode::CMP_iy:
            initializeForIndirectY(opcode, addend.value());
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 5u;
            return;
        default:
            FATAL_ERROR("Wrong OpCode");
        }
    }
    void setRegisterForOpCode(OpCode opCode, u8 value) {
        switch (opCode) {
        case OpCode::CMP_imm:
        case OpCode::CMP_z:
        case OpCode::CMP_abs:
        case OpCode::CMP_zx:
        case OpCode::CMP_absx:
        case OpCode::CMP_absy:
        case OpCode::CMP_ix:
        case OpCode::CMP_iy:
            processor.regs.a = value;
            break;
        case OpCode::CPX_imm:
        case OpCode::CPX_z:
        case OpCode::CPX_abs:
            processor.regs.x = value;
            break;
        case OpCode::CPY_imm:
        case OpCode::CPY_z:
        case OpCode::CPY_abs:
            processor.regs.y = value;
            break;
        default:
            break;
        }
    }
    static std::string constructParamName(const testing::TestParamInfo<OpCode> &info) {
        OpCode opCode = info.param;
        switch (opCode) {
        case OpCode::CMP_imm:
            return "CMP_imm";
        case OpCode::CMP_z:
            return "CMP_z";
        case OpCode::CMP_abs:
            return "CMP_abs";
        case OpCode::CPX_imm:
            return "CPX_imm";
        case OpCode::CPX_z:
            return "CPX_z";
        case OpCode::CPX_abs:
            return "CPX_abs";
        case OpCode::CPY_imm:
            return "CPY_imm";
        case OpCode::CPY_z:
            return "CPY_z";
        case OpCode::CPY_abs:
            return "CPY_abs";
        default:
            FATAL_ERROR("Wrong OpCode");
        }
    }
};

using CompareTestImm = CompareTest;

TEST_P(CompareTestImm, givenImmediateModeAndGreaterValueThenProcessInstruction) {
    flags.expectCarryFlag(false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);

    auto opCode = GetParam();
    u8 loadToMem = 0xCD;
    u8 loadToReg = 0xCC;
    initializeProcessor(opCode, loadToMem, loadToReg);

    processor.executeInstructions(1);
}

TEST_P(CompareTestImm, givenImmediateModeAndEqualValueThenProcessInstruction) {
    flags.expectCarryFlag(true);
    flags.expectZeroFlag(true);
    flags.expectNegativeFlag(false);

    auto opCode = GetParam();
    u8 loadToMem = 0xCC;
    u8 loadToReg = 0xCC;
    initializeProcessor(opCode, loadToMem, loadToReg);

    processor.executeInstructions(1);
}

TEST_P(CompareTestImm, givenImmediateModeAndLessValueThenProcessInstruction) {
    flags.expectCarryFlag(true);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);

    auto opCode = GetParam();
    u8 loadToMem = 0xCB;
    u8 loadToReg = 0xCC;
    initializeProcessor(opCode, loadToMem, loadToReg);

    processor.executeInstructions(1);
}

OpCode opcodesImm[] = {OpCode::CMP_imm, OpCode::CPX_imm, OpCode::CPY_imm};

INSTANTIATE_TEST_SUITE_P(, CompareTestImm,
                         ::testing::ValuesIn(opcodesImm), CompareTestImm::constructParamName);

using CompareTestZ = CompareTest;
TEST_P(CompareTestZ, givenZeroPageModeThenProcessInstruction) {
    flags.expectCarryFlag(false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);

    auto opCode = GetParam();
    u8 loadToMem = 0xCD;
    u8 loadToReg = 0xCC;
    initializeProcessor(opCode, loadToMem, loadToReg);

    processor.executeInstructions(1);
}

OpCode opcodesZ[] = {OpCode::CMP_z, OpCode::CPX_z, OpCode::CPY_z};

INSTANTIATE_TEST_SUITE_P(, CompareTestZ,
                         ::testing::ValuesIn(opcodesZ));

using CompareTestAbs = CompareTest;
TEST_P(CompareTestAbs, givenAbsoluteModeThenProcessInstruction) {
    flags.expectCarryFlag(false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);

    auto opCode = GetParam();
    u8 loadToMem = 0xCD;
    u8 loadToReg = 0xCC;
    initializeProcessor(opCode, loadToMem, loadToReg);

    processor.executeInstructions(1);
}

OpCode opcodesAbs[] = {OpCode::CMP_abs, OpCode::CPX_abs, OpCode::CPY_abs};

INSTANTIATE_TEST_SUITE_P(, CompareTestAbs,
                         ::testing::ValuesIn(opcodesAbs), CompareTestAbs::constructParamName);

using CmpTest = CompareTest;

TEST_F(CmpTest, givenZeroPageModeXThenProcessInstruction) {
    flags.expectCarryFlag(false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);

    auto opCode = OpCode::CMP_zx;
    u8 loadToMem = 0xCD;
    u8 loadToReg = 0xCC;
    initializeProcessor(opCode, loadToMem, loadToReg);

    processor.executeInstructions(1);
}

TEST_F(CmpTest, givenAbsoluteModeXThenProcessInstruction) {
    flags.expectCarryFlag(false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);

    auto opCode = OpCode::CMP_absx;
    u8 loadToMem = 0xCD;
    u8 loadToReg = 0xCC;
    initializeProcessor(opCode, loadToMem, loadToReg);

    processor.executeInstructions(1);
}

TEST_F(CmpTest, givenAbsoluteModeYThenProcessInstruction) {
    flags.expectCarryFlag(false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);

    auto opCode = OpCode::CMP_absy;
    u8 loadToMem = 0xCD;
    u8 loadToReg = 0xCC;
    initializeProcessor(opCode, loadToMem, loadToReg);

    processor.executeInstructions(1);
}

TEST_F(CmpTest, givenIndexedIndirectXModeThenProcessInstruction) {
    flags.expectCarryFlag(false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);

    auto opCode = OpCode::CMP_ix;
    u8 loadToMem = 0xCD;
    u8 loadToReg = 0xCC;
    initializeProcessor(opCode, loadToMem, loadToReg);

    processor.executeInstructions(1);
}

TEST_F(CmpTest, givenIndirectIndexedYModeThenProcessInstruction) {
    flags.expectCarryFlag(false);
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);

    auto opCode = OpCode::CMP_iy;
    u8 loadToMem = 0xCD;
    u8 loadToReg = 0xCC;
    initializeProcessor(opCode, loadToMem, loadToReg);

    processor.executeInstructions(1);
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(5, processor.counters.cyclesProcessed);
}