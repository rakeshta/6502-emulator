//
//  TestMemory.cpp
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 04 Apr 2020.
//  Copyright (c) 2020 Rakesh Ayyaswami. All rights reserved.
//

#include <stdlib.h>
#include "TestMacros.hpp"
#include "../src/Memory.hpp"

using namespace rt_6502_emulator;


// setup & teardown ----------------------------------------------------------------------------------------------------

static Memory *_memory;

TestSetUp({
    _memory = new Memory(true, 0x0000, 0x7FF);
});

TestTearDown({
    delete _memory;
});


// test cases ----------------------------------------------------------------------------------------------------------

TestCase(read_write, "Read Write", {

    // write data
    for (word addr = _memory->addressStart(); addr <= _memory->addressEnd(); addr++) {
        bool success = _memory->write(addr, (0xFF & addr));
        TestAssert(success == true, "Writing to address %#06X failed", addr);
    }

    // read data
    for (word addr = _memory->addressStart(); addr <= _memory->addressEnd(); addr++) {
        byte data;
        bool success = _memory->read(addr, data);
        TestAssert(success == true, "Reading from address 0x%04X failed", addr);
        TestAssert(data == (0xFF & addr), "Incorrect data read from address 0x%04X", addr);
    }
})

TestCase(address_range, "Address Range", {

    byte data;

    // test read & write within range
    TestAssert(_memory->write(0x0000, 0x00) == true, "Write within range should return `true`");
    TestAssert(_memory->write(0x07FF, 0x00) == true, "Write within range should return `true`");

    TestAssert(_memory->read (0x0000, data) == true, "Read within range should return `true`");
    TestAssert(_memory->read (0x07FF, data) == true, "Read within range should return `true`");

    // test read & write outside range
    TestAssert(_memory->write(0x0800, 0x00) == false, "Write outside range should return `false`");
    TestAssert(_memory->write(0xFFFF, 0x00) == false, "Write outside range should return `false`");

    TestAssert(_memory->read (0x0800, data) == false, "Read outside range should return `false`");
    TestAssert(_memory->read (0xFFFF, data) == false, "Read outside range should return `false`");
})

TestCase(load, "Load", {

    // initialize data to load
    byte buffer[0xFF];
    for (word i = 0x00; i < 0xFF; i++) {
        buffer[i] = 0xFF - i;
    }

    // load data
    word startAddr = 0x0F;
    bool success = _memory->load(buffer, startAddr, 0xFF);
    TestAssert(success, "Load data failed");

    // verify written data
    for (word i = 0x00; i < 0xFF; i++) {
        byte data;
        word addr = startAddr + i;
        bool success = _memory->read(addr, data);
        TestAssert(success == true, "Reading from address 0x%04X failed", addr);
        TestAssert(data == buffer[i], "Incorrect data read from address 0x%04X", addr);
    }
})


// test suite ----------------------------------------------------------------------------------------------------------

TestSuite(TestMemory, {
    test_read_write();
    test_address_range();
    test_load();
});
