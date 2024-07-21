#pragma once

#include "test/fixtures/whitebox.h"

#include <gtest/gtest.h>

class FlagsTracker {
public:
    void setUp(WhiteboxProcessor &processor) {
        actualFlags = &processor.regs.flags;

        assertionMask.c = 1;
        assertionMask.z = 1;
        assertionMask.i = 1;
        assertionMask.d = 1;
        assertionMask.b = 1;
        assertionMask.o = 1;
        assertionMask.n = 1;
        assertionMask.r = 1;

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

    void expect() {
#define ASSERTION(fieldName)                                        \
    if (assertionMask.fieldName) {                                  \
        EXPECT_EQ(expectedFlags.fieldName, actualFlags->fieldName); \
    }
        ASSERTION(c)
        ASSERTION(z)
        ASSERTION(i)
        ASSERTION(d)
        ASSERTION(b)
        ASSERTION(o)
        ASSERTION(r)
#undef ASSERTION
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
    }                                                      \
                                                           \
    void ignore##flagName##Flag() {                        \
        assertionMask.fieldName = 0;                       \
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

    // Each bit in this mask selects whether we should make an assertion about
    // given status flag or ignore it.
    StatusFlags assertionMask = {};
};
