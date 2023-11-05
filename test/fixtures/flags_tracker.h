#pragma once

#include "test/fixtures/whitebox.h"

#include <gtest/gtest.h>

class FlagsTracker {
public:
    void setUp(WhiteboxProcessor &processor) {
        actualFlags = &processor.regs.flags;

        actualFlags->c = 0;
        actualFlags->z = 0;
        actualFlags->i = 0;
        actualFlags->d = 0;
        actualFlags->b = 0;
        actualFlags->o = 0;
        actualFlags->n = 0;
        actualFlags->r = 0;
        expectedFlags = *actualFlags;
    }

    void tearDown() {
        EXPECT_EQ(expectedFlags.c, actualFlags->c);
        EXPECT_EQ(expectedFlags.z, actualFlags->z);
        EXPECT_EQ(expectedFlags.i, actualFlags->i);
        EXPECT_EQ(expectedFlags.d, actualFlags->d);
        EXPECT_EQ(expectedFlags.b, actualFlags->b);
        EXPECT_EQ(expectedFlags.o, actualFlags->o);
        EXPECT_EQ(expectedFlags.r, actualFlags->r);
    }

#define EXPECT_FUNC(flagName, fieldName)                   \
    void expect##flagName##Flag(bool before, bool after) { \
        actualFlags->fieldName = before;                   \
        expectedFlags.fieldName = after;                   \
    }                                                      \
                                                           \
    void expect##flagName##Flag(bool after) {              \
        actualFlags->fieldName = !after;                   \
        expectedFlags.fieldName = after;                   \
    }

    EXPECT_FUNC(Carry, c)
    EXPECT_FUNC(Zero, z)
    EXPECT_FUNC(Interrupt, i)
    EXPECT_FUNC(Decimal, d)
    EXPECT_FUNC(Break, b)
    EXPECT_FUNC(Overflow, o)
    EXPECT_FUNC(Negative, n)
    EXPECT_FUNC(Reserved, r)
#undef EXPECT_FUNC

private:
    StatusFlags *actualFlags = nullptr;
    StatusFlags expectedFlags{};
};
