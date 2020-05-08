//
//  CPU.cpp
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 30 Mar 2020.
//  Copyright (c) 2020 Rakesh Ayyaswami. All rights reserved.
//

#include "CPU.hpp"

namespace rt_6502_emulator {

    // constructor & destructor ----------------------------------------------------------------------------------------

    CPU::CPU() {
        _initOperations();
        reset();
    }

    CPU::~CPU() {
        // nothing to do atm
    }


    // accessors -------------------------------------------------------------------------------------------------------

    byte CPU::getAccumulator()    { return _acc; }
    byte CPU::getIndexX()         { return _idx; }
    byte CPU::getIndexY()         { return _idy; }
    byte CPU::getStackPointer()   { return _stackP; }
    byte CPU::getStatus()         { return _status; }
    word CPU::getProgramCounter() { return _pc; }

    bool CPU::isOperationComplete() {
        return _opCycles == 0;
    }


    // public methods --------------------------------------------------------------------------------------------------

    void CPU::reset() {

        // reset registers
        _acc           = 0x00;
        _idx           = 0x00;
        _idy           = 0x00;
        _stackP        = 0x00;
        _status        = STATUS_FLAG_UNUSED | STATUS_FLAG_DISABLE_INTERRUPTS;
        _interruptType = INTERRUPT_TYPE_NONE;

        // read program start address from 0xFFFC to initialize the program counter
        _pc            = word(_read(0xFFFC)) | (word(_read(0xFFFD)) << 8);

        // reset current op addressing
        _opTargetAcc   = false;
        _opAddress     = 0x0000;

        // reset takes 8 clock cycles
        _opCycles      = 8;
    }

    void CPU::irq() {
        _interruptType = INTERRUPT_TYPE_MASKABLE;
    }

    void CPU::nmi() {
        _interruptType = INTERRUPT_TYPE_NON_MASKABLE;
    }

    void CPU::tick() {

        // execute next instruction or interrupt request if the clock ticks
        // required for the previous instruction have elapsed.
        if (_opCycles == 0) {

            // execute interrupt request or instruction
            if (_isInterruptRequested()) {
                _interrupt();
            }
            else {
                _execute();
            }

            // reset interrupt request
            _interruptType = INTERRUPT_TYPE_NONE;

            // ensure unused flag is always set in the status register
            _setStatusFlag(STATUS_FLAG_UNUSED, true);
        }

        // decrement cycles for operation
        if (_opCycles > 0) {
            _opCycles--;
        }
    }

    byte CPU::step() {
        byte count = 0;
        do {
            tick();
            count++;
        } while(_opCycles > 0);
        return count;
    }


    // execution helpers -----------------------------------------------------------------------------------------------

    bool CPU::_isInterruptRequested() {
        switch (_interruptType) {
        case INTERRUPT_TYPE_MASKABLE:
            return _getStatusFlag(STATUS_FLAG_DISABLE_INTERRUPTS) == false;

        case INTERRUPT_TYPE_NON_MASKABLE:
            return true;

        default:
            return false;
        }
    }

    void CPU::_interrupt() {

        // decode interrupt request
        word vector;
        switch (_interruptType) {
        case INTERRUPT_TYPE_MASKABLE:     vector = 0xFFFE; _opCycles = 7; break;
        case INTERRUPT_TYPE_NON_MASKABLE: vector = 0xFFFA; _opCycles = 8; break;
        case INTERRUPT_TYPE_NONE:
        default:
            return;
        }


        // push program counter & status on the stack
        _pushWord(_pc);
        _pushWord(_status);

        // disable interrupts
        _setStatusFlag(STATUS_FLAG_DISABLE_INTERRUPTS, true);

        // load handler address
        _pc = word(_read(vector)) | (word(_read(vector + 1)) << 8);
    }

    void CPU::_execute() {

        // read next operation
        byte opcode = _readNextByte();
        CPU::Operation op = _operations[opcode];

        // set the number of cycles required for the op
        _opCycles = op.cycles;

        // reset addressing state
        _opTargetAcc  = false;
        _opAddress    = 0x0000;

        // execute the operation
        // require an extra cycle if both the addressing & instruction ask for it
        bool extraCycleAddr = (this->*op.addr)();
        bool extraCycleInst = (this->*op.inst)();

        if (extraCycleAddr && extraCycleInst) {
            _opCycles++;
        }
    }


    // status register helpers -----------------------------------------------------------------------------------------

    bool CPU::_getStatusFlag(STATUS_FLAG bit) {
        return (_status & bit) > 0 ? 1 : 0;
    }

    void CPU::_setStatusFlag(STATUS_FLAG bit, bool value) {
        if (value) {
            _status |= bit;
        }
        else {
            _status &= ~bit;
        }
    }

