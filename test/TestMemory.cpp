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

static Memory *_ram;
static Memory *_rom;

TestSetUp({
    _ram = new Memory(true, 0x0000, 0x7FF);
    _rom = new Memory(false, 0x0000, 0x00FF);
});

TestTearDown({
    delete _ram;
    delete _rom;
});


// test cases ----------------------------------------------------------------------------------------------------------

TestCase(read_write, "Read Write", {

    // write data
    for (word addr = _ram->addressStart(); addr <= _ram->addressEnd(); addr++) {
        bool success = _ram->write(addr, (0xFF & addr));
        TestAssert(success == true, "Writing to address %#06X failed", addr);
    }

    // read data
    for (word addr = _ram->addressStart(); addr <= _ram->addressEnd(); addr++) {
        byte data;
        bool success = _ram->read(addr, data);
        TestAssert(success == true, "Reading from address 0x%04X failed", addr);
        TestAssert(data == (0xFF & addr), "Incorrect data read from address 0x%04X", addr);
    }
})

TestCase(address_range, "Address Range", {

    byte data;

    // test read & write within range
    TestAssert(_ram->write(0x0000, 0x00) == true, "Write within range should return `true`");
    TestAssert(_ram->write(0x07FF, 0x00) == true, "Write within range should return `true`");

    TestAssert(_ram->read (0x0000, data) == true, "Read within range should return `true`");
    TestAssert(_ram->read (0x07FF, data) == true, "Read within range should return `true`");

    // test read & write outside range
    TestAssert(_ram->write(0x0800, 0x00) == false, "Write outside range should return `false`");
    TestAssert(_ram->write(0xFFFF, 0x00) == false, "Write outside range should return `false`");

    TestAssert(_ram->read (0x0800, data) == false, "Read outside range should return `false`");
    TestAssert(_ram->read (0xFFFF, data) == false, "Read outside range should return `false`");
})

TestCase(rom_mode, "Rom Mode", {

    byte data;

    // test read
    TestAssert(_rom->read(0x0000, data) == true, "Read within range should return `true`");
    TestAssert(_rom->read(0x00FF, data) == true, "Read within range should return `true`");

    // test write
    TestAssert(_rom->write(0x0000, 0x00) == false, "Write within range should return `false`");
    TestAssert(_rom->write(0x00FF, 0x00) == false, "Write within range should return `false`");
})

TestCase(load, "Load", {

    // initialize data to load
    byte buffer[0xFF];
    for (word i = 0x00; i < 0xFF; i++) {
        buffer[i] = 0xFF - i;
    }

    // load data
    word startAddr = 0x0F;
    bool success = _ram->load(buffer, startAddr, 0xFF);
    TestAssert(success, "Load data failed");

    // verify written data
    for (word i = 0x00; i < 0xFF; i++) {
        byte data;
        word addr = startAddr + i;
        bool success = _ram->read(addr, data);
        TestAssert(success == true, "Reading from address 0x%04X failed", addr);
        TestAssert(data == buffer[i], "Incorrect data read from address 0x%04X", addr);
    }
})


// test suite ----------------------------------------------------------------------------------------------------------

TestSuite(TestMemory, {
    test_read_write();
    test_address_range();
    test_rom_mode();
    test_load();
});
