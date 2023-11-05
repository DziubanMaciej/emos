#include "fixtures.h"

#include "src/error.h"

struct TaxTayTxaTyaTest : EmosTest {
    u8 &getReg(OpCode opCode) {
        switch (opCode) {
        case OpCode::TAX:
        case OpCode::TAY:
            return processor.regs.a;
        case OpCode::TXA:
            return processor.regs.x;
        case OpCode::TYA:
            return processor.regs.y;
        default:
            FATAL_ERROR("Wrong OpCode");
        }
    }
    void initializeProcessor(OpCode opcode, [[maybe_unused]] std::optional<u8> value, std::optional<u8> loadToReg) override {
        processor.memory[startAddress + 0] = static_cast<u8>(opcode);
        getReg(opcode) = loadToReg.value();
        expectedBytesProcessed = 1;
        expectedCyclesProcessed = 2;
    }
};

using TaxTest = TaxTayTxaTyaTest;

TEST_F(TaxTest, givenNegativeValueThenProcessInstruction) {
    u8 loadToReg = 0x80;
    initializeProcessor(OpCode::TAX, std::nullopt, loadToReg);

    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);
    processor.executeInstructions(1);
    EXPECT_EQ(0x80, processor.regs.x);
}

TEST_F(TaxTest, givenPositiveValueThenProcessInstruction) {
    u8 loadToReg = 0x70;
    initializeProcessor(OpCode::TAX, std::nullopt, loadToReg);

    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);
    EXPECT_EQ(0x70, processor.regs.x);
}

TEST_F(TaxTest, givenZeroValueThenProcessInstruction) {
    u8 loadToReg = 0x00;
    initializeProcessor(OpCode::TAX, std::nullopt, loadToReg);

    flags.expectZeroFlag(true);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);
    EXPECT_EQ(0x00, processor.regs.x);
}

using TayTest = TaxTayTxaTyaTest;

TEST_F(TayTest, givenNegativeValueThenProcessInstruction) {
    u8 loadToReg = 0x80;
    initializeProcessor(OpCode::TAY, std::nullopt, loadToReg);

    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);
    processor.executeInstructions(1);
    EXPECT_EQ(0x80, processor.regs.y);
}

TEST_F(TayTest, givenPositiveValueThenProcessInstruction) {
    u8 loadToReg = 0x70;
    initializeProcessor(OpCode::TAY, std::nullopt, loadToReg);

    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);
    EXPECT_EQ(0x70, processor.regs.y);
}

TEST_F(TayTest, givenZeroValueThenProcessInstruction) {
    u8 loadToReg = 0x00;
    initializeProcessor(OpCode::TAY, std::nullopt, loadToReg);

    flags.expectZeroFlag(true);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);
    EXPECT_EQ(0x00, processor.regs.y);
}

using TxaTest = TaxTayTxaTyaTest;

TEST_F(TxaTest, givenNegativeValueThenProcessInstruction) {
    u8 loadToReg = 0x80;
    initializeProcessor(OpCode::TXA, std::nullopt, loadToReg);

    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);
    processor.executeInstructions(1);
    EXPECT_EQ(0x80, processor.regs.a);
}

TEST_F(TxaTest, givenPositiveValueThenProcessInstruction) {
    u8 loadToReg = 0x70;
    initializeProcessor(OpCode::TXA, std::nullopt, loadToReg);

    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);
    EXPECT_EQ(0x70, processor.regs.a);
}

TEST_F(TxaTest, givenZeroValueThenProcessInstruction) {
    u8 loadToReg = 0x00;
    initializeProcessor(OpCode::TXA, std::nullopt, loadToReg);

    flags.expectZeroFlag(true);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);
    EXPECT_EQ(0x00, processor.regs.a);
}

using TyaTest = TaxTayTxaTyaTest;

TEST_F(TyaTest, givenNegativeValueThenProcessInstruction) {
    u8 loadToReg = 0x80;
    initializeProcessor(OpCode::TYA, std::nullopt, loadToReg);

    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(true);
    processor.executeInstructions(1);
    EXPECT_EQ(0x80, processor.regs.a);
}

TEST_F(TyaTest, givenPositiveValueThenProcessInstruction) {
    u8 loadToReg = 0x70;
    initializeProcessor(OpCode::TYA, std::nullopt, loadToReg);

    flags.expectZeroFlag(false);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);
    EXPECT_EQ(0x70, processor.regs.a);
}

TEST_F(TyaTest, givenZeroValueThenProcessInstruction) {
    u8 loadToReg = 0x00;
    initializeProcessor(OpCode::TYA, std::nullopt, loadToReg);

    flags.expectZeroFlag(true);
    flags.expectNegativeFlag(false);
    processor.executeInstructions(1);
    EXPECT_EQ(0x00, processor.regs.a);
}
