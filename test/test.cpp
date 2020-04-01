//
//  test.cpp
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 01 Apr 2020.
//  Copyright (c) 2020 Rakesh Ayyaswami. All rights reserved.
//

#include <iostream>
#include "../src/CPU.hpp"

using namespace std;
using namespace rt_6502_emulator;

int main() {

    CPU cpu;
    cpu.reset();

    cout << "Testing harness" << endl;
    cout << "Acc: " << cpu.getAccumulator() << endl;

    return 0;
}
