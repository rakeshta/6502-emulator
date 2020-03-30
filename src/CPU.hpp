//
//  CPU.hpp
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 30 Mar 2020.
//  Copyright (c) 2020 Rakesh Ayyaswami. All rights reserved.
//

#ifndef __RT_6502_EMULATOR_CPU_H__
#define __RT_6502_EMULATOR_CPU_H__

#include "types.hpp"

namespace rt_6502_emulator {

    /// The 6502 CPU
    class CPU {

    // internal state  -------------------------------------------------------------------------------------------------
    private:

        /// Status flags on the 6502 status register
        enum STATUS_FLAG {
            STATUS_FLAG_CARRY             = (1 << 0),
            STATUS_FLAG_ZERO              = (1 << 1),
            STATUS_FLAG_DISABLEINTERRUPTS = (1 << 2),
            STATUS_FLAG_DECIMAL           = (1 << 3),
            STATUS_FLAG_BREAK             = (1 << 4),
            STATUS_FLAG_UNUSED            = (1 << 5),
            STATUS_FLAG_OVERFLOW          = (1 << 6),
            STATUS_FLAG_NEGATIVE          = (1 << 7),
        };

        byte   _regA;           // accumulator
        byte   _regX;           // x register
        byte   _regY;           // y register
        byte   _stackP;         // stack pointer
        byte   _status;         // status register
        word   _pc;             // program counter

        byte   _extraCycles;    // tracks remaining clock cycles in an active operation


    // public methods  -------------------------------------------------------------------------------------------------
    public:

        /// Constructs and resets an instance of the 6502 CPU emulator.
        CPU();
        ~CPU();

        /// Resets the 6502 to a known state.
        ///
        /// Resetting the 6502 has the following effects:
        /// - The instruction registers are reset to 0x00
        /// - The status register is reset with the unused bit set
        /// - The stack pointer is reset to the top of the stack at 0x00FD
        /// - The vector at address 0xFFFC & 0xFFFD is loaded into the program counter
        void reset();

        /// Performs one clocks worth of operations.
        void clock();


    // bus access ------------------------------------------------------------------------------------------------------
    protected:

        /// Convenience function to read a byte from the given address on the bus.
        ///
        /// @param address the address to read from
        ///
        /// @return a byte of data read from the given address
        byte read(word address);

        /// Convenience function to write a byte to the given address on the bus.
        ///
        /// @param address the address to write to
        /// @param data    the data byte to write
        void write(word address, byte data);

        /// Convenience function to read a byte from the address pointed to by the program counter and increment the
        /// program counter.
        ///
        /// @return a byte read from the address pointed to by the program counter
        byte readNextByte();

        /// Convenience function to read the next two bytes from the address pointed to by the program counter and
        /// increment the program counter by 2.
        ///
        /// @return a word read from the next two bytes
        ///
        /// @note The word is read in little endian format
        word readNextWord();


    // addressing modes ------------------------------------------------------------------------------------------------
    private:

	    /// Implied - The source / destination of the operand is implied in the instruction itself.
	    void _addr_IMP();

	    /// Accumulator - The operand is implicitly defined as the accumulator.
	    void _addr_ACC();

	    /// Immediate - A single byte operand follows the inscturtion in the next position in memory.
	    void _addr_IMM();

	    /// Zero Page - A single byte address offset follows the instruction. The page (MSB) of the address
	    /// is always 0x00.
	    void _addr_ZPG();

	    /// Zero Page, X - Similar to the zero page addressing but with the target address calculated by
	    /// adding the `X` register value to the address.
	    void _addr_ZPX();

	    /// Zero Page, Y - Similar to the zero page addressing but with the target address calculated by
	    /// adding the `Y` register value to the address.
	    void _addr_ZPY();

	    /// Relative - A single byte address offset follows the instruction. The page (MSB) of the address
	    /// is obtained from the program counter.
	    void _addr_REL();

	    /// Absolute - A two byte address follows the instruction. The first byte is the LSB (bits 0 to 7)
	    /// and the second byte is the MSB (bits 8 to 15).
	    void _addr_ABS();

	    /// Absolute, X - Similar to absolute addressing but with the target address calculated by adding
	    /// the `X` register value to the address in the instruction.
	    void _addr_ABX();

	    /// Absolute, Y - Similar to absolute addressing but with the target address calculated by adding
	    /// the `Y` register value to the address in the instruction
	    void _addr_ABY();

	    /// Indirect - A two byte address follows the instruction that points to the location where the
	    /// LSB of the target address is located.
	    void _addr_IND();

	    /// Indexed Indirect - A single byte zero page address offset follows the instruction. This is added
	    /// to the `X` register value to get the location where the LSB of the target address is found.
	    void _addr_IZX();

	    /// Indirect Indexed - A single byte zero page address offset follows the instruction. This points
	    /// to the LSB of the target address which is then offset using the value of the `X` register.
	    void _addr_IZY();


