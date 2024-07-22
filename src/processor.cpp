#include "bit_operations.h"
#include "error.h"
#include "processor.h"

#include <cstring>
#include <limits>

#define INSTRUCTION_TRACE(...)                                  \
    do {                                                        \
        if (debugFeatures.instructionTracingActive) {           \
            debugFeatures.instructionTracer.extra(__VA_ARGS__); \
        }                                                       \
    } while (0)

Processor::Processor() {
    setInstructionData("LDA", OpCode::LDA_imm, AddressingMode::Immediate, &Processor::executeLda);
    setInstructionData("LDA", OpCode::LDA_z, AddressingMode::ZeroPage, &Processor::executeLda);
    setInstructionData("LDA", OpCode::LDA_zx, AddressingMode::ZeroPageX, &Processor::executeLda);
    setInstructionData("LDA", OpCode::LDA_abs, AddressingMode::Absolute, &Processor::executeLda);
    setInstructionData("LDA", OpCode::LDA_absx, AddressingMode::AbsoluteX, &Processor::executeLda);
    setInstructionData("LDA", OpCode::LDA_absy, AddressingMode::AbsoluteY, &Processor::executeLda);
    setInstructionData("LDA", OpCode::LDA_ix, AddressingMode::IndexedIndirectX, &Processor::executeLda);
    setInstructionData("LDA", OpCode::LDA_iy, AddressingMode::IndirectIndexedY, &Processor::executeLda);

    setInstructionData("LDX", OpCode::LDX_imm, AddressingMode::Immediate, &Processor::executeLdx);
    setInstructionData("LDX", OpCode::LDX_z, AddressingMode::ZeroPage, &Processor::executeLdx);
    setInstructionData("LDX", OpCode::LDX_zy, AddressingMode::ZeroPageY, &Processor::executeLdx);
    setInstructionData("LDX", OpCode::LDX_abs, AddressingMode::Absolute, &Processor::executeLdx);
    setInstructionData("LDX", OpCode::LDX_absy, AddressingMode::AbsoluteY, &Processor::executeLdx);

    setInstructionData("LDY", OpCode::LDY_imm, AddressingMode::Immediate, &Processor::executeLdy);
    setInstructionData("LDY", OpCode::LDY_z, AddressingMode::ZeroPage, &Processor::executeLdy);
    setInstructionData("LDY", OpCode::LDY_zx, AddressingMode::ZeroPageX, &Processor::executeLdy);
    setInstructionData("LDY", OpCode::LDY_abs, AddressingMode::Absolute, &Processor::executeLdy);
    setInstructionData("LDY", OpCode::LDY_absx, AddressingMode::AbsoluteX, &Processor::executeLdy);

    setInstructionData("INC", OpCode::INC_z, AddressingMode::ZeroPage, &Processor::executeInc);
    setInstructionData("INC", OpCode::INC_zx, AddressingMode::ZeroPageX, &Processor::executeInc);
    setInstructionData("INC", OpCode::INC_abs, AddressingMode::Absolute, &Processor::executeInc);
    setInstructionData("INC", OpCode::INC_absx, AddressingMode::AbsoluteX, &Processor::executeInc);

    setInstructionData("INX", OpCode::INX, AddressingMode::Implied, &Processor::executeInx);
    setInstructionData("INY", OpCode::INY, AddressingMode::Implied, &Processor::executeIny);
    setInstructionData("DEX", OpCode::DEX, AddressingMode::Implied, &Processor::executeDex);
    setInstructionData("DEY", OpCode::DEY, AddressingMode::Implied, &Processor::executeDey);

    setInstructionData("DEC", OpCode::DEC_z, AddressingMode::ZeroPage, &Processor::executeDec);
    setInstructionData("DEC", OpCode::DEC_zx, AddressingMode::ZeroPageX, &Processor::executeDec);
    setInstructionData("DEC", OpCode::DEC_abs, AddressingMode::Absolute, &Processor::executeDec);
    setInstructionData("DEC", OpCode::DEC_absx, AddressingMode::AbsoluteX, &Processor::executeDec);

    setInstructionData("ASL", OpCode::ASL_acc, AddressingMode::Accumulator, &Processor::executeAsl);
    setInstructionData("ASL", OpCode::ASL_z, AddressingMode::ZeroPage, &Processor::executeAsl);
    setInstructionData("ASL", OpCode::ASL_zx, AddressingMode::ZeroPageX, &Processor::executeAsl);
    setInstructionData("ASL", OpCode::ASL_abs, AddressingMode::Absolute, &Processor::executeAsl);
    setInstructionData("ASL", OpCode::ASL_absx, AddressingMode::AbsoluteX, &Processor::executeAsl);

    setInstructionData("LSR", OpCode::LSR_acc, AddressingMode::Accumulator, &Processor::executeLsr);
    setInstructionData("LSR", OpCode::LSR_z, AddressingMode::ZeroPage, &Processor::executeLsr);
    setInstructionData("LSR", OpCode::LSR_zx, AddressingMode::ZeroPageX, &Processor::executeLsr);
    setInstructionData("LSR", OpCode::LSR_abs, AddressingMode::Absolute, &Processor::executeLsr);
    setInstructionData("LSR", OpCode::LSR_absx, AddressingMode::AbsoluteX, &Processor::executeLsr);

    setInstructionData("ROL", OpCode::ROL_acc, AddressingMode::Accumulator, &Processor::executeRol);
    setInstructionData("ROL", OpCode::ROL_z, AddressingMode::ZeroPage, &Processor::executeRol);
    setInstructionData("ROL", OpCode::ROL_zx, AddressingMode::ZeroPageX, &Processor::executeRol);
    setInstructionData("ROL", OpCode::ROL_abs, AddressingMode::Absolute, &Processor::executeRol);
    setInstructionData("ROL", OpCode::ROL_absx, AddressingMode::AbsoluteX, &Processor::executeRol);

    setInstructionData("ROR", OpCode::ROR_acc, AddressingMode::Accumulator, &Processor::executeRor);
    setInstructionData("ROR", OpCode::ROR_z, AddressingMode::ZeroPage, &Processor::executeRor);
    setInstructionData("ROR", OpCode::ROR_zx, AddressingMode::ZeroPageX, &Processor::executeRor);
    setInstructionData("ROR", OpCode::ROR_abs, AddressingMode::Absolute, &Processor::executeRor);
    setInstructionData("ROR", OpCode::ROR_absx, AddressingMode::AbsoluteX, &Processor::executeRor);

    setInstructionData("TAX", OpCode::TAX, AddressingMode::Implied, &Processor::executeTax);
    setInstructionData("TAY", OpCode::TAY, AddressingMode::Implied, &Processor::executeTay);
    setInstructionData("TXA", OpCode::TXA, AddressingMode::Implied, &Processor::executeTxa);
    setInstructionData("TYA", OpCode::TYA, AddressingMode::Implied, &Processor::executeTya);
    setInstructionData("TSX", OpCode::TSX, AddressingMode::Implied, &Processor::executeTsx);
    setInstructionData("TXS", OpCode::TXS, AddressingMode::Implied, &Processor::executeTxs);

    setInstructionData("PHA", OpCode::PHA, AddressingMode::Implied, &Processor::executePha);
    setInstructionData("PHP", OpCode::PHP, AddressingMode::Implied, &Processor::executePhp);
    setInstructionData("PLA", OpCode::PLA, AddressingMode::Implied, &Processor::executePla);
    setInstructionData("PLP", OpCode::PLP, AddressingMode::Implied, &Processor::executePlp);

    setInstructionData("CMP", OpCode::CMP_imm, AddressingMode::Immediate, &Processor::executeCmp);
    setInstructionData("CMP", OpCode::CMP_z, AddressingMode::ZeroPage, &Processor::executeCmp);
    setInstructionData("CMP", OpCode::CMP_zx, AddressingMode::ZeroPageX, &Processor::executeCmp);
    setInstructionData("CMP", OpCode::CMP_abs, AddressingMode::Absolute, &Processor::executeCmp);
    setInstructionData("CMP", OpCode::CMP_absx, AddressingMode::AbsoluteX, &Processor::executeCmp);
    setInstructionData("CMP", OpCode::CMP_absy, AddressingMode::AbsoluteY, &Processor::executeCmp);
    setInstructionData("CMP", OpCode::CMP_ix, AddressingMode::IndexedIndirectX, &Processor::executeCmp);
    setInstructionData("CMP", OpCode::CMP_iy, AddressingMode::IndirectIndexedY, &Processor::executeCmp);

    setInstructionData("CPX", OpCode::CPX_imm, AddressingMode::Immediate, &Processor::executeCpx);
    setInstructionData("CPX", OpCode::CPX_z, AddressingMode::ZeroPage, &Processor::executeCpx);
    setInstructionData("CPX", OpCode::CPX_abs, AddressingMode::Absolute, &Processor::executeCpx);

    setInstructionData("CPY", OpCode::CPY_imm, AddressingMode::Immediate, &Processor::executeCpy);
    setInstructionData("CPY", OpCode::CPY_z, AddressingMode::ZeroPage, &Processor::executeCpy);
    setInstructionData("CPY", OpCode::CPY_abs, AddressingMode::Absolute, &Processor::executeCpy);

    setInstructionData("ADC", OpCode::ADC_imm, AddressingMode::Immediate, &Processor::executeAdc);
    setInstructionData("ADC", OpCode::ADC_z, AddressingMode::ZeroPage, &Processor::executeAdc);
    setInstructionData("ADC", OpCode::ADC_zx, AddressingMode::ZeroPageX, &Processor::executeAdc);
    setInstructionData("ADC", OpCode::ADC_abs, AddressingMode::Absolute, &Processor::executeAdc);
    setInstructionData("ADC", OpCode::ADC_absx, AddressingMode::AbsoluteX, &Processor::executeAdc);
    setInstructionData("ADC", OpCode::ADC_absy, AddressingMode::AbsoluteY, &Processor::executeAdc);
    setInstructionData("ADC", OpCode::ADC_ix, AddressingMode::IndexedIndirectX, &Processor::executeAdc);
    setInstructionData("ADC", OpCode::ADC_iy, AddressingMode::IndirectIndexedY, &Processor::executeAdc);

    setInstructionData("AND", OpCode::AND_imm, AddressingMode::Immediate, &Processor::executeAnd);
    setInstructionData("AND", OpCode::AND_z, AddressingMode::ZeroPage, &Processor::executeAnd);
    setInstructionData("AND", OpCode::AND_zx, AddressingMode::ZeroPageX, &Processor::executeAnd);
    setInstructionData("AND", OpCode::AND_abs, AddressingMode::Absolute, &Processor::executeAnd);
    setInstructionData("AND", OpCode::AND_absx, AddressingMode::AbsoluteX, &Processor::executeAnd);
    setInstructionData("AND", OpCode::AND_absy, AddressingMode::AbsoluteY, &Processor::executeAnd);
    setInstructionData("AND", OpCode::AND_ix, AddressingMode::IndexedIndirectX, &Processor::executeAnd);
    setInstructionData("AND", OpCode::AND_iy, AddressingMode::IndirectIndexedY, &Processor::executeAnd);

    setInstructionData("EOR", OpCode::EOR_imm, AddressingMode::Immediate, &Processor::executeEor);
    setInstructionData("EOR", OpCode::EOR_z, AddressingMode::ZeroPage, &Processor::executeEor);
    setInstructionData("EOR", OpCode::EOR_zx, AddressingMode::ZeroPageX, &Processor::executeEor);
    setInstructionData("EOR", OpCode::EOR_abs, AddressingMode::Absolute, &Processor::executeEor);
    setInstructionData("EOR", OpCode::EOR_absx, AddressingMode::AbsoluteX, &Processor::executeEor);
    setInstructionData("EOR", OpCode::EOR_absy, AddressingMode::AbsoluteY, &Processor::executeEor);
    setInstructionData("EOR", OpCode::EOR_ix, AddressingMode::IndexedIndirectX, &Processor::executeEor);
    setInstructionData("EOR", OpCode::EOR_iy, AddressingMode::IndirectIndexedY, &Processor::executeEor);

    setInstructionData("BIT", OpCode::BIT_z, AddressingMode::ZeroPage, &Processor::executeBit);
    setInstructionData("BIT", OpCode::BIT_abs, AddressingMode::Absolute, &Processor::executeBit);

    setInstructionData("ORA", OpCode::ORA_imm, AddressingMode::Immediate, &Processor::executeOra);
    setInstructionData("ORA", OpCode::ORA_z, AddressingMode::ZeroPage, &Processor::executeOra);
    setInstructionData("ORA", OpCode::ORA_zx, AddressingMode::ZeroPageX, &Processor::executeOra);
    setInstructionData("ORA", OpCode::ORA_abs, AddressingMode::Absolute, &Processor::executeOra);
    setInstructionData("ORA", OpCode::ORA_absx, AddressingMode::AbsoluteX, &Processor::executeOra);
    setInstructionData("ORA", OpCode::ORA_absy, AddressingMode::AbsoluteY, &Processor::executeOra);
    setInstructionData("ORA", OpCode::ORA_ix, AddressingMode::IndexedIndirectX, &Processor::executeOra);
    setInstructionData("ORA", OpCode::ORA_iy, AddressingMode::IndirectIndexedY, &Processor::executeOra);

    setInstructionData("SEC", OpCode::SEC, AddressingMode::Implied, &Processor::executeSec);
    setInstructionData("SED", OpCode::SED, AddressingMode::Implied, &Processor::executeSed);
    setInstructionData("SEI", OpCode::SEI, AddressingMode::Implied, &Processor::executeSei);

    setInstructionData("CLC", OpCode::CLC, AddressingMode::Implied, &Processor::executeClc);
    setInstructionData("CLD", OpCode::CLD, AddressingMode::Implied, &Processor::executeCld);
    setInstructionData("CLI", OpCode::CLI, AddressingMode::Implied, &Processor::executeCli);
    setInstructionData("CLV", OpCode::CLV, AddressingMode::Implied, &Processor::executeClv);

    setInstructionData("STA", OpCode::STA_z, AddressingMode::ZeroPage, &Processor::executeSta);
    setInstructionData("STA", OpCode::STA_zx, AddressingMode::ZeroPageX, &Processor::executeSta);
    setInstructionData("STA", OpCode::STA_abs, AddressingMode::Absolute, &Processor::executeSta);
    setInstructionData("STA", OpCode::STA_absx, AddressingMode::AbsoluteX, &Processor::executeSta);
    setInstructionData("STA", OpCode::STA_absy, AddressingMode::AbsoluteY, &Processor::executeSta);
    setInstructionData("STA", OpCode::STA_ix, AddressingMode::IndexedIndirectX, &Processor::executeSta);
    setInstructionData("STA", OpCode::STA_iy, AddressingMode::IndirectIndexedY, &Processor::executeSta);

    setInstructionData("STX", OpCode::STX_z, AddressingMode::ZeroPage, &Processor::executeStx);
    setInstructionData("STX", OpCode::STX_zy, AddressingMode::ZeroPageY, &Processor::executeStx);
    setInstructionData("STX", OpCode::STX_abs, AddressingMode::Absolute, &Processor::executeStx);

    setInstructionData("STY", OpCode::STY_z, AddressingMode::ZeroPage, &Processor::executeSty);
    setInstructionData("STY", OpCode::STY_zx, AddressingMode::ZeroPageX, &Processor::executeSty);
    setInstructionData("STY", OpCode::STY_abs, AddressingMode::Absolute, &Processor::executeSty);

    setInstructionData("SBC", OpCode::SBC_imm, AddressingMode::Immediate, &Processor::executeSbc);
    setInstructionData("SBC", OpCode::SBC_z, AddressingMode::ZeroPage, &Processor::executeSbc);
    setInstructionData("SBC", OpCode::SBC_zx, AddressingMode::ZeroPageX, &Processor::executeSbc);
    setInstructionData("SBC", OpCode::SBC_abs, AddressingMode::Absolute, &Processor::executeSbc);
    setInstructionData("SBC", OpCode::SBC_absx, AddressingMode::AbsoluteX, &Processor::executeSbc);
    setInstructionData("SBC", OpCode::SBC_absy, AddressingMode::AbsoluteY, &Processor::executeSbc);
    setInstructionData("SBC", OpCode::SBC_ix, AddressingMode::IndexedIndirectX, &Processor::executeSbc);
    setInstructionData("SBC", OpCode::SBC_iy, AddressingMode::IndirectIndexedY, &Processor::executeSbc);

    setInstructionData("JMP", OpCode::JMP_abs, AddressingMode::Absolute, &Processor::executeJmp);
    setInstructionData("JMP", OpCode::JMP_i, AddressingMode::Indirect, &Processor::executeJmp);

    setInstructionData("JSR", OpCode::JSR, AddressingMode::Absolute, &Processor::executeJsr);

    setInstructionData("RTS", OpCode::RTS, AddressingMode::Implied, &Processor::executeRts);

    setInstructionData("BCC", OpCode::BCC, AddressingMode::Relative, &Processor::executeBcc);
    setInstructionData("BCS", OpCode::BCS, AddressingMode::Relative, &Processor::executeBcs);
    setInstructionData("BEQ", OpCode::BEQ, AddressingMode::Relative, &Processor::executeBeq);
    setInstructionData("BMI", OpCode::BMI, AddressingMode::Relative, &Processor::executeBmi);
    setInstructionData("BNE", OpCode::BNE, AddressingMode::Relative, &Processor::executeBne);
    setInstructionData("BPL", OpCode::BPL, AddressingMode::Relative, &Processor::executeBpl);
    setInstructionData("BVC", OpCode::BVC, AddressingMode::Relative, &Processor::executeBvc);
    setInstructionData("BVS", OpCode::BVS, AddressingMode::Relative, &Processor::executeBvs);

    setInstructionData("NOP", OpCode::NOP, AddressingMode::Implied, &Processor::executeNop);

    setInstructionData("BRK", OpCode::BRK, AddressingMode::Implied, &Processor::executeBrk);

    setInstructionData("RTI", OpCode::RTI, AddressingMode::Implied, &Processor::executeRti);
}

