//
//  main.cpp
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 27 Mar 2020.
//  Copyright (c) 2020 Rakesh Ayyaswami. All rights reserved.
//

#include <iostream>
#include "CPU.hpp"

using namespace std;
using namespace rt_6502_emulator;

int main() {
    CPU cpu;
    cpu.reset();
    cout << "Hello CPU" << endl;
}
