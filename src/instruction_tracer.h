#pragma once

#include "src/error.h"
#include "src/instructions.h"
#include "src/registers.h"

#include <iomanip>
#include <stdarg.h>

class InstructionTracer {
public:
    void beginInstruction(u32 newInstructionIndex, OpCode newOpCode, const char *newMnemonic, u16 newPc) {
        FATAL_ERROR_IF(newOpCode == OpCode::_INVALID, "Invalid opcode");
        FATAL_ERROR_IF(newMnemonic == nullptr, "Invalid menmonic");

        instructionIndex = newInstructionIndex;
        opCode = newOpCode;
        mnemonic = newMnemonic;
        pc = newPc;
    }

    void extra(const char *format, ...) {
        char buffer[1024];

        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);

        extraData << "   " << buffer;
    }

    void endInstruction(StatusFlags flags) {
        INFO(
            std::setfill('0'), std::setw(8), std::dec, instructionIndex, ".",
            "   OpCode=0x", std::setfill('0'), std::setw(2), std::hex, static_cast<int>(opCode),
            " (", mnemonic, ")",
            "    PC=0x", std::setw(4), std::hex, pc,
            "    Flags=", flags.toString(),
            extraData.str());

        instructionIndex = {};
        opCode = OpCode::_INVALID;
        mnemonic = {};
        pc = {};
        extraData = {};
    }

private:
    u32 instructionIndex = {};
    OpCode opCode = OpCode::_INVALID;
    const char *mnemonic = {};
    u16 pc = {};
    std::ostringstream extraData = {};
};
