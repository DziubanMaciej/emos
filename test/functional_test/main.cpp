#include "src/error.h"
#include "src/processor.h"

#include <fstream>

int main() {
    // These numbers are taken from .lst files. They could change if we change the programs
    // or assembler, but it's not very likely, so let's just hardcode them.
#if TEST_INDEX == 0
    // Functional test
    const u32 binaryStartOffset = 0x000A;
    const u32 binarySize = 65526;
    const u16 programStartAddress = 0x0400;
    const u16 programSuccessAddress = 0x336d;
#elif TEST_INDEX == 1
    // Decimal test
    const u32 binaryStartOffset = 0x0200;
    const u32 binarySize = 234;
    const u16 programStartAddress = 0x0200;
    const u16 programSuccessAddress = 0x024b; // not sure about that one
#elif TEST_INDEX == 2
    // Interrupt test
    const u32 binaryStartOffset = 0x000A;
    const u32 binarySize = 65526;
    const u16 programStartAddress = 0x0400;
    const u16 programSuccessAddress = 0x06f5;
#else
#error "Invalid test index"
#endif

    // Read program from file
    std::ifstream file{TEST_BINARY_FILE, std::ios::in | std::ios::binary};
    u8 binary[binarySize];
    file.read(reinterpret_cast<char *>(binary), binarySize);
    FATAL_ERROR_IF(!file, "Failed loading " TEST_BINARY_FILE);

    // Execute the program. If it ends, it's a success.
    Processor processor{};
    processor.loadMemory(binaryStartOffset, binarySize, binary);
    processor.loadProgramCounter(programStartAddress);
    processor.activateHangDetector();
    // processor.activateInstructionTracing();
    processor.executeInstructions(0);

    // Verify success. The test program will always hang, but one designated location means
    // it actually succeeded.
    const u16 hangAddress = processor.getHangAddress();
    if (hangAddress == programSuccessAddress) {
        return 0;
    } else {
        INFO("Hang detected at 0x%04x", hangAddress);
        return 1;
    }
}