    void CPU::_setResultStatusFlags(byte data) {
        _setStatusFlag(STATUS_FLAG_ZERO,     data == 0x00);
        _setStatusFlag(STATUS_FLAG_NEGATIVE, data & 0x80);
    }


    // bus access convenience methods ----------------------------------------------------------------------------------

    byte CPU::_read(word address) {
        byte data;
        bool success = read(address, data);
        return success ? data : 0x00;
    }

    void CPU::_write(word address, byte data) {
        write(address, data);
    }

    byte CPU::_readNextByte() {
        return _read(_pc++);
    }

    word CPU::_readNextWord() {
        word lsb = _read(_pc++);
        word msb = _read(_pc++);
        return (msb << 8) | lsb;
    }

    void CPU::_pushByte(byte data) {
        _write(0x0100 & _stackP, data);
        _stackP--;
    }

    void CPU::_pushWord(word data) {
        _pushByte((data >> 8) & 0xFF);
        _pushByte(data & 0xFF);
    }

    byte CPU::_popByte() {
        byte data = _read(0x0100 & _stackP);
        _stackP++;
        return data;
    }

    word CPU::_popWord() {
        return _popByte() | word(_popByte()) << 8;
    }


    // addressing modes ------------------------------------------------------------------------------------------------

    byte CPU::_fetch() {
        return _opTargetAcc
            ? _acc
            : _read(_opAddress);
    }

    void CPU::_store(byte data) {
        if (_opTargetAcc) {
            _acc = data;
        }
        else {
            _write(_opAddress, data);
        }
    }

    bool CPU::_addr_IMP() {
        return false;
    }

    bool CPU::_addr_ACC() {
        _opTargetAcc  = true;
        return false;
    }

    bool CPU::_addr_IMM() {
        _opAddress    = ++_pc;
        return false;
    }

    bool CPU::_addr_ZPG() {
        _opAddress    = _readNextByte();
        return false;
    }

    bool CPU::_addr_ZPX() {
        _opAddress    = 0x00FF & (_readNextByte() + _idx);
        return false;
    }

    bool CPU::_addr_ZPY() {
        _opAddress    = 0x00FF & (_readNextByte() + _idy);
        return false;
    }

    bool CPU::_addr_REL() {

        // get relative offset. can be negative with 2's complement format
        word rel      = _readNextByte();
        if (rel & 0x80) {
            rel |= 0xFF00; // negative relative address
        }

        // final address is offset from program counter
        // need extra cycle if page boundary crossed
        _opAddress    = _pc + rel;
        return (0xFF00 & _pc) != (0xFF00 & _opAddress);
    }

    bool CPU::_addr_ABS() {
        _opAddress    = _readNextWord();
        return false;
    }

    bool CPU::_addr_ABX() {
        word address  = _readNextWord();
        _opAddress    = address + _idx;
        return (0xFF00 & address) != (0xFF00 & _opAddress);
    }

    bool CPU::_addr_ABY() {
        word address  = _readNextWord();
        _opAddress    = address + _idy;
        return (0xFF00 & address) != (0xFF00 & _opAddress);
    }

    bool CPU::_addr_IND() {
        word address  = _readNextWord();
        word lsb      = _read(address);
        // HARDWARE BUG: If address is last byte of page, adding 1 wraps around to first address of page instead of
        // crossing page boundary.
        // SEE: JMP bug at http://nesdev.com/6502bugs.txt
        word msb      = (address & 0x00FF) == 0x00FF ? _read(address & 0xFF00) : _read(address + 1);
        _opAddress    = (msb << 8) | lsb;
        return false;
    }

    bool CPU::_addr_IZX() {
        word address  = _readNextByte() + _idx;
        word lsb      = _read(address & 0x00FF);
        word msb      = _read((address + 1) & 0x00FF);
        _opAddress    = (msb << 8) | lsb;
        return false;
    }

    bool CPU::_addr_IZY() {
        word address  = _readNextByte();
        word lsb      = _read(address & 0x00FF);
        word msb      = _read((address + 1) & 0x00FF);
        _opAddress    = _idy + ((msb << 8) | lsb);
        return (msb << 8) != (_opAddress & 0xFF00);
    }


    // instructions ----------------------------------------------------------------------------------------------------

    /* Instructions for Illegal Op Codes */

    bool CPU::_inst_KIL() {
        return false;
    }

    bool CPU::_inst_SLO() {
        return false;
    }

    bool CPU::_inst_RLA() {
        return false;
    }

    bool CPU::_inst_SRE() {
        return false;
    }

    bool CPU::_inst_RRA() {
        return false;
    }

    bool CPU::_inst_SAX() {
        return false;
    }

