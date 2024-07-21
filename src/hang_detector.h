#pragma once

#include "src/instructions.h"

class HangDetector {
public:
    void instruction(OpCode opCode, u16 pc) {
        if (hangDetected) {
            return;
        }

        snapshots[snapshotIndex] = StateSnapshot{opCode, pc};
        snapshotIndex = (snapshotIndex + 1) % snapshotsCount;

        for (u32 i = 1; i < snapshotsCount; i++) {
            if (snapshots[0] != snapshots[i]) {
                return;
            }
        }

        hangDetected = true;
        hangAddress = snapshots[0].pc;
    }

    bool isHangDetected() const { return hangDetected; }
    u16 getHangAddress() const { return hangAddress; }

private:
    struct StateSnapshot {
        OpCode opCode = OpCode::_INVALID;
        u16 pc = 0xFFFF;

        bool operator!=(const StateSnapshot &other) const {
            return opCode != other.opCode || pc != other.pc;
        }
    };
    constexpr static u32 snapshotsCount = 10;
    StateSnapshot snapshots[snapshotsCount] = {};
    u32 snapshotIndex = 0;

    bool hangDetected = false;
    u16 hangAddress = 0;
};