#include "src/error.h"
#include "src/processor.h"

#include <fstream>

int main() {
    // These numbers are taken from .lst file. They could change if we change the program
    // or assembler, but it's not very likely, so let's just hardcode them.
    const u32 binaryStartOffset = 0x000A;
    const u32 binarySize = 65526;
    const u16 programStart = 0x400;

    // Read program from file
    std::ifstream file{TEST_BINARY_FILE, std::ios::in | std::ios::binary};
    u8 binary[binarySize];
    file.read(reinterpret_cast<char *>(binary), binarySize);
    FATAL_ERROR_IF(!file, "Failed loading " TEST_BINARY_FILE);

    // Execute the program. If it ends, it's a success.
    Processor processor{};
    processor.loadMemory(binaryStartOffset, binarySize, binary);
    processor.loadProgramCounter(programStart);
    processor.activateHangDetector();
    processor.executeInstructions(0);
}
