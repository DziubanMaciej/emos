#pragma once

#include "test/fixtures/whitebox.h"

#include <gtest/gtest.h>

struct ReferencedValue {
    u8 read() {
        EXPECT_NE(nullptr, value);
        return *value;
    }

    void setMemory(WhiteboxProcessor &processor, u16 address) {
        EXPECT_EQ(nullptr, value); // we should set this only once
        value = &processor.memory[address];
    }

    void setRegisterA(WhiteboxProcessor &processor) {
        EXPECT_EQ(nullptr, value); // we should set this only once
        value = &processor.regs.a;
    }

    void setImmediate(u8 v) {
        EXPECT_EQ(nullptr, value); // we should set this only once
        immediate = v;
        value = &immediate;
    }

private:
    u8 immediate = {};
    u8 *value = nullptr;
};
