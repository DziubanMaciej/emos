#include "src/error.h"
#include "unit_test/fixtures/emos_test.h"
struct IncDecTest : EmosTest {
    ReferencedValue initializeProcessor(OpCode opcode, std::optional<u8> value, std::optional<u8>) {
        switch (opcode) {
        case OpCode::INC_z:
        case OpCode::DEC_z:
            expectedBytesProcessed = 2;
            expectedCyclesProcessed = 5;
            return initializeForZeroPage(opcode, value.value());
        case OpCode::INC_zx:
        case OpCode::DEC_zx:
            expectedBytesProcessed = 2;
            expectedCyclesProcessed = 6;
            return initializeForZeroPageX(opcode, value.value());
        case OpCode::INC_abs:
        case OpCode::DEC_abs:
            expectedBytesProcessed = 3;
            expectedCyclesProcessed = 6;
            return initializeForAbsolute(opcode, value.value());
        case OpCode::INC_absx:
        case OpCode::DEC_absx:
            expectedBytesProcessed = 3;
            expectedCyclesProcessed = 7;
            return initializeForAbsoluteX(opcode, value.value());
        default:
            FATAL_ERROR("Wrong OpCode");
        }
    }
};

using IncTest = IncDecTest;

TEST_F(IncTest, givenZeroPageModeThenProcessInstruction) {
    auto opCode = OpCode::INC_z;
    u8 loadToMem = 0x70;

    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    ReferencedValue referencedValue = initializeProcessor(opCode, loadToMem, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0x71, referencedValue.read());
}

TEST_F(IncTest, givenZeroPageModeAndNegativeValueThenProcessInstruction) {
    auto opCode = OpCode::INC_z;
    u8 loadToMem = 0x7F;

    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);
    ReferencedValue referencedValue = initializeProcessor(opCode, loadToMem, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0x80, referencedValue.read());
}

TEST_F(IncTest, givenZeroPageModeAndWrapAroundThenProcessInstruction) {
    auto opCode = OpCode::INC_z;
    u8 loadToMem = 0xFF;

    flags.expectZeroFlag(true);
    flags.expectNegativeFlag(false);
    ReferencedValue referencedValue = initializeProcessor(opCode, loadToMem, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0x00, referencedValue.read());
}

TEST_F(IncTest, givenZeroPageXModeThenProcessInstruction) {
    auto opCode = OpCode::INC_zx;
    u8 loadToMem = 0x70;

    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    ReferencedValue referencedValue = initializeProcessor(opCode, loadToMem, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0x71, referencedValue.read());
}

TEST_F(IncTest, givenAbsoluteModeThenProcessInstruction) {
    auto opCode = OpCode::INC_abs;
    u8 loadToMem = 0x70;

    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    ReferencedValue referencedValue = initializeProcessor(opCode, loadToMem, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0x71, referencedValue.read());
}

TEST_F(IncTest, givenAbsoluteXModeThenProcessInstruction) {
    auto opCode = OpCode::INC_absx;
    u8 loadToMem = 0x70;

    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    ReferencedValue referencedValue = initializeProcessor(opCode, loadToMem, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0x71, referencedValue.read());
}

using DecTest = IncDecTest;

TEST_F(DecTest, givenZeroPageModeThenProcessInstruction) {
    auto opCode = OpCode::DEC_z;
    u8 loadToMem = 0x80;

    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    ReferencedValue referencedValue = initializeProcessor(opCode, loadToMem, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0x7F, referencedValue.read());
}

TEST_F(DecTest, givenZeroPageModeAndNegativeValueThenProcessInstruction) {
    auto opCode = OpCode::DEC_z;
    u8 loadToMem = 0x81;

    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);
    ReferencedValue referencedValue = initializeProcessor(opCode, loadToMem, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0x80, referencedValue.read());
}

TEST_F(DecTest, givenZeroPageModeAndResultIsZeroThenProcessInstruction) {
    auto opCode = OpCode::DEC_z;
    u8 loadToMem = 0x01;

    flags.expectZeroFlag(true);
    flags.expectNegativeFlag(false);
    ReferencedValue referencedValue = initializeProcessor(opCode, loadToMem, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0x00, referencedValue.read());
}

TEST_F(DecTest, givenZeroPageModeAndWrapAroundThenProcessInstruction) {
    auto opCode = OpCode::DEC_z;
    u8 loadToMem = 0x00;

    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);
    ReferencedValue referencedValue = initializeProcessor(opCode, loadToMem, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0xFF, referencedValue.read());
}

TEST_F(DecTest, givenZeroPageXModeThenProcessInstruction) {
    auto opCode = OpCode::DEC_zx;
    u8 loadToMem = 0x70;

    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    ReferencedValue referencedValue = initializeProcessor(opCode, loadToMem, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0x6F, referencedValue.read());
}

TEST_F(DecTest, givenAbsoluteModeThenProcessInstruction) {
    auto opCode = OpCode::DEC_abs;
    u8 loadToMem = 0x70;

    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    ReferencedValue referencedValue = initializeProcessor(opCode, loadToMem, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0x6F, referencedValue.read());
}

TEST_F(DecTest, givenAbsoluteXModeThenProcessInstruction) {
    auto opCode = OpCode::DEC_absx;
    u8 loadToMem = 0x70;

    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    ReferencedValue referencedValue = initializeProcessor(opCode, loadToMem, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0x6F, referencedValue.read());
}