    bool CPU::_inst_LAX() {
        return false;
    }

    bool CPU::_inst_DCP() {
        return false;
    }

    bool CPU::_inst_ISC() {
        return false;
    }

    bool CPU::_inst_ANC() {
        return false;
    }

    bool CPU::_inst_ALR() {
        return false;
    }

    bool CPU::_inst_ARR() {
        return false;
    }

    bool CPU::_inst_XAA() {
        return false;
    }

    bool CPU::_inst_AXS() {
        return false;
    }

    bool CPU::_inst_AHX() {
        return false;
    }

    bool CPU::_inst_SHY() {
        return false;
    }

    bool CPU::_inst_SHX() {
        return false;
    }

    bool CPU::_inst_TAS() {
        return false;
    }

    bool CPU::_inst_LAS() {
        return false;
    }


    /* Instructions for Legal Op codes  */

    bool CPU::_inst_ADC() {

        /* Add with carry uses 2's complement arithmetic. This allows it to be agnostic of whether the operands are
         * signed or unsigned numbers. https://en.wikipedia.org/wiki/Two%27s_complement
         * Overflow occurs when both operands are the same sign but the result is of a different sign.
         */

        // add fetched data with accumulator & carry bit
        byte data = _fetch();
        word res  = word(_acc) + word(data) + (_getStatusFlag(STATUS_FLAG_CARRY) ? 0x01 : 0x00);

        // set status flags for result
        _setStatusFlag(STATUS_FLAG_CARRY, res > 0xFF);
        _setStatusFlag(STATUS_FLAG_OVERFLOW, (~(_acc ^ data) & (_acc ^ res)) & 0x80);
        _setResultStatusFlags(res);

        // save result in accumulator
        _acc      = res;

        // operation can use extra cycle
        return true;
    }

    bool CPU::_inst_AND() {

        // AND data with accumulator & store result in accumulator
        byte data  = _fetch();
        _acc      &= data;

        // affect zero & negative flags
        _setResultStatusFlags(_acc);

        // operation can use extra cycle
        return true;
    }

    bool CPU::_inst_ASL() {
        word data = word(_fetch()) << 1;
        _setResultStatusFlags(data);
        _store(data);
        return false;
    }

    bool CPU::_inst_BCC() {

        // no branching if carry set
        if (_getStatusFlag(STATUS_FLAG_CARRY)) {
            return false;
        }

        // branch instructions load the destination address on the program counter if their test is successfull.
        // they also require an extra clock cycle to execute the branch. this is in addition to the extra cycle
        // required if branch causes a page change (See REL addressing mode)
        _pc = _opAddress;
        _opCycles++;
        return true;
    }

    bool CPU::_inst_BCS() {

        // no branching if carry cleared
        if (_getStatusFlag(STATUS_FLAG_CARRY) == false) {
            return false;
        }

        // similar to other branch instructions. see BCC
        _pc = _opAddress;
        _opCycles++;
        return true;
    }

    bool CPU::_inst_BEQ() {

        // no branching if not equal (i.e not zero)
        if (_getStatusFlag(STATUS_FLAG_ZERO) == false) {
            return false;
        }

        // similar to other branch instructions. see BCC
        _pc = _opAddress;
        _opCycles++;
        return true;
    }

    bool CPU::_inst_BIT() {

        /* This instruction sets the negative & overflow flags to 7th & 6th bits respectively of the operand and
         * the zero flag based on the AND of accumulator and operand.
         */

        byte data = _fetch();
        _setStatusFlag(STATUS_FLAG_NEGATIVE, data & 0b10000000);
        _setStatusFlag(STATUS_FLAG_OVERFLOW, data & 0b01000000);
        _setStatusFlag(STATUS_FLAG_ZERO,    (data & _acc) == 0x00);

        return false;
    }

    bool CPU::_inst_BMI() {

        // no branching if positive
        if (_getStatusFlag(STATUS_FLAG_NEGATIVE) == false) {
            return false;
        }

        // similar to other branch instructions. see BCC
        _pc = _opAddress;
        _opCycles++;
        return true;
    }

    bool CPU::_inst_BNE() {

        // no branching if equal (i.e zero)
        if (_getStatusFlag(STATUS_FLAG_ZERO)) {
            return false;
        }

        // similar to other branch instructions. see BCC
        _pc = _opAddress;
        _opCycles++;
        return true;
    }

    bool CPU::_inst_BPL() {

        // no branching if negative
        if (_getStatusFlag(STATUS_FLAG_NEGATIVE)) {
            return false;
        }

        // similar to other branch instructions. see BCC
        _pc = _opAddress;
        _opCycles++;
        return true;
    }

