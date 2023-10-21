#include "fixtures.h"

#include "src/error.h"

struct CompareTest : testing::WithParamInterface<OpCode>, EmosTest {
    void checkNotAffectedFlags() override {
        EXPECT_EQ(flagsOnStart.i, processor.regs.flags.i);
        EXPECT_EQ(flagsOnStart.d, processor.regs.flags.d);
        EXPECT_EQ(flagsOnStart.b, processor.regs.flags.b);
        EXPECT_EQ(flagsOnStart.o, processor.regs.flags.o);
    }
    void setRegisterForOpCode(OpCode opCode, u8 value) {
        switch (opCode) {
        case OpCode::CMP_imm:
        case OpCode::CMP_z:
        case OpCode::CMP_abs:
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
    auto opCode = GetParam();
    setRegisterForOpCode(opCode, 0xCC);

    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    processor.memory[startAddress + 1] = 0xCD;

    processor.executeInstructions(1);
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(2, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.c);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_P(CompareTestImm, givenImmediateModeAndEqualValueThenProcessInstruction) {
    auto opCode = GetParam();
    setRegisterForOpCode(opCode, 0xCC);
    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    processor.memory[startAddress + 1] = 0xCC;

    processor.executeInstructions(1);
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(2, processor.counters.cyclesProcessed);
    EXPECT_TRUE(processor.regs.flags.c);
    EXPECT_TRUE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

TEST_P(CompareTestImm, givenImmediateModeAndLessValueThenProcessInstruction) {
    auto opCode = GetParam();
    setRegisterForOpCode(opCode, 0xCC);
    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    processor.memory[startAddress + 1] = 0xCB;

    processor.executeInstructions(1);
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(2, processor.counters.cyclesProcessed);
    EXPECT_TRUE(processor.regs.flags.c);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

OpCode opcodesImm[] = {OpCode::CMP_imm, OpCode::CPX_imm, OpCode::CPY_imm};

INSTANTIATE_TEST_SUITE_P(, CompareTestImm,
                         ::testing::ValuesIn(opcodesImm), CompareTestImm::constructParamName);

using CompareTestZ = CompareTest;
TEST_P(CompareTestZ, givenZeroPageModeThenProcessInstruction) {
    auto opCode = GetParam();
    setRegisterForOpCode(opCode, 0xCC);
    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    processor.memory[startAddress + 1] = 0x05;
    processor.memory[0x05] = 0xCD;

    processor.executeInstructions(1);
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(3, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.c);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

OpCode opcodesZ[] = {OpCode::CMP_z, OpCode::CPX_z, OpCode::CPY_z};

INSTANTIATE_TEST_SUITE_P(, CompareTestZ,
                         ::testing::ValuesIn(opcodesZ));

using CompareTestAbs = CompareTest;
TEST_P(CompareTestAbs, givenAbsoluteModeThenProcessInstruction) {
    auto opCode = GetParam();
    setRegisterForOpCode(opCode, 0xCC);
    processor.memory[startAddress + 0] = static_cast<u8>(opCode);
    processor.memory[startAddress + 1] = 0x34;
    processor.memory[startAddress + 2] = 0x12;
    processor.memory[0x1234] = 0xCD;

    processor.executeInstructions(1);
    EXPECT_EQ(3, processor.counters.bytesProcessed);
    EXPECT_EQ(4, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.c);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

OpCode opcodesAbs[] = {OpCode::CMP_abs, OpCode::CPX_abs, OpCode::CPY_abs};

INSTANTIATE_TEST_SUITE_P(, CompareTestAbs,
                         ::testing::ValuesIn(opcodesAbs), CompareTestAbs::constructParamName);

struct CmpTest : EmosTest {
    void checkNotAffectedFlags() override {
        EXPECT_EQ(flagsOnStart.i, processor.regs.flags.i);
        EXPECT_EQ(flagsOnStart.d, processor.regs.flags.d);
        EXPECT_EQ(flagsOnStart.b, processor.regs.flags.b);
        EXPECT_EQ(flagsOnStart.o, processor.regs.flags.o);
    }
};

TEST_F(CmpTest, givenZeroPageModeXThenProcessInstruction) {
    processor.regs.a = 0xCC;
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::CMP_zx);
    processor.memory[startAddress + 1] = 0x05;
    processor.regs.x = 0x3;
    processor.memory[0x08] = 0xCD;

    processor.executeInstructions(1);
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(4, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.c);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_F(CmpTest, givenAbsoluteModeXThenProcessInstruction) {
    processor.regs.a = 0xCC;
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::CMP_absx);
    processor.memory[startAddress + 1] = 0x34;
    processor.memory[startAddress + 2] = 0x12;
    processor.regs.x = 0x3;
    processor.memory[0x1237] = 0xCD;

    processor.executeInstructions(1);
    EXPECT_EQ(3, processor.counters.bytesProcessed);
    EXPECT_EQ(4, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.c);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_F(CmpTest, givenAbsoluteModeYThenProcessInstruction) {
    processor.regs.a = 0xCC;
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::CMP_absy);
    processor.memory[startAddress + 1] = 0x34;
    processor.memory[startAddress + 2] = 0x12;
    processor.regs.y = 0x3;
    processor.memory[0x1237] = 0xCD;

    processor.executeInstructions(1);
    EXPECT_EQ(3, processor.counters.bytesProcessed);
    EXPECT_EQ(4, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.c);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_F(CmpTest, givenIndexedIndirectXModeThenProcessInstruction) {
    processor.regs.a = 0xCC;
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::CMP_ix);
    processor.memory[startAddress + 1] = 0x54;
    processor.regs.x = 0x5;
    processor.memory[0x59] = 0x34;
    processor.memory[0x5A] = 0x12;
    processor.memory[0x1234] = 0xCD;

    processor.executeInstructions(1);
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(6, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.c);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_F(CmpTest, givenIndirectIndexedYModeThenProcessInstruction) {
    processor.regs.a = 0xCC;
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::CMP_iy);
    processor.memory[startAddress + 1] = 0x54;
    processor.memory[0x54] = 0x30;
    processor.memory[0x55] = 0x12;
    processor.regs.y = 0x4;
    processor.memory[0x1234] = 0xCD;

    processor.executeInstructions(1);
    EXPECT_EQ(2, processor.counters.bytesProcessed);
    EXPECT_EQ(5, processor.counters.cyclesProcessed);
    EXPECT_FALSE(processor.regs.flags.c);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}