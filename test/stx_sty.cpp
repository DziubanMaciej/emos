#include "src/error.h"
#include "test/fixtures/emos_test.h"

struct StxStyTest : testing::WithParamInterface<OpCode>, EmosTest {
    u8 &getReg(OpCode opCode) {
        switch (opCode) {
        case OpCode::STA_z:
        case OpCode::STA_zx:
        case OpCode::STA_abs:
        case OpCode::STA_absx:
        case OpCode::STA_absy:
        case OpCode::STA_ix:
        case OpCode::STA_iy:
            return processor.regs.a;
        case OpCode::STX_z:
        case OpCode::STX_zy:
        case OpCode::STX_abs:
            return processor.regs.x;
        case OpCode::STY_z:
        case OpCode::STY_zx:
        case OpCode::STY_abs:
            return processor.regs.y;
        default:
            FATAL_ERROR("Wrong OpCode");
        }
    }
    void initializeProcessor(OpCode opcode, [[maybe_unused]] std::optional<u8>, std::optional<u8> loadToReg) override {
        getReg(opcode) = loadToReg.value();
        auto dummyValue = 0x07;
        switch (opcode) {
        case OpCode::STA_z:
        case OpCode::STX_z:
        case OpCode::STY_z:
            initializeForZeroPage(opcode, dummyValue);
            expectedBytesProcessed = 2;
            expectedCyclesProcessed = 3;
            return;
        case OpCode::STX_zy:
            initializeForZeroPageY(opcode, dummyValue);
            expectedBytesProcessed = 2;
            expectedCyclesProcessed = 4;
            return;
        case OpCode::STA_zx:
        case OpCode::STY_zx:
            initializeForZeroPageX(opcode, dummyValue);
            expectedBytesProcessed = 2;
            expectedCyclesProcessed = 4;
            return;
        case OpCode::STA_abs:
        case OpCode::STX_abs:
        case OpCode::STY_abs:
            initializeForAbsolute(opcode, dummyValue);
            expectedBytesProcessed = 3;
            expectedCyclesProcessed = 4;
            return;
        case OpCode::STA_absx:
            initializeForAbsoluteX(opcode, dummyValue);
            expectedBytesProcessed = 3;
            expectedCyclesProcessed = 5;
            return;
        case OpCode::STA_absy:
            initializeForAbsoluteY(opcode, dummyValue);
            expectedBytesProcessed = 3;
            expectedCyclesProcessed = 5;
            return;
        case OpCode::STA_ix:
            initializeForIndirectX(opcode, dummyValue);
            expectedBytesProcessed = 2;
            expectedCyclesProcessed = 6;
            return;
        case OpCode::STA_iy:
            initializeForIndirectY(opcode, dummyValue);
            expectedBytesProcessed = 2;
            expectedCyclesProcessed = 6;
            return;
        default:
            FATAL_ERROR("Wrong OpCode");
        }
    }
    static std::string constructParamName(const testing::TestParamInfo<OpCode> &info) {
        OpCode opCode = info.param;
        switch (opCode) {
        case OpCode::STA_z:
            return "STA_z";
        case OpCode::STA_zx:
            return "STA_zx";
        case OpCode::STA_abs:
            return "STA_abs";
        case OpCode::STA_absx:
            return "STA_absx";
        case OpCode::STA_absy:
            return "STA_absy";
        case OpCode::STA_ix:
            return "STA_ix";
        case OpCode::STA_iy:
            return "STA_iy";
        case OpCode::STX_z:
            return "STX_z";
        case OpCode::STX_zy:
            return "STX_zy";
        case OpCode::STX_abs:
            return "STX_abs";
        case OpCode::STY_z:
            return "STY_z";
        case OpCode::STY_zx:
            return "STY_zx";
        case OpCode::STY_abs:
            return "STY_abs";
        default:
            FATAL_ERROR("Wrong OpCode");
        }
    }
};

using StxTest = StxStyTest;
TEST_P(StxTest, givenDifferentStxOpcodesWhenInitializeProcessorThenValueFromRegXIsStoredToMemory) {
    u8 loadToRegX = 0x16;

    initializeProcessor(GetParam(), std::nullopt, loadToRegX);
    processor.executeInstructions(1);

    EXPECT_EQ(loadToRegX, referencedValue.read());
}

INSTANTIATE_TEST_SUITE_P(,
                         StxTest,
                         ::testing::ValuesIn({OpCode::STX_z, OpCode::STX_zy, OpCode::STX_abs}),
                         StxTest::constructParamName);

using StyTest = StxStyTest;
TEST_P(StyTest, givenDifferentStyOpcodesWhenInitializeProcessorThenValueFromRegYIsStoredToMemory) {
    u8 loadToRegY = 0x81;

    initializeProcessor(GetParam(), std::nullopt, loadToRegY);
    processor.executeInstructions(1);

    EXPECT_EQ(loadToRegY, referencedValue.read());
}

INSTANTIATE_TEST_SUITE_P(,
                         StyTest,
                         ::testing::ValuesIn({
                             OpCode::STA_z,
                             OpCode::STA_zx,
                             OpCode::STA_abs,
                             OpCode::STA_absx,
                             OpCode::STA_absy,
                             OpCode::STA_ix,
                             OpCode::STA_iy,
                             OpCode::STX_z,
                             OpCode::STX_zy,
                             OpCode::STX_abs,
                             OpCode::STY_z,
                             OpCode::STY_zx,
                             OpCode::STY_abs,
                         }),
                         StyTest::constructParamName);
