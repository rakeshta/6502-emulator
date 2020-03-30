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
        reset();
    }

    CPU::~CPU() {
        // nothing to do atm
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
        word lsb     = read(0xFFFC);
        word msb     = read(0xFFFD);
        _pc          = (msb << 8) | lsb;

        // reset takes 8 clock cycles
        _extraCycles = 8;
    }


    // bus access convenience methods ----------------------------------------------------------------------------------

    byte CPU::read(word address) {
        return 0x00;
    }

    void CPU::write(word address, byte data) {
        // no op
    }

    byte CPU::readNextByte() {
        return read(_pc++);
    }

    word CPU::readNextWord() {
        byte lsb = read(_pc++);
        byte msb = read(_pc++);
        return (msb << 8) | lsb;
    }


    // addressing modes ------------------------------------------------------------------------------------------------

	void CPU::_addressing_IMP() {
	}

	void CPU::_addressing_ACC() {
	}

	void CPU::_addressing_IMM() {
	}

	void CPU::_addressing_ZPG() {
	}

	void CPU::_addressing_ZPX() {
	}

	void CPU::_addressing_ZPY() {
	}

	void CPU::_addressing_REL() {
	}

	void CPU::_addressing_ABS() {
	}

	void CPU::_addressing_ABX() {
	}

	void CPU::_addressing_ABY() {
	}

	void CPU::_addressing_IND() {
	}

	void CPU::_addressing_IZX() {
	}

	void CPU::_addressing_IZY() {
	}
}