    // instructions ----------------------------------------------------------------------------------------------------
    private:

        /* Instructions for Illegal Op Codes */

	    /// Halts the CPU
	    void _instr_KIL();

	    /// TBD - Illegal op code
	    void _instr_SLO();

	    /// TBD - Illegal op code
	    void _instr_RLA();

	    /// TBD - Illegal op code
	    void _instr_SRE();

	    /// TBD - Illegal op code
	    void _instr_RRA();

	    /// TBD - Illegal op code
	    void _instr_SAX();

	    /// TBD - Illegal op code
	    void _instr_LAX();

	    /// TBD - Illegal op code
	    void _instr_DCP();

	    /// TBD - Illegal op code
	    void _instr_ISC();

	    /// TBD - Illegal op code
	    void _instr_ANC();

	    /// TBD - Illegal op code
	    void _instr_ALR();

	    /// TBD - Illegal op code
	    void _instr_ARR();

	    /// TBD - Illegal op code
	    void _instr_XAA();

	    /// TBD - Illegal op code
	    void _instr_AXS();

	    /// TBD - Illegal op code
	    void _instr_AHX();

	    /// TBD - Illegal op code
	    void _instr_SHY();

	    /// TBD - Illegal op code
	    void _instr_SHX();

	    /// TBD - Illegal op code
	    void _instr_TAS();

	    /// TBD - Illegal op code
	    void _instr_LAS();


	    /* Instructions for Legal Op codes  */

	    /// Add with carry
		void _instr_ADC();

		/// And (with accumulator)
		void _instr_AND();

		/// Arithmetic shift left
		void _instr_ASL();

		/// Branch on carry clear
		void _instr_BCC();

		/// Branch on carry set
		void _instr_BCS();

		/// Branch on equal (zero set)
		void _instr_BEQ();

		/// Bit test
		void _instr_BIT();

		/// Branch on minus (negative set)
		void _instr_BMI();

		/// Branch on not equal (zero clear)
		void _instr_BNE();

		/// Branch on plus (negative clear)
		void _instr_BPL();

		/// Break / interrupt
		void _instr_BRK();

		/// Branch on overflow clear
		void _instr_BVC();

		/// Branch on overflow set
		void _instr_BVS();

		/// Clear carry
		void _instr_CLC();

		/// Clear decimal
		void _instr_CLD();

		/// Clear interrupt disable
		void _instr_CLI();

		/// Clear overflow
		void _instr_CLV();

		/// Compare (with accumulator)
		void _instr_CMP();

		/// Compare with X
		void _instr_CPX();

		/// Compare with Y
		void _instr_CPY();

		/// Decrement
		void _instr_DEC();

		/// Decrement X
		void _instr_DEX();

		/// Decrement Y
		void _instr_DEY();

		/// Exclusive or (with accumulator)
		void _instr_EOR();

		/// Increment
		void _instr_INC();

		/// Increment X
		void _instr_INX();

		/// Increment Y
		void _instr_INY();

		/// Jump
		void _instr_JMP();

		/// Jump subroutine
		void _instr_JSR();

		/// Load accumulator
		void _instr_LDA();

		/// Load X
		void _instr_LDX();

		/// Load Y
		void _instr_LDY();

		/// Logical shift right
		void _instr_LSR();

		/// No operation
		void _instr_NOP();

		/// Or with accumulator
		void _instr_ORA();

		/// Push accumulator
		void _instr_PHA();

		/// Push processor status (SR)
		void _instr_PHP();

		/// Pull accumulator
		void _instr_PLA();

		/// Pull processor status (SR)
		void _instr_PLP();

		/// Rotate left
		void _instr_ROL();

		/// Rotate right
		void _instr_ROR();

		/// Return from interrupt
		void _instr_RTI();

		/// Return from subroutine
		void _instr_RTS();

		/// Subtract with carry
		void _instr_SBC();

		/// Set carry
		void _instr_SEC();

		/// Set decimal
		void _instr_SED();

		/// Set interrupt disable
		void _instr_SEI();

		/// Store accumulator
		void _instr_STA();

		/// Store X
		void _instr_STX();

		/// Store Y
		void _instr_STY();

		/// Transfer accumulator to X
		void _instr_TAX();

		/// Transfer accumulator to Y
		void _instr_TAY();

		/// Transfer stack pointer to X
		void _instr_TSX();

		/// Transfer X to accumulator
		void _instr_TXA();

		/// Transfer X to stack pointer
		void _instr_TXS();

		/// Transfer Y to accumulator
		void _instr_TYA();


    // operations ------------------------------------------------------------------------------------------------------
    private:

        struct Operation {
            char   code[4];
            void  (CPU::*instruction)();
            void  (CPU::*addressing)();
            byte   cycles;
        };

        struct Operation _operations[1] = {
            {"BRK", &CPU::_instr_BRK, &CPU::_addr_IMM, 7},
        };


    };
}

#endif // __RT_6502_EMULATOR_CPU_H__
