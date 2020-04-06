//
//  Addressable.hpp
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 01 Apr 2020.
//  Copyright (c) 2020 Pixxa LLC. All rights reserved.
//

#ifndef __RT_6502_EMULATOR_ADDRESSABLE_HPP__
#define __RT_6502_EMULATOR_ADDRESSABLE_HPP__

#include "types.hpp"

namespace rt_6502_emulator {

    /// Abstract class to be implemented by peripherals attached to the bus.
    class Addressable {
    public:

        /// Returns `true` if this peripheral can be read from.
        virtual bool isReadable()    = 0;

        /// Returns `true` if this peripheral can be written to.
        virtual bool isWritable()    = 0;

        /// Start range of the peripherals address space.
        virtual word addressStart()  = 0;

        /// End range of the peripherals address space.
        virtual word addressEnd()    = 0;

        /// Read a byte from the peripherals address space.
        ///
        /// @param address the address from which to read
        /// @param data    reference to a data byte variable
        ///
        /// @returns `true` if address was read from
        virtual bool read(word address, byte &data) = 0;

        /// Write a byte to the peripherals address space.
        ///
        /// @param address the address to which to write
        /// @param data    the data byte to write
        ///
        /// @returns `true` if address was written to
        virtual bool write(word address, byte data) = 0;
    };
}

#endif // __RT_6502_EMULATOR_ADDRESSABLE_HPP__