    bool CPU::_inst_BRK() {

        // skip padding byte
        _pc++;

        // push return address & status (with BREAK status set) on the stack
        _pushWord(_pc);
        _pushByte(_status & STATUS_FLAG_BREAK);

        // disable interrupts until return (RTI)
        _setStatusFlag(STATUS_FLAG_DISABLE_INTERRUPTS, true);

        // jump to interrupt handler address
        _pc = word(_read(0xFFFE)) | word(_read(0xFFFF)) << 8;

        return false;
    }

    bool CPU::_inst_BVC() {

        // no branching if overflow set
        if (_getStatusFlag(STATUS_FLAG_OVERFLOW)) {
            return false;
        }

        // similar to other branch instructions. see BCC
        _pc = _opAddress;
        _opCycles++;
        return true;
    }

    bool CPU::_inst_BVS() {

        // no branching if overflow clear
        if (_getStatusFlag(STATUS_FLAG_OVERFLOW) == false) {
            return false;
        }

        // similar to other branch instructions. see BCC
        _pc = _opAddress;
        _opCycles++;
        return true;    }

    bool CPU::_inst_CLC() {
        _setStatusFlag(STATUS_FLAG_CARRY, false);
        return false;
    }

    bool CPU::_inst_CLD() {
        _setStatusFlag(STATUS_FLAG_DECIMAL, false);
        return false;
    }

    bool CPU::_inst_CLI() {
        _setStatusFlag(STATUS_FLAG_DISABLE_INTERRUPTS, false);
        return false;
    }

    bool CPU::_inst_CLV() {
        _setStatusFlag(STATUS_FLAG_OVERFLOW, false);
        return false;
    }

    bool CPU::_inst_CMP() {
        byte data = _fetch();
        word res  = word(_acc) - word(data);
        _setStatusFlag(STATUS_FLAG_CARRY, _acc > data);
        _setResultStatusFlags(res);
        return true;
    }

    bool CPU::_inst_CPX() {
        byte data = _fetch();
        word res  = word(_idx) - word(data);
        _setStatusFlag(STATUS_FLAG_CARRY, _idx > data);
        _setResultStatusFlags(res);
        return false;
    }

    bool CPU::_inst_CPY() {
        byte data = _fetch();
        word res  = word(_idy) - word(data);
        _setStatusFlag(STATUS_FLAG_CARRY, _idy > data);
        _setResultStatusFlags(res);
        return false;
    }

    bool CPU::_inst_DEC() {
        byte data = _fetch() - 1;
        _setResultStatusFlags(data);
        _store(data);
        return false;
    }

    bool CPU::_inst_DEX() {
        _idx--;
        _setResultStatusFlags(_idx);
        return false;
    }

    bool CPU::_inst_DEY() {
        _idy--;
        _setResultStatusFlags(_idy);
        return false;
    }

    bool CPU::_inst_EOR() {
        _acc ^= _fetch();
        _setResultStatusFlags(_acc);
        return true;
    }

    bool CPU::_inst_INC() {
        byte data = _fetch() + 1;
        _setResultStatusFlags(data);
        _store(data);
        return false;
    }

    bool CPU::_inst_INX() {
        _idx++;
        _setResultStatusFlags(_idx);
        return false;
    }

    bool CPU::_inst_INY() {
        _idy++;
        _setResultStatusFlags(_idy);
        return false;
    }

    bool CPU::_inst_JMP() {
        _pc = _opAddress;
        return false;
    }

    bool CPU::_inst_JSR() {
        _pushWord(_pc - 1);
        _pc = _opAddress;
        return false;
    }

    bool CPU::_inst_LDA() {
        _acc = _fetch();
        _setResultStatusFlags(_acc);
        return true;
    }

    bool CPU::_inst_LDX() {
        _idx = _fetch();
        _setResultStatusFlags(_idx);
        return true;
    }

    bool CPU::_inst_LDY() {
        _idy = _fetch();
        _setResultStatusFlags(_idy);
        return true;
    }

    bool CPU::_inst_LSR() {
        byte data = _fetch();
        _setStatusFlag(STATUS_FLAG_CARRY, data & 0x01);
        data = data >> 1;
        _setResultStatusFlags(data);
        _store(data);
        return false;
    }

    bool CPU::_inst_NOP() {
        return false;
    }

    bool CPU::_inst_ORA() {
        _acc |= _fetch();
        _setResultStatusFlags(_acc);
        return true;
    }

    bool CPU::_inst_PHA() {
        _pushByte(_acc);
        return false;
    }

    bool CPU::_inst_PHP() {
        _pushByte(_status | STATUS_FLAG_BREAK);
        return false;
    }

    bool CPU::_inst_PLA() {
        _acc = _popByte();
        _setResultStatusFlags(_acc);
        return false;
    }

