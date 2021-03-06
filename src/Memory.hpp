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
#include "Addressable.hpp"

namespace rt_6502_emulator {

    // Memory peripheral that can be attached to the bus to be used either as RAM or ROM.
    class Memory: public Addressable {
    public:

        /// Constructs a RAM or ROM memory module with the given address range.
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


        /// Copies the given data into the memory address space.
        ///
        /// @param buffer  the source data to copy
        /// @param address the destination address
        /// @param length  number of bytes to copy
        ///
        /// @returns `true` if data copied successfully
        bool load(byte *buffer, word address, word length);


        /// Read a byte from the memory address space.
        ///
        /// @param address the address from which to read
        /// @param data    reference to a data byte variable
        ///
        /// @returns `true` if address was read from
        virtual bool read(word address, byte &data);

        /// Write a byte to the memory address space.
        ///
        /// If configured as a ROM, this function is a no-op.
        ///
        /// @param address the address to which to write
        /// @param data    the data byte to write
        virtual bool write(word address, byte data);

    private:

        bool   _isWritable;
        word   _addressStart;
        word   _addressEnd;
        byte  *_contents;
    };
}


#endif // __RT_6502_EMULATOR_MEMORY_HPP__
