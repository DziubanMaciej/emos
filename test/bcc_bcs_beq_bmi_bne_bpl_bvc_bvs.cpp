#include "src/error.h"
#include "test/fixtures/emos_test.h"

struct BranchTest : testing::WithParamInterface<OpCode>, EmosTest {
    void setupFlagsForBranch(bool branchIsTaken) {
        const OpCode opCode = GetParam();

        switch (opCode) {
        case OpCode::BCC:
            flags.expectCarryFlag(!branchIsTaken, !branchIsTaken);
            break;
        case OpCode::BCS:
            flags.expectCarryFlag(branchIsTaken, branchIsTaken);
            break;
        case OpCode::BEQ:
            flags.expectZeroFlag(branchIsTaken, branchIsTaken);
            break;
        case OpCode::BMI:
            flags.expectNegativeFlag(branchIsTaken, branchIsTaken);
            break;
        case OpCode::BNE:
            flags.expectZeroFlag(!branchIsTaken, !branchIsTaken);
            break;
        case OpCode::BPL:
            flags.expectNegativeFlag(!branchIsTaken, !branchIsTaken);
            break;
        case OpCode::BVC:
            flags.expectOverflowFlag(!branchIsTaken, !branchIsTaken);
            break;
        case OpCode::BVS:
            flags.expectOverflowFlag(branchIsTaken, branchIsTaken);
            break;
        default:
            FATAL_ERROR("Invalid opcode")
        }
    }
};

TEST_P(BranchTest, givenBranchIsTakenThenUpdatePc) {
    setupFlagsForBranch(true);

    setStartAddress(0xAA00);
    processor.memory[startAddress + 0] = static_cast<u8>(GetParam());
    processor.memory[startAddress + 1] = 0xFD;

    processor.executeInstructions(1);

    EXPECT_EQ(startAddress + 2 + 0xFD, processor.regs.pc);

    expectedBytesProcessed = 2;
    expectedCyclesProcessed = 3; // additional cycle for branching
}

TEST_P(BranchTest, givenBranchIsNotTakenThenDoNotUpdatePc) {
    setupFlagsForBranch(false);

    setStartAddress(0xAA00);
    processor.memory[startAddress + 0] = static_cast<u8>(GetParam());
    processor.memory[startAddress + 1] = 0xFD;

    processor.executeInstructions(1);

    EXPECT_EQ(startAddress + 2, processor.regs.pc);

    expectedBytesProcessed = 2;
    expectedCyclesProcessed = 2;
}

TEST_P(BranchTest, givenCrossPageBranchIsTakenThenUpdatePc) {
    setupFlagsForBranch(true);

    setStartAddress(0xAA80);
    processor.memory[startAddress + 0] = static_cast<u8>(GetParam());
    processor.memory[startAddress + 1] = 0xFD;

    processor.executeInstructions(1);

    EXPECT_EQ(startAddress + 2 + 0xFD, processor.regs.pc);

    expectedBytesProcessed = 2;
    expectedCyclesProcessed = 4; // additional cycle for branching, additional cycle for page crossing
}

TEST_P(BranchTest, givenCrosPageBranchIsNotTakenThenDoNotUpdatePc) {
    setupFlagsForBranch(false);

    setStartAddress(0xAA80);
    processor.memory[startAddress + 0] = static_cast<u8>(GetParam());
    processor.memory[startAddress + 1] = 0xFD;

    processor.executeInstructions(1);

    EXPECT_EQ(startAddress + 2, processor.regs.pc);

    expectedBytesProcessed = 2;
    expectedCyclesProcessed = 2;
}

INSTANTIATE_TEST_SUITE_P(, BranchTest, ::testing::ValuesIn({
                                           OpCode::BCC,
                                           OpCode::BCS,
                                           OpCode::BEQ,
                                           OpCode::BMI,
                                           OpCode::BNE,
                                           OpCode::BPL,
                                           OpCode::BVC,
                                           OpCode::BVS,
                                       }));