bool Processor::executeInstructions(u32 maxInstructionCount) {
    for (u32 instructionIndex = 0; maxInstructionCount == 0 || instructionIndex < maxInstructionCount; instructionIndex++) {
        const u8 opCode = fetchInstructionByte();
        const InstructionData &instruction = instructionData[opCode];
        if (instruction.exec == nullptr) {
            FATAL_ERROR("Unsupported instruction: ", static_cast<u32>(opCode));
        }

        if (debugFeatures.hangDetectionActive) {
            debugFeatures.hangDetector.instruction(static_cast<OpCode>(opCode), regs.pc - 1);
            if (debugFeatures.hangDetector.isHangDetected()) {
                INFO("Hang detected at 0x", std::hex, debugFeatures.hangDetector.getHangAddress());
                return false;
            }
        }

        if (debugFeatures.instructionTracingActive) {
            debugFeatures.instructionTracer.beginInstruction(instructionIndex, static_cast<OpCode>(opCode), instruction.mnemonic, regs.pc - 1);
        }

        (this->*instruction.exec)(instruction.addressingMode);

        if (debugFeatures.instructionTracingActive) {
            debugFeatures.instructionTracer.endInstruction(regs.flags);
        }
    }

    return true;
}

void Processor::loadMemory(u32 start, u32 length, const u8 *data) {
    FATAL_ERROR_IF(start + length < memorySize, "Out of memory bounds.");
    memcpy(memory + start, data, length);
}

