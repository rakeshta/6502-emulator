//
//  TestInstructions.cpp
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 10 Apr 2020.
//  Copyright (c) 2020 Pixxa LLC. All rights reserved.
//

#include "TestMacros.hpp"
#include "../src/CPU.hpp"
#include "../src/Bus.hpp"
#include "../src/Memory.hpp"

using namespace rt_6502_emulator;


// setup & teardown ----------------------------------------------------------------------------------------------------

static CPU *_cpu;

TestSetUp({
    _cpu = new CPU();
    _cpu->attach(std::make_shared<Memory>(true, 0x0000, 0xFFFF));
    _cpu->write(0xFFFC, 0x00);
    _cpu->write(0xFFFD, 0x00);
})

TestTearDown({
    delete _cpu;
})


// test cases ----------------------------------------------------------------------------------------------------------

TestCase(LDA, "LDA", {

    // LDA #$20
    _cpu->write(0x0000, 0xA9);
    _cpu->write(0x0001, 0x20);


})


// test suite ----------------------------------------------------------------------------------------------------------

TestSuite(TestInstructions, {
    test_LDA();
});
