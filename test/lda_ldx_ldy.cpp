#include "src/error.h"
#include "test/fixtures/emos_test.h"

enum class Reg {
    A,
    X,
    Y,
};

struct LdTest : testing::WithParamInterface<Reg>,
                EmosTest {

    u8 &getReg() {
        switch (GetParam()) {
        case Reg::A:
            return processor.regs.a;
        case Reg::X:
            return processor.regs.x;
        case Reg::Y:
            return processor.regs.y;
        default:
            FATAL_ERROR("Wrong reg");
        }
    }

    OpCode getOpCodeImm() {
        return selectOpCode(OpCode::LDA_imm, OpCode::LDX_imm, OpCode::LDY_imm);
    }

    OpCode getOpCodeZ() {
        return selectOpCode(OpCode::LDA_z, OpCode::LDX_z, OpCode::LDY_z);
    }

    OpCode getOpCodeZx() {
        return selectOpCode(OpCode::LDA_zx, OpCode::_INVALID, OpCode::LDY_zx);
    }

    OpCode getOpCodeZy() {
        return selectOpCode(OpCode::_INVALID, OpCode::LDX_zy, OpCode::_INVALID);
    }

    OpCode getOpCodeAbs() {
        return selectOpCode(OpCode::LDA_abs, OpCode::LDX_abs, OpCode::LDY_abs);
    }

    OpCode getOpCodeAbsx() {
        return selectOpCode(OpCode::LDA_absx, OpCode::_INVALID, OpCode::LDY_absx);
    }

    OpCode getOpCodeAbsy() {
        return selectOpCode(OpCode::_INVALID, OpCode::LDX_absy, OpCode::_INVALID);
    }

    OpCode getOpCodeIx() {
        return selectOpCode(OpCode::LDA_ix, OpCode::_INVALID, OpCode::_INVALID);
    }

    OpCode getOpCodeIy() {
        return selectOpCode(OpCode::LDA_iy, OpCode::_INVALID, OpCode::_INVALID);
    }

    OpCode selectOpCode(OpCode lda, OpCode ldx, OpCode ldy) {
        switch (GetParam()) {
        case Reg::A:
            return lda;
        case Reg::X:
            return ldx;
        case Reg::Y:
            return ldy;
        default:
            FATAL_ERROR("Wrong reg");
        }
    }

    static std::string constructParamName(const testing::TestParamInfo<Reg> &info) {
        switch (info.param) {
        case Reg::A:
            return "LDA";
        case Reg::X:
            return "LDX";
        case Reg::Y:
            return "LDY";
        default:
            FATAL_ERROR("Wrong reg");
        }
    }
    void initializeProcessor(OpCode opcode, std::optional<u8> value, [[maybe_unused]] std::optional<u8> loadToReg) override {

        switch (opcode) {
        case OpCode::LDA_imm:
        case OpCode::LDX_imm:
        case OpCode::LDY_imm:
            initializeForImmediate(opcode, value.value());
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 2u;
            return;
        case OpCode::LDA_z:
        case OpCode::LDX_z:
        case OpCode::LDY_z:
            initializeForZeroPage(opcode, value.value());
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 3u;
            return;
        case OpCode::LDA_zx:
        case OpCode::LDY_zx:
            initializeForZeroPageX(opcode, value.value());
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 4u;
            return;
        case OpCode::LDX_zy:
            initializeForZeroPageY(opcode, value.value());
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 4u;
            return;
        case OpCode::LDA_ix:
            initializeForIndirectX(opcode, value.value());
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 6u;
            return;
        case OpCode::LDA_iy:
            initializeForIndirectY(opcode, value.value());
            expectedBytesProcessed = 2u;
            expectedCyclesProcessed = 5u;
            return;
        case OpCode::LDA_abs:
        case OpCode::LDX_abs:
        case OpCode::LDY_abs:
            initializeForAbsolute(opcode, value.value());
            expectedBytesProcessed = 3u;
            expectedCyclesProcessed = 4u;
            return;
        case OpCode::LDA_absx:
        case OpCode::LDY_absx:
            initializeForAbsoluteX(opcode, value.value());
            expectedBytesProcessed = 3u;
            expectedCyclesProcessed = 4u;
            return;
        case OpCode::LDX_absy:
            initializeForAbsoluteY(opcode, value.value());
            expectedBytesProcessed = 3u;
            expectedCyclesProcessed = 4u;
            return;
        default:
            FATAL_ERROR("Wrong OpCode");
        }
    }
};

TEST_P(LdTest, givenImmediateModeAndNegativeValueThenProcessInstruction) {
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);
    const OpCode opCode = getOpCodeImm();
    initializeProcessor(opCode, 0x84, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0x84, getReg());
}

TEST_P(LdTest, givenImmediateModeAndZeroValueThenProcessInstruction) {
    flags.expectZeroFlag(true);
    flags.expectNegativeFlag(false);
    const OpCode opCode = getOpCodeImm();
    initializeProcessor(opCode, 0x0, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0x0, getReg());
}

TEST_P(LdTest, givenImmediateModeAndPositiveValueThenProcessInstruction) {
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    const OpCode opCode = getOpCodeImm();
    initializeProcessor(opCode, 0x74, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0x74, getReg());
}

TEST_P(LdTest, givenZeroPageModeThenProcessInstruction) {
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);
    const OpCode opCode = getOpCodeZ();
    initializeProcessor(opCode, 0xDD, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, getReg());
}