void Processor::loadProgramCounter(u16 newPc) {
    regs.pc = newPc;
}

void Processor::activateHangDetector() {
    debugFeatures.hangDetectionActive = true;
}

void Processor::activateInstructionTracing() {
    debugFeatures.instructionTracingActive = true;
}

u8 Processor::fetchInstructionByte() {
    const u8 result = readByteFromMemory(regs.pc);
    counters.bytesProcessed += 1;
    regs.pc += 1;
    return result;
}

u16 Processor::fetchInstructionTwoBytes() {
    const u16 result = readTwoBytesFromMemory(regs.pc);
    counters.bytesProcessed += 2;
    regs.pc += 2;
    return result;
}

u8 Processor::readByteFromMemory(u16 address) {
    const u8 byte = memory[address];
    counters.cyclesProcessed += 1;
    return byte;
}

u16 Processor::readTwoBytesFromMemory(u16 address) {
    const u8 lo = memory[address];
    const u8 hi = memory[address + 1];
    counters.cyclesProcessed += 2;
    return (hi << 8) | lo;
}

void Processor::writeByteToMemory(u16 address, u8 byte) {
    memory[address] = byte;
    counters.cyclesProcessed += 1;
}

void Processor::writeTwoBytesToMemory(u16 address, u16 bytes) {
    const u8 lo = bytes & 0xFF;
    const u8 hi = (bytes >> 8) & 0xFF;
    memory[address] = lo;
    memory[address + 1] = hi;
    counters.cyclesProcessed += 2;
}

