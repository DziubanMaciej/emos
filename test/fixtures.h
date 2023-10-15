#pragma once

#include "src/processor.h"

#include <gtest/gtest.h>

struct WhiteboxProcessor : Processor {
    using Processor::counters;
    using Processor::memory;
    using Processor::regs;
};

struct EmosTest : ::testing::Test {
    void SetUp() {
        // Set PC to an arbitrary value - we have to start somewhere.
        startAddress = 0xFF00;
        processor.regs.pc = startAddress;

        // Set registers to bogus values, to make sure they are not used accidentally.
        processor.regs.a = 0x13;
        processor.regs.x = 0x23;
        processor.regs.y = 0x33;
        processor.regs.sp = 0x43;
        processor.regs.flags.c = 1;
        processor.regs.flags.z = 1;
        processor.regs.flags.i = 1;
        processor.regs.flags.d = 1;
        processor.regs.flags.b = 1;
        processor.regs.flags.o = 1;
        processor.regs.flags.n = 1;
        processor.regs.flags.r = 1;
    }

    u16 startAddress = {};
    WhiteboxProcessor processor = {};
};
