#include "error.h"
#include "processor.h"

#include <cstring>

Processor::Processor() {
    setInstructionData(OpCode::LDA_imm, &Processor::getValueImmediate, &Processor::executeLda);
    setInstructionData(OpCode::LDA_z, &Processor::getValueZeroPage, &Processor::executeLda);
    setInstructionData(OpCode::LDA_zx, &Processor::getValueZeroPageX, &Processor::executeLda);
    setInstructionData(OpCode::LDA_abs, &Processor::getValueAbsolute, &Processor::executeLda);
    setInstructionData(OpCode::LDA_absx, &Processor::getValueAbsoluteX, &Processor::executeLda);
    setInstructionData(OpCode::LDA_absy, &Processor::getValueAbsoluteY, &Processor::executeLda);
    setInstructionData(OpCode::LDA_ix, &Processor::getValueIndexedIndirectX, &Processor::executeLda);
    setInstructionData(OpCode::LDA_iy, &Processor::getValueIndirectIndexedY, &Processor::executeLda);

    setInstructionData(OpCode::CMP_imm, &Processor::getValueImmediate, &Processor::executeCmp);
    setInstructionData(OpCode::CMP_z, &Processor::getValueZeroPage, &Processor::executeCmp);
    setInstructionData(OpCode::CMP_zx, &Processor::getValueZeroPageX, &Processor::executeCmp);
    setInstructionData(OpCode::CMP_abs, &Processor::getValueAbsolute, &Processor::executeCmp);
    setInstructionData(OpCode::CMP_absx, &Processor::getValueAbsoluteX, &Processor::executeCmp);
    setInstructionData(OpCode::CMP_absy, &Processor::getValueAbsoluteY, &Processor::executeCmp);
    setInstructionData(OpCode::CMP_ix, &Processor::getValueIndexedIndirectX, &Processor::executeCmp);
    setInstructionData(OpCode::CMP_iy, &Processor::getValueIndirectIndexedY, &Processor::executeCmp);
}

void Processor::executeInstructions(u32 maxInstructionCount) {
    for (u32 instructionIndex = 0; instructionIndex < maxInstructionCount; instructionIndex++) {
        const u8 opCode = fetchInstructionByte();
        const InstructionData &instruction = instructionData[opCode];
        if (instruction.get == nullptr || instruction.exec == nullptr) {
            FATAL_ERROR("Unsupported instruction: ", static_cast<u32>(opCode));
        }

        const u8 value = (this->*instruction.get)();
        (this->*instruction.exec)(value);
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

void Processor::updateFlagsAfterComparison(u8 registerValue, u8 inputValue) {
    regs.flags.c = registerValue >= inputValue;
    regs.flags.z = registerValue == inputValue;
    regs.flags.n = registerValue < inputValue;
}

void Processor::executeLda(u8 value) {
    regs.a = value;
    updateArithmeticFlags(value);
}

void Processor::executeCmp(u8 value) {
    updateFlagsAfterComparison(regs.a, value);
}