u16 Processor::getAddress(AddressingMode mode, bool isReadOnly) {
    switch (mode) {
    case AddressingMode::Accumulator: {
        FATAL_ERROR("Cannot get address in accumulator addressing mode")
    }
    case AddressingMode::Implied: {
        FATAL_ERROR("Cannot get address in implied addressing mode")
    }
    case AddressingMode::Immediate: {
        FATAL_ERROR("Cannot get address in immediate addressing mode")
    }
    case AddressingMode::ZeroPage: {
        return fetchInstructionByte();
    }
    case AddressingMode::ZeroPageX: {
        return sumAddressesZeroPage(fetchInstructionByte(), regs.x);
    }
    case AddressingMode::ZeroPageY: {
        return sumAddressesZeroPage(fetchInstructionByte(), regs.y);
    }
    case AddressingMode::Absolute: {
        return fetchInstructionTwoBytes();
    }
    case AddressingMode::AbsoluteX: {
        return sumAddresses(fetchInstructionTwoBytes(), regs.x, isReadOnly);
    }
    case AddressingMode::AbsoluteY: {
        return sumAddresses(fetchInstructionTwoBytes(), regs.y, isReadOnly);
    }
    case AddressingMode::IndexedIndirectX: {
        u8 tableAddress = fetchInstructionByte();
        u16 address = sumAddressesZeroPage(tableAddress, regs.x);
        address = readTwoBytesFromMemory(address);
        return address;
    }
    case AddressingMode::IndirectIndexedY: {
        u16 address = fetchInstructionByte();
        address = readTwoBytesFromMemory(address);
        address = sumAddresses(address, regs.y, isReadOnly);
        return address;
    }
    case AddressingMode::Indirect: {
        u16 address = fetchInstructionTwoBytes();
        address = readTwoBytesFromMemory(address);
        return address;
    }
    case AddressingMode::Relative: {
        const i8 offset = static_cast<i8>(fetchInstructionByte());
        return sumAddresses(regs.pc, offset, isReadOnly);
    }
    default: {
        FATAL_ERROR("Unknown addressing mode");
    }
    }
}

