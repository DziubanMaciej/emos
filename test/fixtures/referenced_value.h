#pragma once

#include "test/fixtures/whitebox.h"

#include <gtest/gtest.h>

struct ReferencedValue {
    ReferencedValue() = default;

    ReferencedValue &operator=(ReferencedValue &&other) noexcept {
        this->value = other.value;
        this->immediate = other.immediate;
        other.value = {};
        other.immediate = {};
        return *this;
    }

    ReferencedValue(ReferencedValue &&other) noexcept {
        *this = std::move(other);
    }

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

    void set(u8 *address) {
        EXPECT_EQ(nullptr, value); // we should set this only once
        value = address;
    }

    void setImmediate(u8 v) {
        EXPECT_EQ(nullptr, value); // we should set this only once
        immediate = v;
        value = &immediate;
    }

private:
    u8 immediate = {};
    u8 *value = {};
};
