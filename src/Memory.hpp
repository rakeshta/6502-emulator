//
//  Memory.hpp
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 2 Apr 2020.
//  Copyright (c) 2020 Rakesh Ayyaswami. All rights reserved.
//

#ifndef __RT_6502_EMULATOR_MEMORY_HPP__
#define __RT_6502_EMULATOR_MEMORY_HPP__

#include "types.hpp"
#include "IAddressable.hpp"

namespace rt_6502_emulator {

    // Memory peripheral that can be attached to the bus to be used either as RAM or ROM.
    class Memory: public IAddressable {
    public:

        /// Constructs a RAM or ROM memory module with the tiven address range.
        ///
        /// @param isWritable   initializes as a ROM module if `true` or as RAM otherwise
        /// @param addressStart start range of address at which to map the memory
        /// @param addressEnd   end range of address at which to map the memory
        Memory(bool isWritable, word addressStart, word addressEnd);

        /// Copy constructor
        Memory(const Memory &orig);

        /// Destructor
        ~Memory();


        /// Returns `true` always.
        virtual bool isReadable();

        /// Returns `true` if configured as ROM.
        virtual bool isWritable();

        /// Start range of the memory address space.
        virtual word addressStart();

        /// End range of the memory address space.
        virtual word addressEnd();

        /// Read a byte from the memory address space.
        ///
        /// @param address the address from which to read
        ///
        /// @returns a byte read from the peripheral
        virtual byte read(word address);

        /// Write a byte to the memory address space.
        ///
        /// If configured as a ROM, this function is a no-op.
        ///
        /// @param address the address to which to write
        /// @param data    the data byte to write
        virtual void write(word address, byte data);

    private:

        bool   _isWritable;
        word   _addressStart;
        word   _addressEnd;
        byte  *_contents;
    };
}


#endif // __RT_6502_EMULATOR_MEMORY_HPP__