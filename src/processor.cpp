#include "error.h"
#include "processor.h"

#include <cstring>
#include <limits>

Processor::Processor() {
    setInstructionData(OpCode::LDA_imm, AddressingMode::Immediate, &Processor::executeLda);
    setInstructionData(OpCode::LDA_z, AddressingMode::ZeroPage, &Processor::executeLda);
    setInstructionData(OpCode::LDA_zx, AddressingMode::ZeroPageX, &Processor::executeLda);
    setInstructionData(OpCode::LDA_abs, AddressingMode::Absolute, &Processor::executeLda);
    setInstructionData(OpCode::LDA_absx, AddressingMode::AbsoluteX, &Processor::executeLda);
    setInstructionData(OpCode::LDA_absy, AddressingMode::AbsoluteY, &Processor::executeLda);
    setInstructionData(OpCode::LDA_ix, AddressingMode::IndexedIndirectX, &Processor::executeLda);
    setInstructionData(OpCode::LDA_iy, AddressingMode::IndirectIndexedY, &Processor::executeLda);

    setInstructionData(OpCode::LDX_imm, AddressingMode::Immediate, &Processor::executeLdx);
    setInstructionData(OpCode::LDX_z, AddressingMode::ZeroPage, &Processor::executeLdx);
    setInstructionData(OpCode::LDX_zy, AddressingMode::ZeroPageY, &Processor::executeLdx);
    setInstructionData(OpCode::LDX_abs, AddressingMode::Absolute, &Processor::executeLdx);
    setInstructionData(OpCode::LDX_absy, AddressingMode::AbsoluteY, &Processor::executeLdx);

    setInstructionData(OpCode::LDY_imm, AddressingMode::Immediate, &Processor::executeLdy);
    setInstructionData(OpCode::LDY_z, AddressingMode::ZeroPage, &Processor::executeLdy);
    setInstructionData(OpCode::LDY_zx, AddressingMode::ZeroPageX, &Processor::executeLdy);
    setInstructionData(OpCode::LDY_abs, AddressingMode::Absolute, &Processor::executeLdy);
    setInstructionData(OpCode::LDY_absx, AddressingMode::AbsoluteX, &Processor::executeLdy);

    setInstructionData(OpCode::INC_z, AddressingMode::ZeroPage, &Processor::executeInc);
    setInstructionData(OpCode::INC_zx, AddressingMode::ZeroPageX, &Processor::executeInc);
    setInstructionData(OpCode::INC_abs, AddressingMode::Absolute, &Processor::executeInc);
    setInstructionData(OpCode::INC_absx, AddressingMode::AbsoluteX, &Processor::executeInc);

    setInstructionData(OpCode::INX, AddressingMode::Implied, &Processor::executeInx);
    setInstructionData(OpCode::INY, AddressingMode::Implied, &Processor::executeIny);
    setInstructionData(OpCode::DEX, AddressingMode::Implied, &Processor::executeDex);
    setInstructionData(OpCode::DEY, AddressingMode::Implied, &Processor::executeDey);

    setInstructionData(OpCode::DEC_z, AddressingMode::ZeroPage, &Processor::executeDec);
    setInstructionData(OpCode::DEC_zx, AddressingMode::ZeroPageX, &Processor::executeDec);
    setInstructionData(OpCode::DEC_abs, AddressingMode::Absolute, &Processor::executeDec);
    setInstructionData(OpCode::DEC_absx, AddressingMode::AbsoluteX, &Processor::executeDec);

    setInstructionData(OpCode::TAX, AddressingMode::Implied, &Processor::executeTax);
    setInstructionData(OpCode::TAY, AddressingMode::Implied, &Processor::executeTay);
    setInstructionData(OpCode::TXA, AddressingMode::Implied, &Processor::executeTxa);
    setInstructionData(OpCode::TYA, AddressingMode::Implied, &Processor::executeTya);

    setInstructionData(OpCode::CMP_imm, AddressingMode::Immediate, &Processor::executeCmp);
    setInstructionData(OpCode::CMP_z, AddressingMode::ZeroPage, &Processor::executeCmp);
    setInstructionData(OpCode::CMP_zx, AddressingMode::ZeroPageX, &Processor::executeCmp);
    setInstructionData(OpCode::CMP_abs, AddressingMode::Absolute, &Processor::executeCmp);
    setInstructionData(OpCode::CMP_absx, AddressingMode::AbsoluteX, &Processor::executeCmp);
    setInstructionData(OpCode::CMP_absy, AddressingMode::AbsoluteY, &Processor::executeCmp);
    setInstructionData(OpCode::CMP_ix, AddressingMode::IndexedIndirectX, &Processor::executeCmp);
    setInstructionData(OpCode::CMP_iy, AddressingMode::IndirectIndexedY, &Processor::executeCmp);

    setInstructionData(OpCode::CPX_imm, AddressingMode::Immediate, &Processor::executeCpx);
    setInstructionData(OpCode::CPX_z, AddressingMode::ZeroPage, &Processor::executeCpx);
    setInstructionData(OpCode::CPX_abs, AddressingMode::Absolute, &Processor::executeCpx);

    setInstructionData(OpCode::CPY_imm, AddressingMode::Immediate, &Processor::executeCpy);
    setInstructionData(OpCode::CPY_z, AddressingMode::ZeroPage, &Processor::executeCpy);
    setInstructionData(OpCode::CPY_abs, AddressingMode::Absolute, &Processor::executeCpy);

    setInstructionData(OpCode::ADC_imm, AddressingMode::Immediate, &Processor::executeAdc);
    setInstructionData(OpCode::ADC_z, AddressingMode::ZeroPage, &Processor::executeAdc);
    setInstructionData(OpCode::ADC_zx, AddressingMode::ZeroPageX, &Processor::executeAdc);
    setInstructionData(OpCode::ADC_abs, AddressingMode::Absolute, &Processor::executeAdc);
    setInstructionData(OpCode::ADC_absx, AddressingMode::AbsoluteX, &Processor::executeAdc);
    setInstructionData(OpCode::ADC_absy, AddressingMode::AbsoluteY, &Processor::executeAdc);
    setInstructionData(OpCode::ADC_ix, AddressingMode::IndexedIndirectX, &Processor::executeAdc);
    setInstructionData(OpCode::ADC_iy, AddressingMode::IndirectIndexedY, &Processor::executeAdc);
}