u8 Processor::readValue(AddressingMode mode, bool isReadOnly, u16 *outAddress) {
    u16 address{};
    u8 value{};

    switch (mode) {
    case AddressingMode::Accumulator:
        address = 0xFFFF;
        value = regs.a;
        break;
    case AddressingMode::Immediate:
        address = 0xFFFF;
        value = fetchInstructionByte();
        break;
    default:
        address = getAddress(mode, isReadOnly);
        value = readByteFromMemory(address);
        break;
    }

    if (outAddress) {
        *outAddress = address;
    }
    return value;
}

void Processor::writeValue(AddressingMode mode, u8 value, u16 *address) {
    switch (mode) {
    case AddressingMode::Accumulator:
        regs.a = value;
        INSTRUCTION_TRACE("a=0x%02x", regs.a);
        break;
    default: {
        const u16 memoryAddress = address ? *address : getAddress(mode, false);
        writeByteToMemory(memoryAddress, value);
        break;
    }
    }
}

u16 Processor::sumAddresses(u16 base, u16 offset, bool isReadOnly) {
    const u16 result = base + offset;

    const u16 oldPage = base & 0xFF00;
    const u16 newPage = result & 0xFF00;
    const u16 changedPage = oldPage != newPage;

    counters.cyclesProcessed++;
    if (!changedPage && isReadOnly) {
        // For some instructions (e.g. with abs X addressing mode) the latency of addition is hidden
        // by the processor as follows:
        //  - cycle 1: first byte of the address is read
        //  - cycle 2: second byte of the address is read and simultaneously firstByte+X addition is performed
        //
        // If there is no carry in firstByte+x addition, then the latency has been hidden.
        // Otherwise, we need another cycle to incremend secondByte.
        //
        // Latency cannot be hidden in sums for zero page X addressing mode, because we only read 1 byte
        // from the instruction.
        //
        // Latency also cannot be hidden for instructions performing memory write, because the processor performs
        // a dummy memory operation on an incorrect address to achieve it. And clearly, it shouldn't be doing
        // dummy write, as that would corrupt the result.
        //
        // Sources:
        //   https://forums.nesdev.org/viewtopic.php?t=13936
        //   https://retrocomputing.stackexchange.com/q/15621
        hiddenLatencyCycle();
    }
    return result;
}

u16 Processor::sumAddressesZeroPage(u8 base, u8 offset) {
    const u8 result = base + offset; // let it wrap around
    counters.cyclesProcessed++;
    return static_cast<u16>(result);
}

template <typename RegT>
void Processor::registerTransfer(RegT &dst, const RegT &src) {
    static_assert(sizeof(RegT) == 1);
    FATAL_ERROR_IF(&dst == &src, "Cannot do register transfer on one register");
    counters.cyclesProcessed++;
    dst = src;
}

void Processor::aluOperation() {
    counters.cyclesProcessed++;
}

void Processor::idleCycle() {
    counters.cyclesProcessed++;
}

