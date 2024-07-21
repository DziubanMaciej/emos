#pragma once

#include "src/processor.h"

struct WhiteboxProcessor : Processor {
    using Processor::counters;
    using Processor::memory;
    using Processor::regs;
};
