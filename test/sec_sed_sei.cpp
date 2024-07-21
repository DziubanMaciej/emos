#include "src/error.h"
#include "test/fixtures/emos_test.h"

struct SecSedSeiTests : EmosTest {
    void initializeProcessor(OpCode opcode, [[maybe_unused]] std::optional<u8> value, [[maybe_unused]] std::optional<u8> loadToReg) {
        initializeForImplied(opcode);
        expectedBytesProcessed = 1u;
        expectedCyclesProcessed = 2u;
    }
};
using SecTest = SecSedSeiTests;
TEST_F(SecTest, givenNotSetCarryFlagWhenExecuteInstructionSecThenExpectCarryFlagSet) {
    initializeProcessor(OpCode::SEC, std::nullopt, std::nullopt);
    flags.expectCarryFlag(false, true);
    processor.executeInstructions(1);
}
TEST_F(SecTest, givenSetCarryFlagWhenExecuteInstructionSecThenExpectCarryFlagSet) {
    initializeProcessor(OpCode::SEC, std::nullopt, std::nullopt);
    flags.expectCarryFlag(true, true);
    processor.executeInstructions(1);
}
using SedTest = SecSedSeiTests;
TEST_F(SedTest, givenNotSetDecimalFlagWhenExecuteInstructionSedThenExpectDecimalFlagSet) {
    initializeProcessor(OpCode::SED, std::nullopt, std::nullopt);
    flags.expectDecimalFlag(false, true);
    processor.executeInstructions(1);
}
TEST_F(SedTest, givenSetDecimalFlagWhenExecuteInstructionSedThenExpectDecimalFlagSet) {
    initializeProcessor(OpCode::SED, std::nullopt, std::nullopt);
    flags.expectDecimalFlag(true, true);
    processor.executeInstructions(1);
}
using SeiTest = SecSedSeiTests;
TEST_F(SeiTest, givenNotSetInterruptFlagWhenExecuteInstructionSeiThenExpectInteqrruptFlagSet) {
    initializeProcessor(OpCode::SEI, std::nullopt, std::nullopt);
    flags.expectInterruptFlag(false, true);
    processor.executeInstructions(1);
}
TEST_F(SeiTest, givenSetInterruptFlagWhenExecuteInstructionSeiThenExpectInterruptFlagSet) {
    initializeProcessor(OpCode::SEI, std::nullopt, std::nullopt);
    flags.expectInterruptFlag(true, true);
    processor.executeInstructions(1);
}