    bool CPU::_inst_PLP() {
        _status = _popByte();
        return false;
    }

    bool CPU::_inst_ROL() {
        byte data = _fetch();
        byte res  = (data << 1) | _getStatusFlag(STATUS_FLAG_CARRY);
        _setStatusFlag(STATUS_FLAG_CARRY, data & 0x80);
        _setResultStatusFlags(res);
        _store(res);
        return false;
    }

    bool CPU::_inst_ROR() {
        byte data = _fetch();
        byte res  = (data >> 1) | (_getStatusFlag(STATUS_FLAG_CARRY) << 7);
        _setStatusFlag(STATUS_FLAG_CARRY, data & 0x01);
        _setResultStatusFlags(res);
        _store(res);
        return false;
    }

    bool CPU::_inst_RTI() {
        _status = _popByte() & ~STATUS_FLAG_BREAK;
        _pc     = _popWord();
        return false;
    }

    bool CPU::_inst_RTS() {
        _pc = _popWord();
        return false;
    }

    bool CPU::_inst_SBC() {
        /* Substract with carry uses 2's complement arithmetic and works similar to add with carry.
         * R = A - M - (1 - C)   == becomes ==>   R = A - (M + 1) + C
         * And by 2's complement arithmetic -M = M ^ 0xFF + 1   == i.e. ==>   -(M + 1) = M ^ 0xFF
         * With this, we can execute the whole thing similar to addition
         * R = A + (M ^ 0xFF) + c
         */
        byte data = _fetch();
        word res  = _acc + (data ^ 0xFF) + (_getStatusFlag(STATUS_FLAG_CARRY) ? 0x01 : 0x00);
        _setStatusFlag(STATUS_FLAG_CARRY, res > 0xFF);
        _setStatusFlag(STATUS_FLAG_OVERFLOW, (~(_acc ^ data) & (_acc ^ res)) & 0x80);
        _setResultStatusFlags(res);
        return true;
    }

    bool CPU::_inst_SEC() {
        _setStatusFlag(STATUS_FLAG_CARRY, true);
        return false;
    }

    bool CPU::_inst_SED() {
        _setStatusFlag(STATUS_FLAG_DECIMAL, true);
        return false;
    }

    bool CPU::_inst_SEI() {
        _setStatusFlag(STATUS_FLAG_DISABLE_INTERRUPTS, true);
        return false;
    }

    bool CPU::_inst_STA() {
        _store(_acc);
        return false;
    }

    bool CPU::_inst_STX() {
        _store(_idx);
        return false;
    }

    bool CPU::_inst_STY() {
        _store(_idy);
        return false;
    }

    bool CPU::_inst_TAX() {
        _idx = _acc;
        _setResultStatusFlags(_idx);
        return false;
    }

    bool CPU::_inst_TAY() {
        _idy = _acc;
        _setResultStatusFlags(_idy);
        return false;
    }

    bool CPU::_inst_TSX() {
        _idx = _stackP;
        _setResultStatusFlags(_idx);
        return false;
    }

    bool CPU::_inst_TXA() {
        _acc = _idx;
        _setResultStatusFlags(_acc);
        return false;
    }

    bool CPU::_inst_TXS() {
        _stackP = _idx;
        return false;
    }

    bool CPU::_inst_TYA() {
        _acc = _idy;
        _setResultStatusFlags(_acc);
        return false;
    }


    // operations ------------------------------------------------------------------------------------------------------

