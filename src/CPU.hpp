//
//  CPU.hpp
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 30 Mar 2020.
//  Copyright (c) 2020 Rakesh Ayyaswami. All rights reserved.
//

#ifndef __RT_6502_EMULATOR_CPU_HPP__
#define __RT_6502_EMULATOR_CPU_HPP__

#include "types.hpp"
#include "Bus.hpp"

namespace rt_6502_emulator {

    /// The 6502 CPU.
    ///
    /// References:
    /// - https://www.masswerk.at/6502/6502_instruction_set.html
    /// - http://www.oxyron.de/html/opcodes02.html
    /// - http://archive.6502.org/datasheets/mos_6501-6505_mpu_preliminary_aug_1975.pdf
    /// - http://nesdev.com/6502bugs.txt
    class CPU : public Bus {

    // status flags ----------------------------------------------------------------------------------------------------
    public:

        /// Status flags on the status register
        enum STATUS_FLAG {
            STATUS_FLAG_CARRY              = (1 << 0),
            STATUS_FLAG_ZERO               = (1 << 1),
            STATUS_FLAG_DISABLE_INTERRUPTS = (1 << 2),
            STATUS_FLAG_DECIMAL            = (1 << 3),
            STATUS_FLAG_BREAK              = (1 << 4),
            STATUS_FLAG_UNUSED             = (1 << 5),
            STATUS_FLAG_OVERFLOW           = (1 << 6),
            STATUS_FLAG_NEGATIVE           = (1 << 7),
        };

        /// Interrupt type. Used to indicate the last requested interrupt type.
        enum INTERRUPT_TYPE {
            INTERRUPT_TYPE_NONE,
            INTERRUPT_TYPE_MASKABLE,
            INTERRUPT_TYPE_NON_MASKABLE,
        };


    // accessors -------------------------------------------------------------------------------------------------------
    public:

        /// Gets the current value in the accumulator
        byte getAccumulator();

        /// Gets the current value in the x (index) register
        byte getIndexX();

        /// Gets the current value in the y (index) register
        byte getIndexY();

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

        /// Triggers a maskable interrupt if `DISABLE_INTERRUPTS` status bit is not set.
        ///
        /// Triggering an interrupt causes:
        /// - The program counter & status to be pushed on the stack.
        /// - The status bit `DISABLE_INTERRUPTS` to be set.
        /// - The address at vector 0xFFFE, 0xFFFF to be loaded onto the program counter.
        void irq();

        /// Triggers a non-maskable interrupt.
        ///
        /// A non-maskable interrupt is similar to a maskable interrupt except for the following:
        /// - The `DISABLE_INTERRUPTS` status bit does not apply to a non-maskable interrupt.
        /// - The address vector loaded onto the program counter is at 0xFFFA, 0xFFFB.
        void nmi();

        /// Performs one clocks worth of operations.
        ///
        /// Since this is a behavior level emulation, the entire instruction is executed in one clock tick. The
        /// remaining ticks required for the current instruction to complete just cause the emulator to wait.
        void tick();

        /// Executes one instruction before returning. This is useful for debugging and stepping through the program.
        /// It fires as many clock ticks as required to complete the instruction.
        ///
        /// @returns the number of clock ticks elapsed
        byte step();


    // internal state  -------------------------------------------------------------------------------------------------
    private:

        byte   _acc;            // accumulator
        byte   _idx;            // x index register
        byte   _idy;            // y index register
        byte   _stackP;         // stack pointer
        byte   _status;         // status register
        word   _pc;             // program counter

        byte   _opCycles;       // tracks remaining clock cycles in an active operation

        bool   _opTargetAcc;    // set to true by the addressing mode if the target is the accumulator
        word   _opAddress;      // target address computed by the addressing mode of the active operation

        byte   _interruptType;  // tracks the last requested interrupt type


    // execution helpers -----------------------------------------------------------------------------------------------
    private:

        /// Tests if any interrupt has been requested.
        ///
        /// @returns `true` if a pending request exists.
        bool _isInterruptRequested();

        /// Executes interrupt request if any.
        void _interrupt();

        /// Executes the next operation in the program.
        void _execute();


    // status register helpers -----------------------------------------------------------------------------------------
    private:

