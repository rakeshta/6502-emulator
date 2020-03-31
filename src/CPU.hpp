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

	// status flags ----------------------------------------------------------------------------------------------------
	public:

        /// Status flags on the status register
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


    // internal state  -------------------------------------------------------------------------------------------------
    private:

        byte   _regA;           // accumulator
        byte   _regX;           // x register
        byte   _regY;           // y register
        byte   _stackP;         // stack pointer
        byte   _status;         // status register
        word   _pc;             // program counter

        byte   _opCycles;       // tracks remaining clock cycles in an active operation

        bool   _opTargetAcc;    // set to true by the addressing mode if the target is the accumulator
        word   _opAddress;      // target address computed by the addressing mode of the active operation


	// accessors -------------------------------------------------------------------------------------------------------
	public:

		/// Gets the current value in the accumulator
		byte getAccumulator();

		/// Gets the current value in the x (index) register
		byte getRegisterX();

		/// Gets the current value in the y (index) register
		byte getRegisterY();

		/// Gets the current value in the stack pointer
		byte getStackPointer();

		/// Gets the current value in the status register
		byte getStatus();

		/// Gets the current address in the program counter
		word getProgramCounter();

		/// Gets whether the current operation has completed executing.
		/// This is useful for debugging & single stepping through the program.
		bool isOperationComplete();


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
        void tick();


	// status register helpers -----------------------------------------------------------------------------------------
	private:

		bool _getStatusFlag(STATUS_FLAG bit);
		void _setStatusFlag(STATUS_FLAG bit, bool value);


    // bus access ------------------------------------------------------------------------------------------------------
    private:

        /// Convenience function to read a byte from the given address on the bus.
        ///
        /// @param address the address to read from
        ///
        /// @return a byte of data read from the given address
        byte _read(word address);

        /// Convenience function to write a byte to the given address on the bus.
        ///
        /// @param address the address to write to
        /// @param data    the data byte to write
        void _write(word address, byte data);

        /// Convenience function to read a byte from the address pointed to by the program counter and increment the
        /// program counter.
        ///
        /// @return a byte read from the address pointed to by the program counter
        byte _readNextByte();

        /// Convenience function to read the next two bytes from the address pointed to by the program counter and
        /// increment the program counter by 2.
        ///
        /// @return a word read from the next two bytes
        ///
        /// @note The word is read in little endian format
        word _readNextWord();


    // addressing modes ------------------------------------------------------------------------------------------------
    private:

        byte _fetch();

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
	    void _inst_KIL();

	    /// TBD - Illegal op code
	    void _inst_SLO();

	    /// TBD - Illegal op code
	    void _inst_RLA();

	    /// TBD - Illegal op code
	    void _inst_SRE();

	    /// TBD - Illegal op code
	    void _inst_RRA();

	    /// TBD - Illegal op code
	    void _inst_SAX();

	    /// TBD - Illegal op code
	    void _inst_LAX();

	    /// TBD - Illegal op code
	    void _inst_DCP();

	    /// TBD - Illegal op code
	    void _inst_ISC();

	    /// TBD - Illegal op code
	    void _inst_ANC();

	    /// TBD - Illegal op code
	    void _inst_ALR();

	    /// TBD - Illegal op code
	    void _inst_ARR();

	    /// TBD - Illegal op code
	    void _inst_XAA();

	    /// TBD - Illegal op code
	    void _inst_AXS();

	    /// TBD - Illegal op code
	    void _inst_AHX();

	    /// TBD - Illegal op code
	    void _inst_SHY();

	    /// TBD - Illegal op code
	    void _inst_SHX();

	    /// TBD - Illegal op code
	    void _inst_TAS();

	    /// TBD - Illegal op code
	    void _inst_LAS();


	    /* Instructions for Legal Op codes  */

	    /// Add with carry
		void _inst_ADC();

		/// And (with accumulator)
		void _inst_AND();

		/// Arithmetic shift left
		void _inst_ASL();

		/// Branch on carry clear
		void _inst_BCC();

		/// Branch on carry set
		void _inst_BCS();

		/// Branch on equal (zero set)
		void _inst_BEQ();

		/// Bit test
		void _inst_BIT();

		/// Branch on minus (negative set)
		void _inst_BMI();

		/// Branch on not equal (zero clear)
		void _inst_BNE();

		/// Branch on plus (negative clear)
		void _inst_BPL();

		/// Break / interrupt
		void _inst_BRK();

		/// Branch on overflow clear
		void _inst_BVC();

		/// Branch on overflow set
		void _inst_BVS();

		/// Clear carry
		void _inst_CLC();

		/// Clear decimal
		void _inst_CLD();

		/// Clear interrupt disable
		void _inst_CLI();

		/// Clear overflow
		void _inst_CLV();

		/// Compare (with accumulator)
		void _inst_CMP();

		/// Compare with X
		void _inst_CPX();

		/// Compare with Y
		void _inst_CPY();

		/// Decrement
		void _inst_DEC();

		/// Decrement X
		void _inst_DEX();

		/// Decrement Y
		void _inst_DEY();

		/// Exclusive or (with accumulator)
		void _inst_EOR();

		/// Increment
		void _inst_INC();

		/// Increment X
		void _inst_INX();

		/// Increment Y
		void _inst_INY();

		/// Jump
		void _inst_JMP();

		/// Jump subroutine
		void _inst_JSR();

		/// Load accumulator
		void _inst_LDA();

		/// Load X
		void _inst_LDX();

		/// Load Y
		void _inst_LDY();

		/// Logical shift right
		void _inst_LSR();

		/// No operation
		void _inst_NOP();

		/// Or with accumulator
		void _inst_ORA();

		/// Push accumulator
		void _inst_PHA();

		/// Push processor status (SR)
		void _inst_PHP();

		/// Pull accumulator
		void _inst_PLA();

		/// Pull processor status (SR)
		void _inst_PLP();

		/// Rotate left
		void _inst_ROL();

		/// Rotate right
		void _inst_ROR();

		/// Return from interrupt
		void _inst_RTI();

		/// Return from subroutine
		void _inst_RTS();

		/// Subtract with carry
		void _inst_SBC();

		/// Set carry
		void _inst_SEC();

		/// Set decimal
		void _inst_SED();

		/// Set interrupt disable
		void _inst_SEI();

		/// Store accumulator
		void _inst_STA();

		/// Store X
		void _inst_STX();

		/// Store Y
		void _inst_STY();

		/// Transfer accumulator to X
		void _inst_TAX();

		/// Transfer accumulator to Y
		void _inst_TAY();

		/// Transfer stack pointer to X
		void _inst_TSX();

		/// Transfer X to accumulator
		void _inst_TXA();

		/// Transfer X to stack pointer
		void _inst_TXS();

		/// Transfer Y to accumulator
		void _inst_TYA();


    // operations ------------------------------------------------------------------------------------------------------
    private:

        typedef struct _Operation {
			byte   code;
            char   abbr[4];
            void  (CPU::*inst)();
            void  (CPU::*addr)();
            byte   cycles;
        } Operation;

        Operation _operations[256];

		void _initOperations();
    };
}

#endif // __RT_6502_EMULATOR_CPU_H__
