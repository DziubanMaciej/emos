#pragma once

#include "types.h"

// Counters are not an integral part of the processor, but are used to aid testing and debugging.
struct Counters {
    u32 bytesProcessed = 0;
    u32 cyclesProcessed = 0;
};
