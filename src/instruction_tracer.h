#pragma once

#include "src/error.h"
#include "src/instructions.h"
#include "src/registers.h"

#include <cstdarg>
#include <sstream>

class InstructionTracer {
public:
    void beginInstruction(u32 newInstructionIndex, OpCode newOpCode, const char *newMnemonic, u16 newPc) {
        FATAL_ERROR_IF(newOpCode == OpCode::_INVALID, "Invalid opcode");
        FATAL_ERROR_IF(newMnemonic == nullptr, "Invalid mnemonic");

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
        INFO("%08d   OpCode=0x%02x (%s)   PC=0x%04x    Flags=%s%s",
             instructionIndex, static_cast<int>(opCode), mnemonic,
             pc, flags.toString().c_str(), extraData.str().c_str());

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
