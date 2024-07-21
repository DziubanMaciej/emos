#include "src/error.h"
#include "test/fixtures/emos_test.h"

struct BitTest : testing::WithParamInterface<OpCode>, EmosTest {
    void initializeProcessor(OpCode opcode, std::optional<u8> value, std::optional<u8> regA) {
        referencedValue.clear(); // TODO remove this, and just return referencedValue from this function

        processor.regs.a = regA.value();
        switch (opcode) {
        case OpCode::BIT_z:
            initializeForZeroPage(opcode, value.value());
            expectedBytesProcessed += 2u;
            expectedCyclesProcessed += 3u;
            return;
        case OpCode::BIT_abs:
            initializeForAbsolute(opcode, value.value());
            expectedBytesProcessed += 3u;
            expectedCyclesProcessed += 4u;
            return;
        default:
            FATAL_ERROR("Wrong OpCode");
        }
    }

    static std::string constructParamName(const testing::TestParamInfo<OpCode> &info) {
        OpCode opCode = info.param;
        switch (opCode) {
        case OpCode::BIT_z:
            return "BIT_z";
        case OpCode::BIT_abs:
            return "BIT_abs";
        default:
            FATAL_ERROR("Wrong OpCode");
        }
    }
};

TEST_P(BitTest, givenZeroAndResultWhenExecutingBitThenZeroFlagIsSet) {
    auto run = [&](u8 regA, u8 value, bool shouldSet) {
        initializeProcessor(GetParam(), value, regA);

        flags.expectZeroFlag(shouldSet);
        flags.ignoreCarryFlag();
        flags.ignoreOverflowFlag();

        processor.regs.pc = startAddress;
        processor.executeInstructions(1);
        flags.expect();
    };

    run(0b1111'1111, 0b1111'1011, false);
    run(0b1111'1111, 0b1111'1111, false);
    run(0b0000'0001, 0b0000'1001, false);
    run(0b1111'0000, 0b0000'1111, true);
    run(0b0000'1111, 0b0000'0000, true);
}

TEST_P(BitTest, givenDifferentMemoryValuesThenBit6AndBit7AreMovedToOverflowAndNegativeFlag) {
    auto run = [&](u8 regA, u8 value, bool bit7, bool bit6) {
        initializeProcessor(GetParam(), value, regA);

        flags.ignoreZeroFlag();
        flags.expectNegativeFlag(bit7);
        flags.expectOverflowFlag(bit6);

        processor.regs.pc = startAddress;
        processor.executeInstructions(1);
        flags.expect();
    };

    run(0b1011'1111, 0b0011'1011, false, false);
    run(0b1011'1111, 0b0111'1011, false, true);
    run(0b1011'1111, 0b1011'1011, true, false);
    run(0b1011'1111, 0b1111'1011, true, true);
}

OpCode opcodesBit[] = {OpCode::BIT_z, OpCode::BIT_abs};
INSTANTIATE_TEST_SUITE_P(, BitTest, ::testing::ValuesIn(opcodesBit), BitTest::constructParamName);
