#pragma once

namespace Instruction {

    const int pushLiteral = 0;
    const int push_1      = 1;
    const int push_0      = 2;
    const int add         = 3;
    const int subtract    = 4;
    const int multiply    = 5;
    const int divide      = 6;
    const int printTop    = 7;
    const int call        = 8;
    const int pushRegister = 9;
    const int popRegister = 10;
    const int branchZero  = 11; // branch if top of stack is zero
    const int branchNZero = 12; // branch if top of stack is not zero

    bool isInstruction(int op) {
        switch(op) {
            case pushLiteral: case push_1: case push_0:
            case add: case subtract: case multiply:
            case divide: case printTop: case call:
            case pushRegister: case popRegister:
            case branchZero: case branchNZero:
                return true;
            default:
                return false;
        }
    }

} // end of namespace Instruction