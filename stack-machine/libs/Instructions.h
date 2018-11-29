#pragma once

#include <vector>
#include <stdexcept>

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
    const int ret         = 13;
    const int loads       = 14;
    const int stores      = 15;
    const int movr        = 16;

    const std::vector<const char*> instruction_strings = {
        "pushlit", "push1", "push0", "add", "sub", "mul", "div",
        "ptop", "call", "pushr", "popr", "bzero", "bnzero", "ret",
        "loads", "stores", "movr"
    };

    int isInstruction(std::string str) {
        for(int i = 0; i < instruction_strings.size(); i++) {
            if(str == instruction_strings.at(i))
                return i;
        }
        throw std::runtime_error("Unknown instruction token: " + str);
    }

    bool isInstruction(int op) {
        switch(op) {
            case pushLiteral: case push_1: case push_0:
            case add: case subtract: case multiply:
            case divide: case printTop: case call:
            case pushRegister: case popRegister:
            case branchZero: case branchNZero: case ret:
                return true;
            default:
                return false;
        }
    }

} // end of namespace Instruction