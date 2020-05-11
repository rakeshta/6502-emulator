//
//  TestBus.cpp
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 08 Apr 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

#include <memory>
#include "TestMacros.hpp"
#include "../src/Bus.hpp"
#include "../src/Memory.hpp"

using namespace rt_6502_emulator;


// setup & teardown ----------------------------------------------------------------------------------------------------

static Bus *_bus;

TestSetUp({
    _bus = new Bus();
    _bus->attach(std::make_shared<Memory>(true, 0x0000, 0x07FF));
});

TestTearDown({
    delete _bus;
});


// test cases ----------------------------------------------------------------------------------------------------------

TestCase(read_write, "Read Write", {

    // write data
    for (word addr = 0x0000; addr <= 0x07FF; addr++) {
        bool success = _bus->write(addr, (0xFF & addr));
        TestAssert(success == true, "Writing to address %#06X failed", addr);
    }

    // read data
    for (word addr = 0x0000; addr <= 0x07FF; addr++) {
        byte data;
        bool success = _bus->read(addr, data);
        TestAssert(success == true, "Reading from address 0x%04X failed", addr);
        TestAssert(data == (0xFF & addr), "Incorrect data read from address 0x%04X", addr);
    }
})

TestCase(address_range, "Address Range", {

    byte data;

    // test read & write within range
    TestAssert(_bus->write(0x0000, 0x00) == true, "Write within range should return `true`");
    TestAssert(_bus->write(0x07FF, 0x00) == true, "Write within range should return `true`");

    TestAssert(_bus->read (0x0000, data) == true, "Read within range should return `true`");
    TestAssert(_bus->read (0x07FF, data) == true, "Read within range should return `true`");

    // test read & write outside range
    TestAssert(_bus->write(0x0800, 0x00) == false, "Write outside range should return `false`");
    TestAssert(_bus->write(0xFFFF, 0x00) == false, "Write outside range should return `false`");

    TestAssert(_bus->read (0x0800, data) == false, "Read outside range should return `false`");
    TestAssert(_bus->read (0xFFFF, data) == false, "Read outside range should return `false`");
})


// test suite ----------------------------------------------------------------------------------------------------------

TestSuite(TestBus, {
    test_read_write();
});