void Processor::executeInstructions(u32 maxInstructionCount) {
    for (u32 instructionIndex = 0; instructionIndex < maxInstructionCount; instructionIndex++) {
        const u8 opCode = fetchInstructionByte();
        const InstructionData &instruction = instructionData[opCode];
        if (instruction.exec == nullptr) {
            FATAL_ERROR("Unsupported instruction: ", static_cast<u32>(opCode));
        }

        (this->*instruction.exec)(instruction.addressingMode);
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
    default: {
        FATAL_ERROR("Unknown addressing mode");
    }
    }
}

u8 Processor::readValue(AddressingMode mode, bool isReadOnly) {
    if (mode == AddressingMode::Immediate) {
        return fetchInstructionByte();
    } else {
        const u16 address = getAddress(mode, isReadOnly);
        return readByteFromMemory(address);
    }
}

void Processor::writeValue(AddressingMode mode, u8 value) {
    const u16 address = getAddress(mode, false);
    writeByteToMemory(address, value);
}

u16 Processor::sumAddresses(u16 base, u16 offset, bool isReadOnly) {
    const u16 result = base + offset;

    const u16 oldPage = base & 0xFF00;
    const u16 newPage = result & 0xFF00;
    const u16 changedPage = oldPage != newPage;
    if (changedPage || !isReadOnly) {
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
        counters.cyclesProcessed++;
    }
    return result;
}

u16 Processor::sumAddressesZeroPage(u8 base, u8 offset) {
    const u8 result = base + offset; // let it wrap around
    counters.cyclesProcessed++;
    return static_cast<u16>(result);
}

void Processor::registerTransfer(u8 &dst, const u8 &src) {
    FATAL_ERROR_IF(&dst == &src, "Cannot do register transfer on one register");
    counters.cyclesProcessed++;
    dst = src;
}

void Processor::aluOperation() {
    counters.cyclesProcessed++;
}

bool Processor::isSignBitSet(u8 value) {
    return value & 0x80;
}

void Processor::updateArithmeticFlags(u8 value) {
    regs.flags.z = value == 0;
    regs.flags.n = isSignBitSet(value);
}

void Processor::updateFlagsAfterComparison(u8 registerValue, u8 inputValue) {
    regs.flags.c = registerValue >= inputValue;
    regs.flags.z = registerValue == inputValue;
    regs.flags.n = registerValue < inputValue;
}

void Processor::executeLda(AddressingMode mode) {
    const u8 value = readValue(mode, true);
    regs.a = value;
    updateArithmeticFlags(value);
}

void Processor::executeLdx(AddressingMode mode) {
    const u8 value = readValue(mode, true);
    regs.x = value;
    updateArithmeticFlags(value);
}

void Processor::executeLdy(AddressingMode mode) {
    const u8 value = readValue(mode, true);
    regs.y = value;
    updateArithmeticFlags(value);
}

void Processor::executeInc(AddressingMode mode) {
    const u16 address = getAddress(mode, false);
    u8 value = readByteFromMemory(address);
    value++;
    aluOperation();
    writeByteToMemory(address, value);
    updateArithmeticFlags(value);
}

void Processor::executeDec(AddressingMode mode) {
    const u16 address = getAddress(mode, false);
    u8 value = readByteFromMemory(address);
    value--;
    aluOperation();
    writeByteToMemory(address, value);
    updateArithmeticFlags(value);
}

void Processor::executeInx(AddressingMode) {
    regs.x++;
    aluOperation();
    updateArithmeticFlags(regs.x);
}

void Processor::executeIny(AddressingMode) {
    regs.y++;
    aluOperation();
    updateArithmeticFlags(regs.y);
}

void Processor::executeDex(AddressingMode) {
    regs.x--;
    aluOperation();
    updateArithmeticFlags(regs.x);
}

void Processor::executeDey(AddressingMode) {
    regs.y--;
    aluOperation();
    updateArithmeticFlags(regs.y);
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

void Processor::updateCarryFlagIfOverflow(u16 value) {
    regs.flags.c = value > std::numeric_limits<u8>::max();
}

u16 Processor::updateOverflowForSumWithCarry(u8 inputValue1, u8 inputValue2) {
    u16 sum = inputValue1 + inputValue2;
    if (!(isSignBitSet(inputValue1) ^ isSignBitSet(inputValue2))) { // when 0 && 0 or 1 && 1
        regs.flags.o = isSignBitSet(sum) ^ isSignBitSet(inputValue1);
    }
    return sum;
}

void Processor::executeAdc(AddressingMode mode) {
    const u8 addend = readValue(mode, true);
    u16 sum = updateOverflowForSumWithCarry(regs.a, regs.flags.c);
    sum = updateOverflowForSumWithCarry(sum, addend);
    updateCarryFlagIfOverflow(sum);
    regs.a = static_cast<u8>(sum);
    updateArithmeticFlags(regs.a);
}