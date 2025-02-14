#include "src/error.h"
#include "unit_test/fixtures/emos_test.h"

struct InxInyDexDeyTest : testing::WithParamInterface<OpCode>, EmosTest {
    u8 &getReg(OpCode opCode) {
        switch (opCode) {
        case OpCode::INX:
        case OpCode::DEX:
            return processor.regs.x;
        case OpCode::INY:
        case OpCode::DEY:
            return processor.regs.y;
        default:
            FATAL_ERROR("Wrong OpCode");
        }
    }

    void initializeProcessor(OpCode opcode, [[maybe_unused]] std::optional<u8> value, std::optional<u8> loadToReg) {
        processor.memory[startAddress + 0] = static_cast<u8>(opcode);
        getReg(opcode) = loadToReg.value();
        expectedBytesProcessed = 1;
        expectedCyclesProcessed = 2;
    }

    static std::string constructParamName(const testing::TestParamInfo<OpCode> &info) {
        OpCode opCode = info.param;
        switch (opCode) {
        case OpCode::INX:
            return "INX";
        case OpCode::INY:
            return "INY";
        case OpCode::DEX:
            return "DEX";
        case OpCode::DEY:
            return "DEY";
        default:
            FATAL_ERROR("Wrong OpCode");
        }
    }
};

using InxInyTest = InxInyDexDeyTest;

TEST_P(InxInyTest, givenIncrementOperationAndPositiveOutputThenProcessInstruction) {
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    const OpCode opCode = GetParam();
    u8 loadToReg = 0x10;
    initializeProcessor(opCode, std::nullopt, loadToReg);

    processor.executeInstructions(1);
    EXPECT_EQ(0x11, getReg(opCode));
}

TEST_P(InxInyTest, givenNegativeOutputThenProcessInstruction) {
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);
    const OpCode opCode = GetParam();
    u8 loadToReg = 0x7F;
    initializeProcessor(opCode, std::nullopt, loadToReg);

    processor.executeInstructions(1);
    EXPECT_EQ(0x80, getReg(opCode));
}

TEST_P(InxInyTest, givenZeroOutputThenProcessInstruction) {
    flags.expectZeroFlag(true);
    flags.expectNegativeFlag(false);
    const OpCode opCode = GetParam();
    u8 loadToReg = 0xFF;
    initializeProcessor(opCode, std::nullopt, loadToReg);

    processor.executeInstructions(1);
    EXPECT_EQ(0x00, getReg(opCode));
}

INSTANTIATE_TEST_SUITE_P(, InxInyTest, ::testing::ValuesIn({OpCode::INX, OpCode::INY}), InxInyTest::constructParamName);

using DexDeyTest = InxInyDexDeyTest;

TEST_P(DexDeyTest, givenDecrementOperationAndPositiveOutputThenProcessInstruction) {
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    const OpCode opCode = GetParam();
    u8 loadToReg = 0x10;
    initializeProcessor(opCode, std::nullopt, loadToReg);

    processor.executeInstructions(1);
    EXPECT_EQ(0x0F, getReg(opCode));
}

TEST_P(DexDeyTest, givenDecrementOperationAndZeroOutputThenProcessInstruction) {
    flags.expectZeroFlag(true);
    flags.expectNegativeFlag(false);
    const OpCode opCode = GetParam();
    u8 loadToReg = 0x01;
    initializeProcessor(opCode, std::nullopt, loadToReg);

    processor.executeInstructions(1);
    EXPECT_EQ(0x00, getReg(opCode));
}

TEST_P(DexDeyTest, givenDecrementOperationAndNegativeOutputThenProcessInstruction) {
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);
    const OpCode opCode = GetParam();
    u8 loadToReg = 0x81;
    initializeProcessor(opCode, std::nullopt, loadToReg);

    processor.executeInstructions(1);
    EXPECT_EQ(0x80, getReg(opCode));
}

TEST_P(DexDeyTest, givenDecrementOperationWrapAroundThenProcessInstruction) {
    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);
    const OpCode opCode = GetParam();
    u8 loadToReg = 0x00;
    initializeProcessor(opCode, std::nullopt, loadToReg);

    processor.executeInstructions(1);
    EXPECT_EQ(0xFF, getReg(opCode));
}

INSTANTIATE_TEST_SUITE_P(, DexDeyTest, ::testing::ValuesIn({OpCode::DEX, OpCode::DEY}), DexDeyTest::constructParamName);