        /// Tests whether the specified bit in the status register is on.
        ///
        /// @param bit the bit (status flag) to test
        ///
        /// @returns `true` if the specified status bit is `1`
        bool _getStatusFlag(STATUS_FLAG bit);

        /// Sets or resets the specified bit in the status register.
        ///
        /// @param bit   the bit (status flag) to update
        /// @param value `true` sets the bit to `1` and `false` sets it to `0`
        void _setStatusFlag(STATUS_FLAG bit, bool value);

        /// Convenience method to set the zero & negative bits on the status register based on the result of the last
        /// CPU operation.
        ///
        /// @param data the result of the last CPU operation
        void _setResultStatusFlags(byte data);


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

        /// Convenience function to push a byte to the top of the stack.
        ///
        /// @param the data byte to push
        void _pushByte(byte data);

        /// Convenience function to push a word to the top of the stack. The MSB is pushed first.
        ///
        /// @param the data word to push
        void _pushWord(word data);

        /// Convenience function to pop a byte from the stack.
        ///
        /// @param the data byte to pop
        byte _popByte();

        /// Convenience function to pop a word from the stack. The MSB is popped second.
        ///
        /// @param the data word to pop
        word _popWord();


    // addressing modes ------------------------------------------------------------------------------------------------
    private:

        /// Convenience function to fetch the operand based on the executing instruction's addressing mode.
        byte _fetch();

        /// Convenience function to store result based on the executing instruction's addressing mode.
        void _store(byte data);

        /// Implied - The source / destination of the operand is implied in the instruction itself.
        bool _addr_IMP();

        /// Accumulator - The operand is implicitly defined as the accumulator.
        bool _addr_ACC();

        /// Immediate - A single byte operand follows the instruction in the next position in memory.
        bool _addr_IMM();

        /// Zero Page - A single byte address offset follows the instruction. The page (MSB) of the address
        /// is always 0x00.
        bool _addr_ZPG();

        /// Zero Page, X - Similar to the zero page addressing but with the target address calculated by
        /// adding the `X` register value to the address.
        bool _addr_ZPX();

        /// Zero Page, Y - Similar to the zero page addressing but with the target address calculated by
        /// adding the `Y` register value to the address.
        bool _addr_ZPY();

        /// Relative - A single byte address offset follows the instruction. The page (MSB) of the address
        /// is obtained from the program counter.
        bool _addr_REL();

        /// Absolute - A two byte address follows the instruction. The first byte is the LSB (bits 0 to 7)
        /// and the second byte is the MSB (bits 8 to 15).
        bool _addr_ABS();

        /// Absolute, X - Similar to absolute addressing but with the target address calculated by adding
        /// the `X` register value to the address in the instruction.
        bool _addr_ABX();

        /// Absolute, Y - Similar to absolute addressing but with the target address calculated by adding
        /// the `Y` register value to the address in the instruction
        bool _addr_ABY();

        /// Indirect - A two byte address follows the instruction that points to the location where the
        /// LSB of the target address is located.
        bool _addr_IND();

        /// Indexed Indirect - A single byte zero page address offset follows the instruction. This is added
        /// to the `X` register value to get the location where the LSB of the target address is found.
        bool _addr_IZX();

        /// Indirect Indexed - A single byte zero page address offset follows the instruction. This points
        /// to the LSB of the target address which is then offset using the value of the `X` register.
        bool _addr_IZY();


    // instructions ----------------------------------------------------------------------------------------------------
    private:

        /* Instructions for Illegal Op Codes */

        /// Halts the CPU
        bool _inst_KIL();

        /// TBD - Illegal op code
        bool _inst_SLO();

        /// TBD - Illegal op code
        bool _inst_RLA();

        /// TBD - Illegal op code
        bool _inst_SRE();

        /// TBD - Illegal op code
        bool _inst_RRA();

        /// TBD - Illegal op code
        bool _inst_SAX();

        /// TBD - Illegal op code
        bool _inst_LAX();

        /// TBD - Illegal op code
        bool _inst_DCP();

        /// TBD - Illegal op code
        bool _inst_ISC();

        /// TBD - Illegal op code
        bool _inst_ANC();

        /// TBD - Illegal op code
        bool _inst_ALR();

