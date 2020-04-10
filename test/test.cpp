//
//  test.cpp
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 01 Apr 2020.
//  Copyright (c) 2020 Rakesh Ayyaswami. All rights reserved.
//

#include "TestMacros.hpp"


int main() {
    RunTestSuite(TestMemory);
    RunTestSuite(TestBus);
    RunTestSuite(TestInstructions);
    return 0;
}
