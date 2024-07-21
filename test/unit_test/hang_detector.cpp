#include "src/bit_operations.h"
#include "src/error.h"
#include "unit_test/fixtures/emos_test.h"

struct HangDetectorTest : EmosTest {};

TEST_F(HangDetectorTest, givenNoHungInstructionWhenHangDetectorIsActiveThenDoNotDetectHang) {
    std::fill_n(&processor.memory[startAddress], 50, static_cast<u8>(OpCode::INX));

    processor.activateHangDetector();
    ASSERT_TRUE(processor.executeInstructions(50));

    processor.counters.bytesProcessed = 0;
    processor.counters.cyclesProcessed = 0;
}

TEST_F(HangDetectorTest, givenHungInstructionWhenHangDetectorIsActiveThenDetectHang) {
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::JMP_abs);
    processor.memory[startAddress + 1] = lo(startAddress);
    processor.memory[startAddress + 2] = hi(startAddress);

    processor.activateHangDetector();
    ASSERT_FALSE(processor.executeInstructions(50));

    processor.counters.bytesProcessed = 0;
    processor.counters.cyclesProcessed = 0;
}

TEST_F(HangDetectorTest, givenHungInstructionWhenHangDetectorIsInactiveThenDoNotDetectHang) {
    processor.memory[startAddress + 0] = static_cast<u8>(OpCode::JMP_abs);
    processor.memory[startAddress + 1] = lo(startAddress);
    processor.memory[startAddress + 2] = hi(startAddress);

    ASSERT_TRUE(processor.executeInstructions(50));

    processor.counters.bytesProcessed = 0;
    processor.counters.cyclesProcessed = 0;
}
