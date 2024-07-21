#include "src/error.h"
#include "test/fixtures/emos_test.h"

struct NopTest : EmosTest {};

TEST_F(NopTest, givenProcessorThenExecuteNop) {
    initializeForImplied(OpCode::NOP);
    processor.executeInstructions(1);

    expectedBytesProcessed = 1;
    expectedCyclesProcessed = 2;
}