void Processor::hiddenLatencyCycle() {
    counters.cyclesProcessed--;
}

void Processor::updateArithmeticFlags(u8 value) {
    regs.flags.z = value == 0;
    regs.flags.n = isSignBitSet(value);
}

void Processor::updateFlagsAfterComparison(u8 registerValue, u8 inputValue) {
    regs.flags.c = registerValue >= inputValue;
    regs.flags.z = registerValue == inputValue;
    regs.flags.n = registerValue < inputValue;

    INSTRUCTION_TRACE("reg=0x%02x val=0x%02x", registerValue, inputValue);
}

u16 Processor::updateOverflowForSumWithCarry(u8 addend) {
    u16 accumulatorWithCarry = regs.a + regs.flags.c;
    u16 sum = accumulatorWithCarry + addend;

    if (isSignBitSet(addend) == isSignBitSet(regs.a)) {
        regs.flags.o = isSignBitSet(sum) ^ isSignBitSet(addend);
    }
    return sum;
}

void Processor::updateCarryFlagIfOverflow(u16 value, bool OverflowValue) {
    // if true then ADC :1 SBC: 0, if false then ADC :0 SBC: 1
    regs.flags.c = (value > std::numeric_limits<u8>::max()) ? OverflowValue : !OverflowValue;
}

void Processor::sumWithCarry(u8 addend, bool OverflowValue) {
    // sum =  a + addend + c
    regs.flags.o = 0;
    u16 sum = updateOverflowForSumWithCarry(addend);
    updateCarryFlagIfOverflow(sum, OverflowValue);
    regs.a = static_cast<u8>(sum);
    updateArithmeticFlags(regs.a);
}

void Processor::pushToStack(u8 value) {
    // 1 cycle for writing value
    // 1 cycle for decrementing stack pointer
    counters.cyclesProcessed += 2;

    constexpr u16 stackBase = 0x0100;
    const u16 address = stackBase + regs.sp;
    memory[address] = value;
    regs.sp--;

    INSTRUCTION_TRACE("StackPush(memory[0x%04x]<-0x%02x, sp=0x%02x)", address, memory[address], regs.sp);
}

void Processor::pushToStack16(u16 value) {
    // 2 cycles for writing value
    // 1 cycle for decreasing stack pointer
    counters.cyclesProcessed += 3;

    constexpr u16 stackBase = 0x0100;
    const u16 address = stackBase + regs.sp;
    memory[address] = hi(value);
    memory[address - 1] = lo(value);
    regs.sp -= 2;

    INSTRUCTION_TRACE("StackPush(memory[0x%04x]=0x%02x, memory[0x%04x]=0x%02x, sp=0x%02x)",
                      address, memory[address],
                      address - 1, memory[address - 1],
                      regs.sp);
}

u8 Processor::popFromStack() {
    // 1 cycle for reading value
    // 1 cycle for incrementing pointer
    counters.cyclesProcessed += 2;

    constexpr u16 stackBase = 0x0100;
    regs.sp++;
    const u16 address = stackBase + regs.sp;

    INSTRUCTION_TRACE("StackPop(0x%02x<-memory[0x%04x], sp=0x%02x)", memory[address], address, regs.sp);
    return memory[address];
}

u16 Processor::popFromStack16() {
    // 2 cycles for reading value
    // 1 cycle for incrementing pointer
    counters.cyclesProcessed += 3;

    constexpr u16 stackBase = 0x0100;
    regs.sp += 2;
    const u16 address = stackBase + regs.sp;

    INSTRUCTION_TRACE("StackPop(0x%02x<-memory[0x%04x], 0x%02x<-memory[0x%04x], sp=0x%02x)",
                      memory[address - 1], address - 1,
                      memory[address], address,
                      regs.sp);

    const u16 lo = memory[address - 1];
    const u16 hi = memory[address];
    return constructU16(hi, lo);
}

void Processor::executeLda(AddressingMode mode) {
    const u8 value = readValue(mode, true);
    regs.a = value;
    updateArithmeticFlags(value);

    INSTRUCTION_TRACE("a=0x%02x", regs.a);
}

void Processor::executeLdx(AddressingMode mode) {
    const u8 value = readValue(mode, true);
    regs.x = value;
    updateArithmeticFlags(value);

    INSTRUCTION_TRACE("x=0x%02x", regs.x);
}

void Processor::executeLdy(AddressingMode mode) {
    const u8 value = readValue(mode, true);
    regs.y = value;
    updateArithmeticFlags(value);

    INSTRUCTION_TRACE("y=0x%02x", regs.y);
}

void Processor::executeInc(AddressingMode mode) {
    const u16 address = getAddress(mode, false);
    u8 value = readByteFromMemory(address);
    value++;
    aluOperation();
    writeByteToMemory(address, value);
    updateArithmeticFlags(value);

    INSTRUCTION_TRACE("a=0x%02x", regs.a);
}

void Processor::executeDec(AddressingMode mode) {
    const u16 address = getAddress(mode, false);
    u8 value = readByteFromMemory(address);
    value--;
    aluOperation();
    writeByteToMemory(address, value);
    updateArithmeticFlags(value);

    INSTRUCTION_TRACE("a=0x%02x", regs.a);
}

void Processor::executeInx(AddressingMode) {
    regs.x++;
    aluOperation();
    updateArithmeticFlags(regs.x);

    INSTRUCTION_TRACE("x=0x%02x", regs.x);
}

