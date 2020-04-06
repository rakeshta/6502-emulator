//
//  Bus.hpp
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 7 Apr 2020.
//  Copyright (c) 2020 Rakesh Ayyaswami. All rights reserved.
//

#ifndef __RT_6502_EMULATOR_BUS_HPP__
#define __RT_6502_EMULATOR_BUS_HPP__

#include <vector>
#include <memory>
#include "types.hpp"
#include "Addressable.hpp"

namespace rt_6502_emulator {

    /// The bus is used to connect device (like RAM, ROM etc) to the CPU.
    /// It handles the muktiplexing of addressing between the CPU & device.
    class Bus: public Addressable {
    public:

        /// Constructs an instance of the Bus with no devices attached.
        Bus();

        /// Destructor
        ~Bus();


        /// Returns `true` always.
        virtual bool isReadable();

        /// Returns `true` always
        virtual bool isWritable();

        /// Start range of the memory address space. Hard coded to 0x0000.
        virtual word addressStart();

        /// End range of the memoy address space. Hard coded to 0xFFFF.
        virtual word addressEnd();

        /// Read a byte from the specified memory address. If the address does not fall in the range of any connected
        /// device, the method reads 0x00.
        ///
        /// @param address the address from which to read
        /// @param data    reference to data byte variable to read into
        ///
        /// @returns `true` if address maps to a device and data read successfully
        virtual bool read(word address, byte &data);

        /// Write a byte to the specified memory address. If the address does not fall in the range of any connected
        /// device, the method does nothing.
        ///
        /// @param address the address to which to write
        /// @param data    the data byte to write
        ///
        /// @returns `true` if the address maps to a device and data was succesfully written to it.
        virtual bool write(word address, byte data);


        /// Attaches the given device to the bus.
        ///
        /// @param device the device to attach
        void attach(std::shared_ptr<Addressable> device);

    private:

        /// List of devices attached to the bus
        std::vector<std::shared_ptr<Addressable>> _devices;
    };
}

#endif // __RT_6502_EMULATOR_BUS_HPP__