//
//  Memory.cpp
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 2 Apr 2020.
//  Copyright (c) 2020 Rakesh Ayyaswami. All rights reserved.
//

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "Memory.hpp"

namespace rt_6502_emulator {

    // TODO: Add assignment operator

    // constructors & destructor ---------------------------------------------------------------------------------------

    Memory::Memory(bool isWritable, word addressStart, word addressEnd) {
        assert(addressEnd > addressStart);

        // initialize
        _isWritable   = isWritable;
        _addressStart = addressStart;
        _addressEnd   = addressEnd;

        // allocate memory
        word size     = _addressEnd - _addressStart + 1;
        _contents     = (byte *)malloc(size);
        assert(_contents);
    }

    Memory::Memory(const Memory &orig) {

        // initialize
        _isWritable   = orig._isWritable;
        _addressStart = orig._addressStart;
        _addressEnd   = orig._addressEnd;

        // allocate memory
        word size     = _addressEnd - _addressStart + 1;
        _contents     = (byte *)malloc(size);
        assert(_contents);

        // copy contents
        memcpy(_contents, orig._contents, size);
    }

    Memory::~Memory() {
        free(_contents);
    }


    // accessors -------------------------------------------------------------------------------------------------------

    bool Memory::isReadable()   { return true; }
    bool Memory::isWritable()   { return _isWritable; }
    word Memory::addressStart() { return _addressStart; }
    word Memory::addressEnd()   { return _addressEnd; }

    bool Memory::read(word address, byte &data) {
        if (address >= _addressStart && address <= _addressEnd) {
            word offset = address - _addressStart;
            data = _contents[offset];
            return true;
        }
        return false;
    }

    bool Memory::write(word address, byte data) {
        if (address >= _addressStart && address <= _addressEnd) {
            word offset = address - _addressStart;
            _contents[offset] = data;
            return true;
        }
        return false;
    }
}
