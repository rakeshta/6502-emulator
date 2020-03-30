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

	void CPU::_addr_IMP() {
	}

	void CPU::_addr_ACC() {
	}

	void CPU::_addr_IMM() {
	}

	void CPU::_addr_ZPG() {
	}

	void CPU::_addr_ZPX() {
	}

	void CPU::_addr_ZPY() {
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

    void CPU::_instr_KIL() {
    }

    void CPU::_instr_SLO() {
    }

    void CPU::_instr_RLA() {
    }

    void CPU::_instr_SRE() {
    }

    void CPU::_instr_RRA() {
    }

    void CPU::_instr_SAX() {
    }

    void CPU::_instr_LAX() {
    }

    void CPU::_instr_DCP() {
    }

    void CPU::_instr_ISC() {
    }

    void CPU::_instr_ANC() {
    }

    void CPU::_instr_ALR() {
    }

    void CPU::_instr_ARR() {
    }

    void CPU::_instr_XAA() {
    }

    void CPU::_instr_AXS() {
    }

    void CPU::_instr_AHX() {
    }

    void CPU::_instr_SHY() {
    }

    void CPU::_instr_SHX() {
    }

    void CPU::_instr_TAS() {
    }

    void CPU::_instr_LAS() {
    }



    /* Instructions for Legal Op codes  */

    void CPU::_instr_ADC() {
    }

    void CPU::_instr_AND() {
    }

    void CPU::_instr_ASL() {
    }

    void CPU::_instr_BCC() {
    }

    void CPU::_instr_BCS() {
    }

    void CPU::_instr_BEQ() {
    }

    void CPU::_instr_BIT() {
    }

    void CPU::_instr_BMI() {
    }

    void CPU::_instr_BNE() {
    }

    void CPU::_instr_BPL() {
    }

    void CPU::_instr_BRK() {
    }

    void CPU::_instr_BVC() {
    }

    void CPU::_instr_BVS() {
    }

    void CPU::_instr_CLC() {
    }

    void CPU::_instr_CLD() {
    }

    void CPU::_instr_CLI() {
    }

    void CPU::_instr_CLV() {
    }

    void CPU::_instr_CMP() {
    }

    void CPU::_instr_CPX() {
    }

    void CPU::_instr_CPY() {
    }

    void CPU::_instr_DEC() {
    }

    void CPU::_instr_DEX() {
    }

    void CPU::_instr_DEY() {
    }

    void CPU::_instr_EOR() {
    }

    void CPU::_instr_INC() {
    }

    void CPU::_instr_INX() {
    }

    void CPU::_instr_INY() {
    }

    void CPU::_instr_JMP() {
    }

    void CPU::_instr_JSR() {
    }

    void CPU::_instr_LDA() {
    }

    void CPU::_instr_LDX() {
    }

    void CPU::_instr_LDY() {
    }

    void CPU::_instr_LSR() {
    }

    void CPU::_instr_NOP() {
    }

    void CPU::_instr_ORA() {
    }

    void CPU::_instr_PHA() {
    }

    void CPU::_instr_PHP() {
    }

    void CPU::_instr_PLA() {
    }

    void CPU::_instr_PLP() {
    }

    void CPU::_instr_ROL() {
    }

    void CPU::_instr_ROR() {
    }

    void CPU::_instr_RTI() {
    }

    void CPU::_instr_RTS() {
    }

    void CPU::_instr_SBC() {
    }

    void CPU::_instr_SEC() {
    }

    void CPU::_instr_SED() {
    }

    void CPU::_instr_SEI() {
    }

    void CPU::_instr_STA() {
    }

    void CPU::_instr_STX() {
    }

    void CPU::_instr_STY() {
    }

    void CPU::_instr_TAX() {
    }

    void CPU::_instr_TAY() {
    }

    void CPU::_instr_TSX() {
    }

    void CPU::_instr_TXA() {
    }

    void CPU::_instr_TXS() {
    }

    void CPU::_instr_TYA() {
    }

}
