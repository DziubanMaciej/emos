#include "src/error.h"
#include "test/fixtures/emos_test.h"

struct JsrRtsTest : EmosTest {
    static u8 retAddressLo(u16 address) {
        address--;
        address &= 0xFF;
        return static_cast<u8>(address);
    }
    static u8 retAddressHi(u16 address) {
        address--;
        address &= 0xFF00;
        address >>= 8;
        return static_cast<u8>(address);
    }
};

TEST_F(JsrRtsTest, whenProcessingJsrThenPushAddressOnStackAndJump) {
    processor.regs.sp = 0xFF;

    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::JSR);
    processor.memory[startAddress + 1] = 0x20;
    processor.memory[startAddress + 2] = 0x12;

    processor.executeInstructions(1);

    const u16 returnAddress = startAddress + 3;
    EXPECT_EQ(retAddressLo(returnAddress), processor.memory[0x1FE]);
    EXPECT_EQ(retAddressHi(returnAddress), processor.memory[0x1FF]);
    EXPECT_EQ(0xFD, processor.regs.sp);
    EXPECT_EQ(0x1220, processor.regs.pc);

    expectedCyclesProcessed = 6;
    expectedBytesProcessed = 3;
}

TEST_F(JsrRtsTest, whenProcessingRtsThenPopAddressFromStackAndJump) {
    const u16 returnAddress = 0x4050;

    processor.regs.sp = 0xC0;
    processor.memory[0x1C1] = returnAddress & 0xFF;
    processor.memory[0x1C2] = (returnAddress & 0xFF00) >> 8;

    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::RTS);

    processor.executeInstructions(1);

    EXPECT_EQ(0xC2, processor.regs.sp);
    EXPECT_EQ(returnAddress + 1, processor.regs.pc);

    expectedCyclesProcessed = 6;
    expectedBytesProcessed = 1;
}

TEST_F(JsrRtsTest, givenSubroutineCallThenJumpAndReturn) {
    processor.regs.sp = 0xFF;
    processor.regs.x = 0xDD;
    processor.regs.y = 0xCC;

    // Subroutine
    const u16 subroutineAddress = 0x1220;
    processor.memory[subroutineAddress + 0] = static_cast<u8>(OpCode::INY); // 2 cycles
    processor.memory[subroutineAddress + 1] = static_cast<u8>(OpCode::INY); // 2 cycles
    processor.memory[subroutineAddress + 2] = static_cast<u8>(OpCode::RTS); // 6 cycles

    // Main routine
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::JSR); // 6 cycles
    processor.memory[startAddress + 1] = subroutineAddress & 0xFF;
    processor.memory[startAddress + 2] = (subroutineAddress & 0xFF00) >> 8;
    processor.memory[startAddress + 3] = static_cast<u8>(OpCode::INX); // 2 cycles
    processor.memory[startAddress + 4] = static_cast<u8>(OpCode::INX); // 2 cycles

    processor.executeInstructions(6);
    EXPECT_EQ(0xFF, processor.regs.sp);
    EXPECT_EQ(0xDF, processor.regs.x);
    EXPECT_EQ(0xCE, processor.regs.y);
    EXPECT_EQ(startAddress + 5, processor.regs.pc);

    expectedCyclesProcessed = 20;
    expectedBytesProcessed = 8;
}