    #define CPU_OP(code, inst, addr, cycles) \
        _operations[code] = (CPU::Operation){code, #inst, &CPU::_inst_##inst, &CPU::_addr_##addr, cycles}

    void CPU::_initOperations() {
        CPU_OP(0x00, BRK, IMP, 7);
        CPU_OP(0x01, ORA, IZX, 6);
        CPU_OP(0x02, KIL, IMP, 0);
        CPU_OP(0x03, SLO, IZX, 8);
        CPU_OP(0x04, NOP, ZPG, 3);
        CPU_OP(0x05, ORA, ZPG, 3);
        CPU_OP(0x06, ASL, ZPG, 5);
        CPU_OP(0x07, SLO, ZPG, 5);
        CPU_OP(0x08, PHP, IMP, 3);
        CPU_OP(0x09, ORA, IMM, 2);
        CPU_OP(0x0A, ASL, IMP, 2);
        CPU_OP(0x0B, ANC, IMM, 2);
        CPU_OP(0x0C, NOP, ABS, 4);
        CPU_OP(0x0D, ORA, ABS, 4);
        CPU_OP(0x0E, ASL, ABS, 6);
        CPU_OP(0x0F, SLO, ABS, 6);

        CPU_OP(0x10, BPL, REL, 2); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0x11, ORA, IZY, 5); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0x12, KIL, IMP, 0);
        CPU_OP(0x13, SLO, IZY, 8);
        CPU_OP(0x14, NOP, ZPX, 4);
        CPU_OP(0x15, ORA, ZPX, 4);
        CPU_OP(0x16, ASL, ZPX, 6);
        CPU_OP(0x17, SLO, ZPX, 6);
        CPU_OP(0x18, CLC, IMP, 2);
        CPU_OP(0x19, ORA, ABY, 4); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0x1A, NOP, IMP, 2);
        CPU_OP(0x1B, SLO, ABY, 7);
        CPU_OP(0x1C, NOP, ABX, 4); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0x1D, ORA, ABX, 4); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0x1E, ASL, ABX, 7);
        CPU_OP(0x1F, SLO, ABX, 7);

        CPU_OP(0x20, JSR, ABS, 6);
        CPU_OP(0x21, AND, IZX, 6);
        CPU_OP(0x22, KIL, IMP, 0);
        CPU_OP(0x23, RLA, IZX, 8);
        CPU_OP(0x24, BIT, ZPG, 3);
        CPU_OP(0x25, AND, ZPG, 3);
        CPU_OP(0x26, ROL, ZPG, 5);
        CPU_OP(0x27, RLA, ZPG, 5);
        CPU_OP(0x28, PLP, IMP, 4);
        CPU_OP(0x29, AND, IMM, 2);
        CPU_OP(0x2A, ROL, IMP, 2);
        CPU_OP(0x2B, ANC, IMM, 2);
        CPU_OP(0x2C, BIT, ABS, 4);
        CPU_OP(0x2D, AND, ABS, 4);
        CPU_OP(0x2E, ROL, ABS, 6);
        CPU_OP(0x2F, RLA, ABS, 6);

        CPU_OP(0x30, BMI, REL, 2); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0x31, AND, IZY, 5); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0x32, KIL, IMP, 0);
        CPU_OP(0x33, RLA, IZY, 8);
        CPU_OP(0x34, NOP, ZPX, 4);
        CPU_OP(0x35, AND, ZPX, 4);
        CPU_OP(0x36, ROL, ZPX, 6);
        CPU_OP(0x37, RLA, ZPX, 6);
        CPU_OP(0x38, SEC, IMP, 2);
        CPU_OP(0x39, AND, ABY, 4); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0x3A, NOP, IMP, 2);
        CPU_OP(0x3B, RLA, ABY, 7);
        CPU_OP(0x3C, NOP, ABX, 4); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0x3D, AND, ABX, 4); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0x3E, ROL, ABX, 7);
        CPU_OP(0x3F, RLA, ABX, 7);

        CPU_OP(0x40, RTI, IMP, 6);
        CPU_OP(0x41, EOR, IZX, 6);
        CPU_OP(0x42, KIL, IMP, 0);
        CPU_OP(0x43, SRE, IZX, 8);
        CPU_OP(0x44, NOP, ZPG, 3);
        CPU_OP(0x45, EOR, ZPG, 3);
        CPU_OP(0x46, LSR, ZPG, 5);
        CPU_OP(0x47, SRE, ZPG, 5);
        CPU_OP(0x48, PHA, IMP, 3);
        CPU_OP(0x49, EOR, IMM, 2);
        CPU_OP(0x4A, LSR, IMP, 2);
        CPU_OP(0x4B, ALR, IMM, 2);
        CPU_OP(0x4C, JMP, ABS, 3);
        CPU_OP(0x4D, EOR, ABS, 4);
        CPU_OP(0x4E, LSR, ABS, 6);
        CPU_OP(0x4F, SRE, ABS, 6);

        CPU_OP(0x50, BVC, REL, 2); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0x51, EOR, IZY, 5); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0x52, KIL, IMP, 0);
        CPU_OP(0x53, SRE, IZY, 8);
        CPU_OP(0x54, NOP, ZPX, 4);
        CPU_OP(0x55, EOR, ZPX, 4);
        CPU_OP(0x56, LSR, ZPX, 6);
        CPU_OP(0x57, SRE, ZPX, 6);
        CPU_OP(0x58, CLI, IMP, 2);
        CPU_OP(0x59, EOR, ABY, 4); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0x5A, NOP, IMP, 2);
        CPU_OP(0x5B, SRE, ABY, 7);
        CPU_OP(0x5C, NOP, ABX, 4); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0x5D, EOR, ABX, 4); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0x5E, LSR, ABX, 7);
        CPU_OP(0x5F, SRE, ABX, 7);

        CPU_OP(0x60, RTS, IMP, 6);
        CPU_OP(0x61, ADC, IZX, 6);
        CPU_OP(0x62, KIL, IMP, 0);
        CPU_OP(0x63, RRA, IZX, 8);
        CPU_OP(0x64, NOP, ZPG, 3);
        CPU_OP(0x65, ADC, ZPG, 3);
        CPU_OP(0x66, ROR, ZPG, 5);
        CPU_OP(0x67, RRA, ZPG, 5);
        CPU_OP(0x68, PLA, IMP, 4);
        CPU_OP(0x69, ADC, IMM, 2);
        CPU_OP(0x6A, ROR, IMP, 2);
        CPU_OP(0x6B, ARR, IMM, 2);
        CPU_OP(0x6C, JMP, IND, 5);
        CPU_OP(0x6D, ADC, ABS, 4);
        CPU_OP(0x6E, ROR, ABS, 6);
        CPU_OP(0x6F, RRA, ABS, 6);

        CPU_OP(0x70, BVS, REL, 2); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0x71, ADC, IZY, 5); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0x72, KIL, IMP, 0);
        CPU_OP(0x73, RRA, IZY, 8);
        CPU_OP(0x74, NOP, ZPX, 4);
        CPU_OP(0x75, ADC, ZPX, 4);
        CPU_OP(0x76, ROR, ZPX, 6);
        CPU_OP(0x77, RRA, ZPX, 6);
        CPU_OP(0x78, SEI, IMP, 2);
        CPU_OP(0x79, ADC, ABY, 4); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0x7A, NOP, IMP, 2);
        CPU_OP(0x7B, RRA, ABY, 7);
        CPU_OP(0x7C, NOP, ABX, 4); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0x7D, ADC, ABX, 4); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0x7E, ROR, ABX, 7);
        CPU_OP(0x7F, RRA, ABX, 7);

        CPU_OP(0x80, NOP, IMM, 2);
        CPU_OP(0x81, STA, IZX, 6);
        CPU_OP(0x82, NOP, IMM, 2);
        CPU_OP(0x83, SAX, IZX, 6);
        CPU_OP(0x84, STY, ZPG, 3);
        CPU_OP(0x85, STA, ZPG, 3);
        CPU_OP(0x86, STX, ZPG, 3);
        CPU_OP(0x87, SAX, ZPG, 3);
        CPU_OP(0x88, DEY, IMP, 2);
        CPU_OP(0x89, NOP, IMM, 2);
        CPU_OP(0x8A, TXA, IMP, 2);
        CPU_OP(0x8B, XAA, IMM, 2);
        CPU_OP(0x8C, STY, ABS, 4);
        CPU_OP(0x8D, STA, ABS, 4);
        CPU_OP(0x8E, STX, ABS, 4);
        CPU_OP(0x8F, SAX, ABS, 4);

        CPU_OP(0x90, BCC, REL, 2); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0x91, STA, IZY, 6);
        CPU_OP(0x92, KIL, IMP, 0);
        CPU_OP(0x93, AHX, IZY, 6);
        CPU_OP(0x94, STY, ZPX, 4);
        CPU_OP(0x95, STA, ZPX, 4);
        CPU_OP(0x96, STX, ZPY, 4);
        CPU_OP(0x97, SAX, ZPY, 4);
        CPU_OP(0x98, TYA, IMP, 2);
        CPU_OP(0x99, STA, ABY, 5);
        CPU_OP(0x9A, TXS, IMP, 2);
        CPU_OP(0x9B, TAS, ABY, 5);
        CPU_OP(0x9C, SHY, ABX, 5);
        CPU_OP(0x9D, STA, ABX, 5);
        CPU_OP(0x9E, SHX, ABY, 5);
        CPU_OP(0x9F, AHX, ABY, 5);

        CPU_OP(0xA0, LDY, IMM, 2);
        CPU_OP(0xA1, LDA, IZX, 6);
        CPU_OP(0xA2, LDX, IMM, 2);
        CPU_OP(0xA3, LAX, IZX, 6);
        CPU_OP(0xA4, LDY, ZPG, 3);
        CPU_OP(0xA5, LDA, ZPG, 3);
        CPU_OP(0xA6, LDX, ZPG, 3);
        CPU_OP(0xA7, LAX, ZPG, 3);
        CPU_OP(0xA8, TAY, IMP, 2);
        CPU_OP(0xA9, LDA, IMM, 2);
        CPU_OP(0xAA, TAX, IMP, 2);
        CPU_OP(0xAB, LAX, IMM, 2);
        CPU_OP(0xAC, LDY, ABS, 4);
        CPU_OP(0xAD, LDA, ABS, 4);
        CPU_OP(0xAE, LDX, ABS, 4);
        CPU_OP(0xAF, LAX, ABS, 4);

        CPU_OP(0xB0, BCS, REL, 2); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0xB1, LDA, IZY, 5); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0xB2, KIL, IMP, 0);
        CPU_OP(0xB3, LAX, IZY, 5); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0xB4, LDY, ZPX, 4);
        CPU_OP(0xB5, LDA, ZPX, 4);
        CPU_OP(0xB6, LDX, ZPY, 4);
        CPU_OP(0xB7, LAX, ZPY, 4);
        CPU_OP(0xB8, CLV, IMP, 2);
        CPU_OP(0xB9, LDA, ABY, 4); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0xBA, TSX, IMP, 2);
        CPU_OP(0xBB, LAS, ABY, 4); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0xBC, LDY, ABX, 4); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0xBD, LDA, ABX, 4); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0xBE, LDX, ABY, 4); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0xBF, LAX, ABY, 4); // TODO: Add 1 cycle if page boundary is crossed

        CPU_OP(0xC0, CPY, IMM, 2);
        CPU_OP(0xC1, CMP, IZX, 6);
        CPU_OP(0xC2, NOP, IMM, 2);
        CPU_OP(0xC3, DCP, IZX, 8);
        CPU_OP(0xC4, CPY, ZPG, 3);
        CPU_OP(0xC5, CMP, ZPG, 3);
        CPU_OP(0xC6, DEC, ZPG, 5);
        CPU_OP(0xC7, DCP, ZPG, 5);
        CPU_OP(0xC8, INY, IMP, 2);
        CPU_OP(0xC9, CMP, IMM, 2);
        CPU_OP(0xCA, DEX, IMP, 2);
        CPU_OP(0xCB, AXS, IMM, 2);
        CPU_OP(0xCC, CPY, ABS, 4);
        CPU_OP(0xCD, CMP, ABS, 4);
        CPU_OP(0xCE, DEC, ABS, 6);
        CPU_OP(0xCF, DCP, ABS, 6);

        CPU_OP(0xD0, BNE, REL, 2); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0xD1, CMP, IZY, 5); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0xD2, KIL, IMP, 0);
        CPU_OP(0xD3, DCP, IZY, 8);
        CPU_OP(0xD4, NOP, ZPX, 4);
        CPU_OP(0xD5, CMP, ZPX, 4);
        CPU_OP(0xD6, DEC, ZPX, 6);
        CPU_OP(0xD7, DCP, ZPX, 6);
        CPU_OP(0xD8, CLD, IMP, 2);
        CPU_OP(0xD9, CMP, ABY, 4); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0xDA, NOP, IMP, 2);
        CPU_OP(0xDB, DCP, ABY, 7);
        CPU_OP(0xDC, NOP, ABX, 4); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0xDD, CMP, ABX, 4); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0xDE, DEC, ABX, 7);
        CPU_OP(0xDF, DCP, ABX, 7);

        CPU_OP(0xE0, CPX, IMM, 2);
        CPU_OP(0xE1, SBC, IZX, 6);
        CPU_OP(0xE2, NOP, IMM, 2);
        CPU_OP(0xE3, ISC, IZX, 8);
        CPU_OP(0xE4, CPX, ZPG, 3);
        CPU_OP(0xE5, SBC, ZPG, 3);
        CPU_OP(0xE6, INC, ZPG, 5);
        CPU_OP(0xE7, ISC, ZPG, 5);
        CPU_OP(0xE8, INX, IMP, 2);
        CPU_OP(0xE9, SBC, IMM, 2);
        CPU_OP(0xEA, NOP, IMP, 2);
        CPU_OP(0xEB, SBC, IMM, 2);
        CPU_OP(0xEC, CPX, ABS, 4);
        CPU_OP(0xED, SBC, ABS, 4);
        CPU_OP(0xEE, INC, ABS, 6);
        CPU_OP(0xEF, ISC, ABS, 6);

        CPU_OP(0xF0, BEQ, REL, 2); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0xF1, SBC, IZY, 5); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0xF2, KIL, IMP, 0);
        CPU_OP(0xF3, ISC, IZY, 8);
        CPU_OP(0xF4, NOP, ZPX, 4);
        CPU_OP(0xF5, SBC, ZPX, 4);
        CPU_OP(0xF6, INC, ZPX, 6);
        CPU_OP(0xF7, ISC, ZPX, 6);
        CPU_OP(0xF8, SED, IMP, 2);
        CPU_OP(0xF9, SBC, ABY, 4); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0xFA, NOP, IMP, 2);
        CPU_OP(0xFB, ISC, ABY, 7);
        CPU_OP(0xFC, NOP, ABX, 4); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0xFD, SBC, ABX, 4); // TODO: Add 1 cycle if page boundary is crossed
        CPU_OP(0xFE, INC, ABX, 7);
        CPU_OP(0xFF, ISC, ABX, 7);
    }
}