void Processor::executeIny(AddressingMode) {
    regs.y++;
    aluOperation();
    updateArithmeticFlags(regs.y);

    INSTRUCTION_TRACE("y=0x%02x", regs.y);
}

void Processor::executeDex(AddressingMode) {
    regs.x--;
    aluOperation();
    updateArithmeticFlags(regs.x);

    INSTRUCTION_TRACE("x=0x%02x", regs.x);
}

void Processor::executeDey(AddressingMode) {
    regs.y--;
    aluOperation();
    updateArithmeticFlags(regs.y);

    INSTRUCTION_TRACE("y=0x%02x", regs.y);
}

void Processor::executeAsl(AddressingMode mode) {
    u16 address{};
    u8 value = readValue(mode, false, &address);
    aluOperation();
    regs.flags.c = isSignBitSet(value);
    value <<= 1;
    writeValue(mode, value, &address);
    updateArithmeticFlags(value);
}

void Processor::executeLsr(AddressingMode mode) {
    u16 address{};
    u8 value = readValue(mode, false, &address);
    aluOperation();
    regs.flags.c = isZeroBitSet(value);
    value >>= 1;
    writeValue(mode, value, &address);
    updateArithmeticFlags(value);
}

void Processor::executeRol(AddressingMode mode) {
    u16 address{};
    u8 value = readValue(mode, false, &address);

    const bool zeroBit = regs.flags.c;
    regs.flags.c = isSignBitSet(value);
    value <<= 1;
    setBit<0>(value, zeroBit);

    aluOperation();
    writeValue(mode, value, &address);
    updateArithmeticFlags(value);
}

void Processor::executeRor(AddressingMode mode) {
    u16 address{};
    u8 value = readValue(mode, false, &address);

    const bool oldestBit = regs.flags.c;
    regs.flags.c = isZeroBitSet(value);
    value >>= 1;
    setBit<7>(value, oldestBit);

    aluOperation();
    writeValue(mode, value, &address);
    updateArithmeticFlags(value);
}

void Processor::executeCmp(AddressingMode mode) {
    const u8 value = readValue(mode, true);
    updateFlagsAfterComparison(regs.a, value);
}

void Processor::executeCpx(AddressingMode mode) {
    const u8 value = readValue(mode, true);
    updateFlagsAfterComparison(regs.x, value);
}

void Processor::executeCpy(AddressingMode mode) {
    const u8 value = readValue(mode, true);
    updateFlagsAfterComparison(regs.y, value);
}

void Processor::executeTax(AddressingMode) {
    registerTransfer(regs.x, regs.a);
    updateArithmeticFlags(regs.x);
}
void Processor::executeTay(AddressingMode) {
    registerTransfer(regs.y, regs.a);
    updateArithmeticFlags(regs.y);
}
void Processor::executeTxa(AddressingMode) {
    registerTransfer(regs.a, regs.x);
    updateArithmeticFlags(regs.a);
}
void Processor::executeTya(AddressingMode) {
    registerTransfer(regs.a, regs.y);
    updateArithmeticFlags(regs.a);
}

void Processor::executeTsx(AddressingMode) {
    registerTransfer(regs.x, regs.sp);
    updateArithmeticFlags(regs.x);
}
void Processor::executeTxs(AddressingMode) {
    registerTransfer(regs.sp, regs.x);
}

void Processor::executePha(AddressingMode) {
    pushToStack(regs.a);
}

void Processor::executePhp(AddressingMode) {
    StatusFlags pushedFlags = regs.flags;
    pushedFlags.b = 1;
    pushedFlags.r = 1;
    pushToStack(pushedFlags.toU8());
}

void Processor::executePla(AddressingMode) {
    const u8 tmpReg = popFromStack();
    registerTransfer(regs.a, tmpReg);
    updateArithmeticFlags(tmpReg);
}

void Processor::executePlp(AddressingMode) {
    StatusFlags poppedFlags = StatusFlags::fromU8(popFromStack());
    poppedFlags.r = regs.flags.r; // reserved flag is ignored
    poppedFlags.b = regs.flags.b; // break flag is ignored
    registerTransfer(regs.flags, poppedFlags);
}

void Processor::executeAdc(AddressingMode mode) {
    const u8 srcRegA = regs.a;
    const u8 srcCarry = regs.flags.c;

    const u8 addend = readValue(mode, true);
    sumWithCarry(addend, true);

    INSTRUCTION_TRACE("0x%02x+0x%02x+0x%x=0x%02x", srcRegA, addend, srcCarry, regs.a);
}

void Processor::executeAnd(AddressingMode mode) {
    const u8 value = readValue(mode, true);
    regs.a &= value;
    updateArithmeticFlags(regs.a);

    INSTRUCTION_TRACE("a=0x%02x", regs.a);
}

void Processor::executeEor(AddressingMode mode) {
    const u8 value = readValue(mode, true);
    regs.a ^= value;
    updateArithmeticFlags(regs.a);

    INSTRUCTION_TRACE("a=0x%02x", regs.a);
}

void Processor::executeOra(AddressingMode mode) {
    const u8 value = readValue(mode, true);
    regs.a |= value;
    updateArithmeticFlags(regs.a);

    INSTRUCTION_TRACE("a=0x%02x", regs.a);
}

