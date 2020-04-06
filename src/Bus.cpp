//
//  Bus.cpp
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 7 Apr 2020.
//  Copyright (c) 2020 Rakesh Ayyaswami. All rights reserved.
//

#include "Bus.hpp"

namespace rt_6502_emulator {


    // constructors & destructor ---------------------------------------------------------------------------------------

    Bus::Bus() {
        _devices = {};
    }

    Bus::~Bus() {}


    // attach ----------------------------------------------------------------------------------------------------------

    void Bus::attach(std::shared_ptr<Addressable> device) {
        _devices.push_back(device);
    }


    // accessors -------------------------------------------------------------------------------------------------------

    bool Bus::isReadable()   { return true; }
    bool Bus::isWritable()   { return true; }
    word Bus::addressStart() { return 0x0000; }
    word Bus::addressEnd()   { return 0xFFFF; }


    // read / write ----------------------------------------------------------------------------------------------------

    bool Bus::read(word address, byte &data) {
        for (std::size_t i = 0; i < _devices.size(); i++) {
            auto device = _devices[i];

            if (address < device->addressStart() || address > device->addressEnd()) {
                continue;
            }

            bool success = device->read(address, data);
            if (success) {
                return true;
            }
        }
        return false;
    }

    bool Bus::write(word address, byte data) {
        for (std::size_t i = 0; i < _devices.size(); i++) {
            auto device = _devices[i];

            if (address < device->addressStart() || address > device->addressEnd()) {
                continue;
            }

            bool success = device->write(address, data);
            if (success) {
                return true;
            }
        }
        return false;
    }
}