TEST_P(LdTest, givenZeroPageXModeThenProcessInstruction) {
    const OpCode opCode = getOpCodeZx();
    if (opCode == OpCode::_INVALID) {
        GTEST_SKIP();
    }
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);
    initializeProcessor(opCode, 0xDD, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, getReg());
}

TEST_P(LdTest, givenZeroPageXModeAndWrapAroundThenProcessInstruction) {
    const OpCode opCode = getOpCodeZx();
    if (opCode == OpCode::_INVALID) {
        GTEST_SKIP();
    }
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);

    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    processor.memory[startAddress + 1] = 0xF0;
    processor.regs.x = 0x30;
    processor.memory[0x20] = 0xDD; // address addition should wrap around to zero page address

    expectedBytesProcessed = 2;
    expectedCyclesProcessed = 4;

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, getReg());
}

TEST_P(LdTest, givenZeroPageYModeThenProcessInstruction) {
    const OpCode opCode = getOpCodeZy();
    if (opCode == OpCode::_INVALID) {
        GTEST_SKIP();
    }
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);
    initializeProcessor(opCode, 0xDD, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, getReg());
}

TEST_P(LdTest, givenZeroPageYModeAndWrapAroundThenProcessInstruction) {
    const OpCode opCode = getOpCodeZy();
    if (opCode == OpCode::_INVALID) {
        GTEST_SKIP();
    }
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);

    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    processor.memory[startAddress + 1] = 0xF0;
    processor.regs.y = 0x30;
    processor.memory[0x20] = 0xDD; // address addition should wrap around to zero page address
    expectedBytesProcessed = 2;
    expectedCyclesProcessed = 4;

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, getReg());
}

TEST_P(LdTest, givenAbsoluteModeThenProcessInstruction) {
    const OpCode opCode = getOpCodeAbs();
    if (opCode == OpCode::_INVALID) {
        GTEST_SKIP();
    }
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);
    initializeProcessor(opCode, 0xDD, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, getReg());
}

TEST_P(LdTest, givenAbsoluteXModeThenProcessInstruction) {
    const OpCode opCode = getOpCodeAbsx();
    if (opCode == OpCode::_INVALID) {
        GTEST_SKIP();
    }
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);
    initializeProcessor(opCode, 0xDD, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, getReg());
}

TEST_P(LdTest, givenAbsoluteXModeAndPageCrossThenProcessInstruction) {
    const OpCode opCode = getOpCodeAbsx();
    if (opCode == OpCode::_INVALID) {
        GTEST_SKIP();
    }
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);

    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    processor.memory[startAddress + 1] = 0xF1;
    processor.memory[startAddress + 2] = 0x12;
    processor.regs.x = 0xF;
    processor.memory[0x1300] = 0xDD;
    expectedBytesProcessed = 3;
    expectedCyclesProcessed = 5;

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, getReg());
}

TEST_P(LdTest, givenAbsoluteYModeThenProcessInstruction) {
    const OpCode opCode = getOpCodeAbsy();
    if (opCode == OpCode::_INVALID) {
        GTEST_SKIP();
    }
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);
    initializeProcessor(opCode, 0xDD, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, getReg());
}

TEST_P(LdTest, givenAbsoluteYModeAndPageCrossThenProcessInstruction) {
    const OpCode opCode = getOpCodeAbsy();
    if (opCode == OpCode::_INVALID) {
        GTEST_SKIP();
    }
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);

    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    processor.memory[startAddress + 1] = 0xF1;
    processor.memory[startAddress + 2] = 0x12;
    processor.regs.y = 0xF;
    processor.memory[0x1300] = 0xDD;
    expectedBytesProcessed = 3;
    expectedCyclesProcessed = 5;

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, getReg());
}

TEST_P(LdTest, givenIndexedIndirectXModeThenProcessInstruction) {
    const OpCode opCode = getOpCodeIx();
    if (opCode == OpCode::_INVALID) {
        GTEST_SKIP();
    }
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);
    initializeProcessor(opCode, 0xDD, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, getReg());
}

TEST_P(LdTest, givenIndexedIndirectXModeAndWraparoundThenProcessInstruction) {
    const OpCode opCode = getOpCodeIx();
    if (opCode == OpCode::_INVALID) {
        GTEST_SKIP();
    }
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);

    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    processor.memory[startAddress + 1] = 0xF0;
    processor.regs.x = 0x30;
    processor.memory[0x20] = 0x34;
    processor.memory[0x21] = 0x12;
    processor.memory[0x1234] = 0xDD;
    expectedBytesProcessed = 2;
    expectedCyclesProcessed = 6;

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, getReg());
}

TEST_P(LdTest, givenIndirectIndexedYModeThenProcessInstruction) {
    const OpCode opCode = getOpCodeIy();
    if (opCode == OpCode::_INVALID) {
        GTEST_SKIP();
    }
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);
    initializeProcessor(opCode, 0xDD, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, getReg());
}

TEST_P(LdTest, givenIndirectIndexedYModeAndPageCrossedThenProcessInstruction) {
    const OpCode opCode = getOpCodeIy();
    if (opCode == OpCode::_INVALID) {
        GTEST_SKIP();
    }
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);

    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    processor.memory[startAddress + 1] = 0x54;
    processor.memory[0x54] = 0xFF;
    processor.memory[0x55] = 0x12;
    processor.regs.y = 0x4;
    processor.memory[0x1303] = 0xDD;
    expectedBytesProcessed = 2;
    expectedCyclesProcessed = 6;

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, getReg());
}

INSTANTIATE_TEST_SUITE_P(, LdTest, ::testing::ValuesIn({Reg::A, Reg::X, Reg::Y}), LdTest::constructParamName);