void Processor::executeBit(AddressingMode mode) {
    const u8 value = readValue(mode, true);
    regs.flags.z = (regs.a & value) == 0;
    regs.flags.o = isBitSet<6>(value);
    regs.flags.n = isBitSet<7>(value);
}

void Processor::executeSec(AddressingMode) {
    regs.flags.c = 1;
    counters.cyclesProcessed++;
}
void Processor::executeSed(AddressingMode) {
    regs.flags.d = 1;
    counters.cyclesProcessed++;
}
void Processor::executeSei(AddressingMode) {
    regs.flags.i = 1;
    counters.cyclesProcessed++;
}

void Processor::executeClc(AddressingMode) {
    regs.flags.c = 0;
    counters.cyclesProcessed++;
}

void Processor::executeCld(AddressingMode) {
    regs.flags.d = 0;
    counters.cyclesProcessed++;
}

void Processor::executeCli(AddressingMode) {
    regs.flags.i = 0;
    counters.cyclesProcessed++;
}

void Processor::executeClv(AddressingMode) {
    regs.flags.o = 0;
    counters.cyclesProcessed++;
}

void Processor::executeSta(AddressingMode mode) {
    const u16 address = getAddress(mode, false);
    writeByteToMemory(address, regs.a);
}

void Processor::executeStx(AddressingMode mode) {
    const u16 address = getAddress(mode, false);
    writeByteToMemory(address, regs.x);
}

void Processor::executeSty(AddressingMode mode) {
    const u16 address = getAddress(mode, false);
    writeByteToMemory(address, regs.y);
}

void Processor::executeSbc(AddressingMode mode) {
    const u8 srcRegA = regs.a;
    const u8 srcCarry = regs.flags.c;

    const u8 value = readValue(mode, true);
    sumWithCarry(~value, true);

    INSTRUCTION_TRACE("0x%02x-0x%02x-(0x1-0%x)=0x%02x+0x%02x+0%x=0x%02x",
                      srcRegA, value, srcCarry,
                      srcRegA, (~value & 0xFF), srcCarry,
                      regs.a);
}

void Processor::executeJmp(AddressingMode mode) {
    const u16 address = getAddress(mode, true);
    regs.pc = address;
}

void Processor::executeJsr(AddressingMode mode) {
    const u16 calledAddress = getAddress(mode, true);
    pushToStack16(regs.pc - 1);
    regs.pc = calledAddress;
}

void Processor::executeRts(AddressingMode) {
    idleCycle(); // Fetching a second instruction byte needlessly. See http://forum.6502.org/viewtopic.php?f=2&t=5146
    const u16 returnAddress = popFromStack16() + 1;
    aluOperation(); // Incrementing PC
    regs.pc = returnAddress;
}

void Processor::executeBranch(AddressingMode mode, bool take) {
    if (take) {
        const u16 branchAddress = getAddress(mode, true);
        regs.pc = branchAddress;
        idleCycle(); // when pc is calculated, it's already too late to schedule memory fetch, so there's an extra cycle

        INSTRUCTION_TRACE("Branched");
    } else {
        fetchInstructionByte();

        INSTRUCTION_TRACE("NotBranched");
    }
}

void Processor::executeBcc(AddressingMode mode) {
    executeBranch(mode, !regs.flags.c);
}

void Processor::executeBcs(AddressingMode mode) {
    executeBranch(mode, regs.flags.c);
}

void Processor::executeBeq(AddressingMode mode) {
    executeBranch(mode, regs.flags.z);
}

void Processor::executeBmi(AddressingMode mode) {
    executeBranch(mode, regs.flags.n);
}

void Processor::executeBne(AddressingMode mode) {
    executeBranch(mode, !regs.flags.z);
}

void Processor::executeBpl(AddressingMode mode) {
    executeBranch(mode, !regs.flags.n);
}

void Processor::executeBvc(AddressingMode mode) {
    executeBranch(mode, !regs.flags.o);
}

void Processor::executeBvs(AddressingMode mode) {
    executeBranch(mode, regs.flags.o);
}
void Processor::executeNop(AddressingMode) {
    idleCycle();
}

void Processor::executeBrk(AddressingMode) {
    readByteFromMemory(regs.pc);

    pushToStack16(regs.pc + 1);
    hiddenLatencyCycle(); // decreasing SP register can be hidden

    StatusFlags pushedFlags = regs.flags;
    pushedFlags.b = 1;
    pushedFlags.r = 1;
    pushToStack(pushedFlags.toU8());
    hiddenLatencyCycle(); // decreasing SP register can be hidden

    regs.pc = readTwoBytesFromMemory(0xFFFE);
    regs.flags.i = 1;
}

void Processor::executeRti(AddressingMode) {
    u8 flags = popFromStack();
    hiddenLatencyCycle(); // decreasing SP register can be hidden
    u8 pcLo = popFromStack();
    hiddenLatencyCycle(); // decreasing SP register can be hidden
    u8 pcHi = popFromStack();

    StatusFlags newFlags = StatusFlags::fromU8(flags);
    newFlags.b = regs.flags.b; // B flag is ignored
    newFlags.r = regs.flags.r; // R flag is ignored
    regs.flags = newFlags;
    regs.pc = constructU16(pcHi, pcLo);
    aluOperation();
}
