#include "src/error.h"
#include "unit_test/fixtures/emos_test.h"

enum class Register : char {
    A = 'A',
    X = 'X',
    Y = 'Y',
    S = 'S',
};

using Param = std::tuple<Register, Register>;

struct RegisterTransferTest : EmosTest, ::testing::WithParamInterface<Param> {
    static OpCode getOpcode() {
        const Register src = std::get<0>(GetParam());
        const Register dst = std::get<1>(GetParam());

        if (src == Register::A && dst == Register::X) {
            return OpCode::TAX;
        } else if (src == Register::A && dst == Register::Y) {
            return OpCode::TAY;
        } else if (src == Register::X && dst == Register::A) {
            return OpCode::TXA;
        } else if (src == Register::Y && dst == Register::A) {
            return OpCode::TYA;
        } else if (src == Register::S && dst == Register::X) {
            return OpCode::TSX;
        } else if (src == Register::X && dst == Register::S) {
            return OpCode::TXS;
        } else {
            UNREACHABLE_CODE();
        }
    }

    u8 &getReg(Register reg) {
        switch (reg) {
        case Register::A:
            return processor.regs.a;
        case Register::X:
            return processor.regs.x;
        case Register::Y:
            return processor.regs.y;
        case Register::S:
            return processor.regs.sp;
        default:
            UNREACHABLE_CODE();
        }
    }

    static bool shouldSetNZRegisters() {
        return getOpcode() != OpCode::TXS;
    }

    ReferencedValue initializeProcessor(OpCode opcode, [[maybe_unused]] std::optional<u8> value, std::optional<u8> loadToReg) {
        const Register src = std::get<0>(GetParam());
        const Register dst = std::get<1>(GetParam());

        processor.memory[startAddress + 0] = static_cast<u8>(opcode);
        getReg(src) = loadToReg.value();
        expectedBytesProcessed = 1;
        expectedCyclesProcessed = 2;

        ReferencedValue referencedValue;
        referencedValue.set(&getReg(dst));
        return referencedValue;
    }

    static std::string constructParamName(const testing::TestParamInfo<Param> &info) {
        const Register src = std::get<0>(info.param);
        const Register dst = std::get<1>(info.param);

        std::string result = "T__";
        result[1] = static_cast<char>(src);
        result[2] = static_cast<char>(dst);
        return result;
    }
};

TEST_P(RegisterTransferTest, givenNegativeValueThenProcessInstruction) {
    u8 loadToReg = 0x80;
    ReferencedValue referencedValue = initializeProcessor(getOpcode(), std::nullopt, loadToReg);

    if (shouldSetNZRegisters()) {
        flags.expectZeroFlag(false);
        flags.expectNegativeFlag(true);
    }
    processor.executeInstructions(1);
    EXPECT_EQ(0x80, referencedValue.read());
}

TEST_P(RegisterTransferTest, givenPositiveValueThenProcessInstruction) {
    u8 loadToReg = 0x70;
    ReferencedValue referencedValue = initializeProcessor(getOpcode(), std::nullopt, loadToReg);

    if (shouldSetNZRegisters()) {
        flags.expectZeroFlag(false);
        flags.expectNegativeFlag(false);
    }
    processor.executeInstructions(1);
    EXPECT_EQ(0x70, referencedValue.read());
}

TEST_P(RegisterTransferTest, givenZeroValueThenProcessInstruction) {
    u8 loadToReg = 0x00;
    ReferencedValue referencedValue = initializeProcessor(getOpcode(), std::nullopt, loadToReg);

    if (shouldSetNZRegisters()) {
        flags.expectZeroFlag(true);
        flags.expectNegativeFlag(false);
    }
    processor.executeInstructions(1);
    EXPECT_EQ(0x00, referencedValue.read());
}

INSTANTIATE_TEST_SUITE_P(,
                         RegisterTransferTest,
                         ::testing::ValuesIn({
                             Param{Register::X, Register::A},
                             Param{Register::A, Register::X},
                             Param{Register::Y, Register::A},
                             Param{Register::A, Register::Y},
                             Param{Register::S, Register::X},
                             Param{Register::X, Register::S},
                         }),
                         RegisterTransferTest::constructParamName);
