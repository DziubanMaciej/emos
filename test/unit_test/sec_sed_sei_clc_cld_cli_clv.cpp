#include "src/error.h"
#include "unit_test/fixtures/emos_test.h"

struct FlagChangeTest : EmosTest {
    void initializeProcessor(OpCode opcode, [[maybe_unused]] std::optional<u8> value, [[maybe_unused]] std::optional<u8> loadToReg) {
        expectedBytesProcessed += 1u;
        expectedCyclesProcessed += 2u;
        initializeForImplied(opcode);
    }
};

TEST_F(FlagChangeTest, whenExecutingSecThenSetCarryFlag) {
    {
        initializeProcessor(OpCode::SEC, std::nullopt, std::nullopt);

        flags.expectCarryFlag(false, true);
        processor.regs.pc = startAddress;
        processor.executeInstructions(1);
        flags.expect();
    }
    {
        initializeProcessor(OpCode::SEC, std::nullopt, std::nullopt);

        flags.expectCarryFlag(true, true);
        processor.regs.pc = startAddress;
        processor.executeInstructions(1);
        flags.expect();
    }
}

TEST_F(FlagChangeTest, whenExecutingSeiThenSetDecimalFlag) {
    {
        initializeProcessor(OpCode::SED, std::nullopt, std::nullopt);

        flags.expectDecimalFlag(false, true);
        processor.regs.pc = startAddress;
        processor.executeInstructions(1);
        flags.expect();
    }
    {
        initializeProcessor(OpCode::SED, std::nullopt, std::nullopt);

        flags.expectDecimalFlag(true, true);
        processor.regs.pc = startAddress;
        processor.executeInstructions(1);
        flags.expect();
    }
}

TEST_F(FlagChangeTest, whenExecutingSeiThenSetInterruptDisableFlag) {
    {
        initializeProcessor(OpCode::SEI, std::nullopt, std::nullopt);

        flags.expectInterruptFlag(false, true);
        processor.regs.pc = startAddress;
        processor.executeInstructions(1);
        flags.expect();
    }
    {
        initializeProcessor(OpCode::SEI, std::nullopt, std::nullopt);

        flags.expectInterruptFlag(true, true);
        processor.regs.pc = startAddress;
        processor.executeInstructions(1);
        flags.expect();
    }
}

TEST_F(FlagChangeTest, whenExecutingClcThenClearCarryFlag) {
    {
        initializeProcessor(OpCode::CLC, std::nullopt, std::nullopt);

        flags.expectCarryFlag(true, false);
        processor.regs.pc = startAddress;
        processor.executeInstructions(1);
        flags.expect();
    }
    {
        initializeProcessor(OpCode::CLC, std::nullopt, std::nullopt);

        flags.expectCarryFlag(false, false);
        processor.regs.pc = startAddress;
        processor.executeInstructions(1);
        flags.expect();
    }
}

TEST_F(FlagChangeTest, whenExecutingCldThenClearDecimalFlag) {
    {
        initializeProcessor(OpCode::CLD, std::nullopt, std::nullopt);

        flags.expectDecimalFlag(true, false);
        processor.regs.pc = startAddress;
        processor.executeInstructions(1);
        flags.expect();
    }
    {
        initializeProcessor(OpCode::CLD, std::nullopt, std::nullopt);

        flags.expectDecimalFlag(false, false);
        processor.regs.pc = startAddress;
        processor.executeInstructions(1);
        flags.expect();
    }
}

TEST_F(FlagChangeTest, whenExecutingCliThenClearInterruptDisableFlag) {
    {
        initializeProcessor(OpCode::CLI, std::nullopt, std::nullopt);

        flags.expectInterruptFlag(true, false);
        processor.regs.pc = startAddress;
        processor.executeInstructions(1);
        flags.expect();
    }
    {
        initializeProcessor(OpCode::CLI, std::nullopt, std::nullopt);

        flags.expectInterruptFlag(false, false);
        processor.regs.pc = startAddress;
        processor.executeInstructions(1);
        flags.expect();
    }
}

TEST_F(FlagChangeTest, whenExecutingClvThenClearOverflowFlag) {
    {
        initializeProcessor(OpCode::CLV, std::nullopt, std::nullopt);

        flags.expectOverflowFlag(true, false);
        processor.regs.pc = startAddress;
        processor.executeInstructions(1);
        flags.expect();
    }
    {
        initializeProcessor(OpCode::CLV, std::nullopt, std::nullopt);

        flags.expectOverflowFlag(false, false);
        processor.regs.pc = startAddress;
        processor.executeInstructions(1);
        flags.expect();
    }
}
