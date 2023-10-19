#include "fixtures.h"

#include "src/error.h"

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
};

TEST_P(LdTest, givenImmediateModeAndNegativeValueThenProcessInstruction) {
    const OpCode opCode = getOpCodeImm();
    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    processor.memory[startAddress + 1] = 0x84;

    processor.executeInstructions(1);
    EXPECT_EQ(0x84, getReg());
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(2, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_P(LdTest, givenImmediateModeAndZeroValueThenProcessInstruction) {
    const OpCode opCode = getOpCodeImm();
    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    processor.memory[startAddress + 1] = 0x0;

    processor.executeInstructions(1);
    EXPECT_EQ(0x0, getReg());
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(2, processor.counters.cyclesProcessed);
    EXPECT_TRUE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

TEST_P(LdTest, givenImmediateModeAndPositiveValueThenProcessInstruction) {
    const OpCode opCode = getOpCodeImm();
    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    processor.memory[startAddress + 1] = 0x74;

    processor.executeInstructions(1);
    EXPECT_EQ(0x74, getReg());
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(2, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

TEST_P(LdTest, givenZeroPageModeThenProcessInstruction) {
    const OpCode opCode = getOpCodeZ();
    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    processor.memory[startAddress + 1] = 0x84;
    processor.memory[0x84] = 0xDD;

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, getReg());
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(3, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_P(LdTest, givenZeroPageXModeThenProcessInstruction) {
    const OpCode opCode = getOpCodeZx();
    if (opCode == OpCode::_INVALID) {
        GTEST_SKIP();
    }

    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    processor.memory[startAddress + 1] = 0x84;
    processor.regs.x = 0x5;
    processor.memory[0x89] = 0xDD;

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, getReg());
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(4, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_P(LdTest, givenZeroPageXModeAndWrapAroundThenProcessInstruction) {
    const OpCode opCode = getOpCodeZx();
    if (opCode == OpCode::_INVALID) {
        GTEST_SKIP();
    }

    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    processor.memory[startAddress + 1] = 0xF0;
    processor.regs.x = 0x30;
    processor.memory[0x20] = 0xDD; // address addition should wrap around to zero page address

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, getReg());
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(4, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_P(LdTest, givenZeroPageYModeThenProcessInstruction) {
    const OpCode opCode = getOpCodeZy();
    if (opCode == OpCode::_INVALID) {
        GTEST_SKIP();
    }

    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    processor.memory[startAddress + 1] = 0x84;
    processor.regs.y = 0x5;
    processor.memory[0x89] = 0xDD;

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, getReg());
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(4, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_P(LdTest, givenZeroPageYModeAndWrapAroundThenProcessInstruction) {
    const OpCode opCode = getOpCodeZy();
    if (opCode == OpCode::_INVALID) {
        GTEST_SKIP();
    }

    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    processor.memory[startAddress + 1] = 0xF0;
    processor.regs.y = 0x30;
    processor.memory[0x20] = 0xDD; // address addition should wrap around to zero page address

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, getReg());
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(4, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_P(LdTest, givenAbsoluteModeThenProcessInstruction) {
    const OpCode opCode = getOpCodeAbs();
    if (opCode == OpCode::_INVALID) {
        GTEST_SKIP();
    }

    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    processor.memory[startAddress + 1] = 0x34;
    processor.memory[startAddress + 2] = 0x12;
    processor.memory[0x1234] = 0xDD;

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, getReg());
    EXPECT_EQ(3, processor.counters.bytesProcessed);
    EXPECT_EQ(4, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_P(LdTest, givenAbsoluteXModeThenProcessInstruction) {
    const OpCode opCode = getOpCodeAbsx();
    if (opCode == OpCode::_INVALID) {
        GTEST_SKIP();
    }

    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    processor.memory[startAddress + 1] = 0x34;
    processor.memory[startAddress + 2] = 0x12;
    processor.regs.x = 0x5;
    processor.memory[0x1239] = 0xDD;

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, getReg());
    EXPECT_EQ(3, processor.counters.bytesProcessed);
    EXPECT_EQ(4, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_P(LdTest, givenAbsoluteXModeAndPageCrossThenProcessInstruction) {
    const OpCode opCode = getOpCodeAbsx();
    if (opCode == OpCode::_INVALID) {
        GTEST_SKIP();
    }

    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    processor.memory[startAddress + 1] = 0xF1;
    processor.memory[startAddress + 2] = 0x12;
    processor.regs.x = 0xF;
    processor.memory[0x1300] = 0xDD;

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, getReg());
    EXPECT_EQ(3, processor.counters.bytesProcessed);
    EXPECT_EQ(5, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_P(LdTest, givenAbsoluteYModeThenProcessInstruction) {
    const OpCode opCode = getOpCodeAbsy();
    if (opCode == OpCode::_INVALID) {
        GTEST_SKIP();
    }

    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    processor.memory[startAddress + 1] = 0x34;
    processor.memory[startAddress + 2] = 0x12;
    processor.regs.y = 0x5;
    processor.memory[0x1239] = 0xDD;

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, getReg());
    EXPECT_EQ(3, processor.counters.bytesProcessed);
    EXPECT_EQ(4, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_P(LdTest, givenAbsoluteYModeAndPageCrossThenProcessInstruction) {
    const OpCode opCode = getOpCodeAbsy();
    if (opCode == OpCode::_INVALID) {
        GTEST_SKIP();
    }

    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    processor.memory[startAddress + 1] = 0xF1;
    processor.memory[startAddress + 2] = 0x12;
    processor.regs.y = 0xF;
    processor.memory[0x1300] = 0xDD;

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, getReg());
    EXPECT_EQ(3, processor.counters.bytesProcessed);
    EXPECT_EQ(5, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_P(LdTest, givenIndexedIndirectXModeThenProcessInstruction) {
    const OpCode opCode = getOpCodeIx();
    if (opCode == OpCode::_INVALID) {
        GTEST_SKIP();
    }

    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    processor.memory[startAddress + 1] = 0x54;
    processor.regs.x = 0x5;
    processor.memory[0x59] = 0x34;
    processor.memory[0x5A] = 0x12;
    processor.memory[0x1234] = 0xDD;

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, getReg());
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(6, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_P(LdTest, givenIndexedIndirectXModeAndWraparoundThenProcessInstruction) {
    const OpCode opCode = getOpCodeIx();
    if (opCode == OpCode::_INVALID) {
        GTEST_SKIP();
    }

    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    processor.memory[startAddress + 1] = 0xF0;
    processor.regs.x = 0x30;
    processor.memory[0x20] = 0x34;
    processor.memory[0x21] = 0x12;
    processor.memory[0x1234] = 0xDD;

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, getReg());
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(6, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_P(LdTest, givenIndirectIndexedYModeThenProcessInstruction) {
    const OpCode opCode = getOpCodeIy();
    if (opCode == OpCode::_INVALID) {
        GTEST_SKIP();
    }

    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    processor.memory[startAddress + 1] = 0x54;
    processor.memory[0x54] = 0x30;
    processor.memory[0x55] = 0x12;
    processor.regs.y = 0x4;
    processor.memory[0x1234] = 0xDD;

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, getReg());
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(5, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_P(LdTest, givenIndirectIndexedYModeAndPageCrossedThenProcessInstruction) {
    const OpCode opCode = getOpCodeIy();
    if (opCode == OpCode::_INVALID) {
        GTEST_SKIP();
    }

    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    processor.memory[startAddress + 1] = 0x54;
    processor.memory[0x54] = 0xFF;
    processor.memory[0x55] = 0x12;
    processor.regs.y = 0x4;
    processor.memory[0x1303] = 0xDD;

    processor.executeInstructions(1);
    EXPECT_EQ(0xDD, getReg());
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(6, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

INSTANTIATE_TEST_SUITE_P(, LdTest, ::testing::ValuesIn({Reg::A, Reg::X, Reg::Y}), LdTest::constructParamName);
