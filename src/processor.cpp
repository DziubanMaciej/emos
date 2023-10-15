#include "error.h"
#include "processor.h"

#include <cstring>

void Processor::executeInstructions(u32 maxInstructionCount) {
    for (u32 instructionIndex = 0; instructionIndex < maxInstructionCount; instructionIndex++) {
        const OpCode opCode = static_cast<OpCode>(fetchInstructionByte());
        executeInstruction(opCode);
    }
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

u8 Processor::getValueImmediate() {
    return fetchInstructionByte();
}
u8 Processor::getValueZeroPage() {
    u8 address = fetchInstructionByte();
    return readByteFromMemory(address);
}

u8 Processor::getValueZeroPageX() {
    u16 address = sumAddressesZeroPage(fetchInstructionByte(), regs.x);
    return readByteFromMemory(address);
}

u8 Processor::getValueAbsolute() {
    u16 address = fetchInstructionTwoBytes();
    return readByteFromMemory(address);
}

u8 Processor::getValueAbsoluteX() {
    u16 address = sumAddresses(fetchInstructionTwoBytes(), regs.x);
    return readByteFromMemory(address);
}

u8 Processor::getValueAbsoluteY() {
    u16 address = sumAddresses(fetchInstructionTwoBytes(), regs.y);
    return readByteFromMemory(address);
}

u8 Processor::getValueIndexedIndirectX() {
    u8 tableAddress = fetchInstructionByte();
    u16 address = sumAddressesZeroPage(tableAddress, regs.x);
    address = readTwoBytesFromMemory(address);
    return readByteFromMemory(address);
}

u8 Processor::getValueIndirectIndexedY() {
    u16 address = fetchInstructionByte();
    address = readTwoBytesFromMemory(address);
    address = sumAddresses(address, regs.y);
    return readByteFromMemory(address);
}


u16 Processor::sumAddresses(u16 base, u16 offset) {
    const u16 result = base + offset;

    const u16 oldPage = base & 0xFF00;
    const u16 newPage = result & 0xFF00;
    const u16 changedPage = oldPage != newPage;
    if (changedPage) {
        // For some instructions (namely with abs X addressing mode) the latency of addition is hidden
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
        // Sources:
        //   https://forums.nesdev.org/viewtopic.php?t=13936
        //   https://retrocomputing.stackexchange.com/q/15621
        counters.cyclesProcessed++;
    }
    return result;
}

u16 Processor::sumAddressesZeroPage(u8 base, u8 offset) {
    const u8 result = base + offset; // let it wrap around
    counters.cyclesProcessed++;
    return static_cast<u16>(result);
}

void Processor::updateArithmeticFlags(u8 value) {
    regs.flags.z = value == 0;
    regs.flags.n = bool(value & 0x80);
}

void Processor::executeInstruction(OpCode opCode) {
    switch (opCode) {
    case OpCode::LDA_imm: {
        regs.a = getValueImmediate();
        updateArithmeticFlags(regs.a);
    } break;
    case OpCode::LDA_z: {
        regs.a = getValueZeroPage();
        updateArithmeticFlags(regs.a);
    } break;
    case OpCode::LDA_zx: {
        regs.a = getValueZeroPageX();
        updateArithmeticFlags(regs.a);
    } break;
    case OpCode::LDA_abs: {
        regs.a = getValueAbsolute();
        updateArithmeticFlags(regs.a);
    } break;
    case OpCode::LDA_absx: {
        regs.a = getValueAbsoluteX();
        updateArithmeticFlags(regs.a);
    } break;
    case OpCode::LDA_absy: {
        regs.a = getValueAbsoluteY();
        updateArithmeticFlags(regs.a);
    } break;
    case OpCode::LDA_ix: {
        regs.a = getValueIndexedIndirectX();
        updateArithmeticFlags(regs.a);
    } break;
    case OpCode::LDA_iy: {
        regs.a = getValueIndirectIndexedY();
        updateArithmeticFlags(regs.a);
    } break;
    default:
        FATAL_ERROR("Unsupported instruction: ", static_cast<u32>(opCode));
    }
}
