#include "fixtures.h"

#include "src/error.h"
struct IncDecTest : EmosTest {
    void initializeProcessor(OpCode opcode, std::optional<u8> value, std::optional<u8>) override {
        switch (opcode) {
        case OpCode::INC_z:
        case OpCode::DEC_z:
            initializeForZeroPage(opcode, value.value());
            expectedBytesProcessed = 2;
            expectedCyclesProcessed = 5;
            return;
        case OpCode::INC_zx:
        case OpCode::DEC_zx:
            initializeForZeroPageX(opcode, value.value());
            expectedBytesProcessed = 2;
            expectedCyclesProcessed = 6;
            return;
        case OpCode::INC_abs:
        case OpCode::DEC_abs:
            initializeForAbsolute(opcode, value.value());
            expectedBytesProcessed = 3;
            expectedCyclesProcessed = 6;
            return;
        case OpCode::INC_absx:
        case OpCode::DEC_absx:
            initializeForAbsoluteX(opcode, value.value());
            expectedBytesProcessed = 3;
            expectedCyclesProcessed = 7;
            return;
        default:
            FATAL_ERROR("Wrong OpCode");
        }
    }
};

using IncTest = IncDecTest;

TEST_F(IncTest, givenZeroPageModeThenProcessInstruction) {
    auto opCode = OpCode::INC_z;
    u8 loadToMem = 0x70;

    initializeProcessor(opCode, loadToMem, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0x71, processor.memory[dummyAddressUsedForWriteValueToMemory]);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

TEST_F(IncTest, givenZeroPageModeAndNegativeValueThenProcessInstruction) {
    auto opCode = OpCode::INC_z;
    u8 loadToMem = 0x7F;

    initializeProcessor(opCode, loadToMem, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0x80, processor.memory[dummyAddressUsedForWriteValueToMemory]);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_F(IncTest, givenZeroPageModeAndWrapAroundThenProcessInstruction) {
    auto opCode = OpCode::INC_z;
    u8 loadToMem = 0xFF;

    initializeProcessor(opCode, loadToMem, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0x00, processor.memory[dummyAddressUsedForWriteValueToMemory]);
    EXPECT_TRUE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

TEST_F(IncTest, givenZeroPageXModeThenProcessInstruction) {
    auto opCode = OpCode::INC_zx;
    u8 loadToMem = 0x70;

    initializeProcessor(opCode, loadToMem, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0x71, processor.memory[dummyAddressUsedForWriteValueToMemory]);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

TEST_F(IncTest, givenAbsoluteModeThenProcessInstruction) {
    auto opCode = OpCode::INC_abs;
    u8 loadToMem = 0x70;

    initializeProcessor(opCode, loadToMem, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0x71, processor.memory[dummyAddressUsedForWriteValueToMemory]);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

TEST_F(IncTest, givenAbsoluteXModeThenProcessInstruction) {
    auto opCode = OpCode::INC_absx;
    u8 loadToMem = 0x70;

    initializeProcessor(opCode, loadToMem, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0x71, processor.memory[dummyAddressUsedForWriteValueToMemory]);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

using DecTest = IncDecTest;

TEST_F(DecTest, givenZeroPageModeThenProcessInstruction) {
    auto opCode = OpCode::DEC_z;
    u8 loadToMem = 0x80;

    initializeProcessor(opCode, loadToMem, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0x7F, processor.memory[dummyAddressUsedForWriteValueToMemory]);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

TEST_F(DecTest, givenZeroPageModeAndNegativeValueThenProcessInstruction) {
    auto opCode = OpCode::DEC_z;
    u8 loadToMem = 0x81;

    initializeProcessor(opCode, loadToMem, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0x80, processor.memory[dummyAddressUsedForWriteValueToMemory]);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_F(DecTest, givenZeroPageModeAndResultIsZeroThenProcessInstruction) {
    auto opCode = OpCode::DEC_z;
    u8 loadToMem = 0x01;

    initializeProcessor(opCode, loadToMem, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0x00, processor.memory[dummyAddressUsedForWriteValueToMemory]);
    EXPECT_TRUE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

TEST_F(DecTest, givenZeroPageModeAndWrapAroundThenProcessInstruction) {
    auto opCode = OpCode::DEC_z;
    u8 loadToMem = 0x00;

    initializeProcessor(opCode, loadToMem, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0xFF, processor.memory[dummyAddressUsedForWriteValueToMemory]);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_TRUE(processor.regs.flags.n);
}

TEST_F(DecTest, givenZeroPageXModeThenProcessInstruction) {
    auto opCode = OpCode::DEC_zx;
    u8 loadToMem = 0x70;

    initializeProcessor(opCode, loadToMem, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0x6F, processor.memory[dummyAddressUsedForWriteValueToMemory]);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

TEST_F(DecTest, givenAbsoluteModeThenProcessInstruction) {
    auto opCode = OpCode::DEC_abs;
    u8 loadToMem = 0x70;

    initializeProcessor(opCode, loadToMem, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0x6F, processor.memory[dummyAddressUsedForWriteValueToMemory]);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}

TEST_F(DecTest, givenAbsoluteXModeThenProcessInstruction) {
    auto opCode = OpCode::DEC_absx;
    u8 loadToMem = 0x70;

    initializeProcessor(opCode, loadToMem, std::nullopt);

    processor.executeInstructions(1);
    EXPECT_EQ(0x6F, processor.memory[dummyAddressUsedForWriteValueToMemory]);
    EXPECT_FALSE(processor.regs.flags.z);
    EXPECT_FALSE(processor.regs.flags.n);
}