        /// TBD - Illegal op code
        bool _inst_ARR();

        /// TBD - Illegal op code
        bool _inst_XAA();

        /// TBD - Illegal op code
        bool _inst_AXS();

        /// TBD - Illegal op code
        bool _inst_AHX();

        /// TBD - Illegal op code
        bool _inst_SHY();

        /// TBD - Illegal op code
        bool _inst_SHX();

        /// TBD - Illegal op code
        bool _inst_TAS();

        /// TBD - Illegal op code
        bool _inst_LAS();


        /* Instructions for Legal Op codes  */

        /// Add with carry
        bool _inst_ADC();

        /// And (with accumulator)
        bool _inst_AND();

        /// Arithmetic shift left
        bool _inst_ASL();

        /// Branch on carry clear
        bool _inst_BCC();

        /// Branch on carry set
        bool _inst_BCS();

        /// Branch on equal (zero set)
        bool _inst_BEQ();

        /// Bit test
        bool _inst_BIT();

        /// Branch on minus (negative set)
        bool _inst_BMI();

        /// Branch on not equal (zero clear)
        bool _inst_BNE();

        /// Branch on plus (negative clear)
        bool _inst_BPL();

        /// Break / interrupt
        bool _inst_BRK();

        /// Branch on overflow clear
        bool _inst_BVC();

        /// Branch on overflow set
        bool _inst_BVS();

        /// Clear carry
        bool _inst_CLC();

        /// Clear decimal
        bool _inst_CLD();

        /// Clear interrupt disable
        bool _inst_CLI();

        /// Clear overflow
        bool _inst_CLV();

        /// Compare (with accumulator)
        bool _inst_CMP();

        /// Compare with X
        bool _inst_CPX();

        /// Compare with Y
        bool _inst_CPY();

        /// Decrement
        bool _inst_DEC();

        /// Decrement X
        bool _inst_DEX();

        /// Decrement Y
        bool _inst_DEY();

        /// Exclusive or (with accumulator)
        bool _inst_EOR();

        /// Increment
        bool _inst_INC();

        /// Increment X
        bool _inst_INX();

        /// Increment Y
        bool _inst_INY();

        /// Jump
        bool _inst_JMP();

        /// Jump subroutine
        bool _inst_JSR();

        /// Load accumulator
        bool _inst_LDA();

        /// Load X
        bool _inst_LDX();

        /// Load Y
        bool _inst_LDY();

        /// Logical shift right
        bool _inst_LSR();

        /// No operation
        bool _inst_NOP();

        /// Or with accumulator
        bool _inst_ORA();

        /// Push accumulator
        bool _inst_PHA();

        /// Push processor status (SR)
        bool _inst_PHP();

        /// Pull accumulator
        bool _inst_PLA();

        /// Pull processor status (SR)
        bool _inst_PLP();

        /// Rotate left
        bool _inst_ROL();

        /// Rotate right
        bool _inst_ROR();

        /// Return from interrupt
        bool _inst_RTI();

        /// Return from subroutine
        bool _inst_RTS();

        /// Subtract with carry
        bool _inst_SBC();

        /// Set carry
        bool _inst_SEC();

        /// Set decimal
        bool _inst_SED();

        /// Set interrupt disable
        bool _inst_SEI();

        /// Store accumulator
        bool _inst_STA();

        /// Store X
        bool _inst_STX();

        /// Store Y
        bool _inst_STY();

        /// Transfer accumulator to X
        bool _inst_TAX();

        /// Transfer accumulator to Y
        bool _inst_TAY();

        /// Transfer stack pointer to X
        bool _inst_TSX();

        /// Transfer X to accumulator
        bool _inst_TXA();

        /// Transfer X to stack pointer
        bool _inst_TXS();

        /// Transfer Y to accumulator
        bool _inst_TYA();


    // operations ------------------------------------------------------------------------------------------------------
    private:

        typedef struct _Operation {
            byte   code;
            char   abbr[4];
            bool  (CPU::*inst)();
            bool  (CPU::*addr)();
            byte   cycles;
        } Operation;

        Operation _operations[256];

        void _initOperations();
    };
}

#endif // __RT_6502_EMULATOR_CPU_HPP__
