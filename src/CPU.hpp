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

    private:
        byte   _regA;           // accumulator
        byte   _regX;           // x register
        byte   _regY;           // y register
        byte   _stackP;         // stack pointer
        byte   _status;         // status register
        word   _pc;             // program counter

        byte   _extraCycles;    // tracks remaining clock cycles in an active operation

    public:
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
    };
}

#endif // __RT_6502_EMULATOR_CPU_H__
