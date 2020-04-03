//
//  TestMemory.cpp
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 04 Apr 2020.
//  Copyright (c) 2020 Rakesh Ayyaswami. All rights reserved.
//

#include "TestMacros.hpp"


TestCase(read_write, "Read Write", {
    TestAssert(true, "Write to memory");
})

TestCase(random_access, "Random Access", {
    TestAssert(false, "Write to memory");
})

TestSuite(TestMemory, {
    test_read_write();
    test_random_access();
});