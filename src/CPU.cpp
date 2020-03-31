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

    byte CPU::getAccumulator()    { return _regA; }
    byte CPU::getRegisterX()      { return _regX; }
    byte CPU::getRegisterY()      { return _regY; }
    byte CPU::getStackPointer()   { return _stackP; }
    byte CPU::getStatus()         { return _status; }
    word CPU::getProgramCounter() { return _pc; }

    bool CPU::isOperationComplete() {
        return _opCycles == 0;
    }


    // public methods --------------------------------------------------------------------------------------------------

    void CPU::reset() {

        // reset registers
        _regA        = 0x00;
        _regX        = 0x00;
        _regY        = 0x00;
        _stackP      = 0x00;
        _status      = STATUS_FLAG_UNUSED;

        // read program start address from 0xFFFC to initialize the program counter
        word lsb     = _read(0xFFFC);
        word msb     = _read(0xFFFD);
        _pc          = (msb << 8) | lsb;

        // reset current op addressing
        _opTargetAcc = false;
        _opAddress   = 0x0000;

        // reset takes 8 clock cycles
        _opCycles    = 8;
    }

    void CPU::tick() {

        // execute next instruction if the clock ticks required for the previous instruction have elapsed.
        if (_opCycles == 0) {

            // read next operation
            byte opcode = _readNextByte();
            CPU::Operation op = _operations[opcode];

            // set the number of cycles required for the op
            _opCycles = op.cycles;

            // execute the operation
            (this->*op.addr)();
            (this->*op.inst)();

            // ensure unused flag is always set in the status register
            _setStatusFlag(STATUS_FLAG_UNUSED, true);
        }

        // decrement cycles for operation
        if (_opCycles > 0) {
            _opCycles--;
        }
    }


    // status register helpers -----------------------------------------------------------------------------------------

    bool CPU::_getStatusFlag(STATUS_FLAG bit) {
        return (_status & bit) > 0 ? true : false;
    }

    void CPU::_setStatusFlag(STATUS_FLAG bit, bool value) {
        if (value) {
            _status |= bit;
        }
        else {
            _status &= ~bit;
        }
    }


    // bus access convenience methods ----------------------------------------------------------------------------------

    byte CPU::_read(word address) {
        return 0x00;
    }

    void CPU::_write(word address, byte data) {
        // no op
    }

    byte CPU::_readNextByte() {
        return _read(_pc++);
    }

    word CPU::_readNextWord() {
        byte lsb = _read(_pc++);
        byte msb = _read(_pc++);
        return (msb << 8) | lsb;
    }


    // addressing modes ------------------------------------------------------------------------------------------------

    byte CPU::_fetch() {
        return _opTargetAcc
            ? _regA
            : _read(_opAddress);
    }

	void CPU::_addr_IMP() {
        _opTargetAcc = false;
        _opAddress   = 0x00;
	}

	void CPU::_addr_ACC() {
        _opTargetAcc = true;
        _opAddress   = 0x0000;
	}

	void CPU::_addr_IMM() {
        _opTargetAcc = false;
        _opAddress   = ++_pc;
	}

	void CPU::_addr_ZPG() {
        _opTargetAcc = false;
        _opAddress   = _readNextByte();
	}

	void CPU::_addr_ZPX() {
        _opTargetAcc = false;
        _opAddress   = 0x00FF & (_readNextByte() + _regX);
	}

	void CPU::_addr_ZPY() {
        _opTargetAcc = false;
        _opAddress   = 0x00FF & (_readNextByte() + _regY);
	}

	void CPU::_addr_REL() {
	}

	void CPU::_addr_ABS() {
	}

	void CPU::_addr_ABX() {
	}

	void CPU::_addr_ABY() {
	}

	void CPU::_addr_IND() {
	}

	void CPU::_addr_IZX() {
	}

    void CPU::_addr_IZY() {
    }


    // instructions ----------------------------------------------------------------------------------------------------

    /* Instructions for Illegal Op Codes */

    void CPU::_inst_KIL() {
    }

    void CPU::_inst_SLO() {
    }

    void CPU::_inst_RLA() {
    }

    void CPU::_inst_SRE() {
    }

    void CPU::_inst_RRA() {
    }

    void CPU::_inst_SAX() {
    }

    void CPU::_inst_LAX() {
    }

    void CPU::_inst_DCP() {
    }

    void CPU::_inst_ISC() {
    }

    void CPU::_inst_ANC() {
    }

    void CPU::_inst_ALR() {
    }

    void CPU::_inst_ARR() {
    }

    void CPU::_inst_XAA() {
    }

    void CPU::_inst_AXS() {
    }

    void CPU::_inst_AHX() {
    }

    void CPU::_inst_SHY() {
    }

    void CPU::_inst_SHX() {
    }

    void CPU::_inst_TAS() {
    }

    void CPU::_inst_LAS() {
    }



    /* Instructions for Legal Op codes  */

    void CPU::_inst_ADC() {
    }

    void CPU::_inst_AND() {
    }

    void CPU::_inst_ASL() {
    }

    void CPU::_inst_BCC() {
    }

    void CPU::_inst_BCS() {
    }

    void CPU::_inst_BEQ() {
    }

    void CPU::_inst_BIT() {
    }

    void CPU::_inst_BMI() {
    }

    void CPU::_inst_BNE() {
    }

    void CPU::_inst_BPL() {
    }

    void CPU::_inst_BRK() {
    }

    void CPU::_inst_BVC() {
    }

    void CPU::_inst_BVS() {
    }

    void CPU::_inst_CLC() {
    }

    void CPU::_inst_CLD() {
    }

    void CPU::_inst_CLI() {
    }

    void CPU::_inst_CLV() {
    }

    void CPU::_inst_CMP() {
    }

    void CPU::_inst_CPX() {
    }

    void CPU::_inst_CPY() {
    }

    void CPU::_inst_DEC() {
    }

    void CPU::_inst_DEX() {
    }

    void CPU::_inst_DEY() {
    }

    void CPU::_inst_EOR() {
    }

    void CPU::_inst_INC() {
    }

    void CPU::_inst_INX() {
    }

    void CPU::_inst_INY() {
    }

    void CPU::_inst_JMP() {
    }

    void CPU::_inst_JSR() {
    }

    void CPU::_inst_LDA() {
    }

    void CPU::_inst_LDX() {
    }

    void CPU::_inst_LDY() {
    }

    void CPU::_inst_LSR() {
    }

    void CPU::_inst_NOP() {
    }

    void CPU::_inst_ORA() {
    }

    void CPU::_inst_PHA() {
    }

    void CPU::_inst_PHP() {
    }

    void CPU::_inst_PLA() {
    }

    void CPU::_inst_PLP() {
    }

    void CPU::_inst_ROL() {
    }

    void CPU::_inst_ROR() {
    }

    void CPU::_inst_RTI() {
    }

    void CPU::_inst_RTS() {
    }

    void CPU::_inst_SBC() {
    }

    void CPU::_inst_SEC() {
    }

    void CPU::_inst_SED() {
    }

    void CPU::_inst_SEI() {
    }

    void CPU::_inst_STA() {
    }

    void CPU::_inst_STX() {
    }

    void CPU::_inst_STY() {
    }

    void CPU::_inst_TAX() {
    }

    void CPU::_inst_TAY() {
    }

    void CPU::_inst_TSX() {
    }

    void CPU::_inst_TXA() {
    }

    void CPU::_inst_TXS() {
    }

    void CPU::_inst_TYA() {
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
