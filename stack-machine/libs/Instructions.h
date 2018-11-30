#pragma once

#include <vector>
#include <stdexcept>
#include <map>

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
    const int halt        = 17;
    const int relative    = 18;

    const std::map<std::string, const int> instruction_strings = {
        {"pushlit", pushLiteral}, {"push1", push_1}, {"push0", push_0},
        {"add", add}, {"sub", subtract}, {"mul", multiply}, {"div", divide},
        {"ptop", printTop}, {"call", call}, {"pushr", pushRegister}, {"popr", popRegister},
        {"bzero", branchZero}, {"bnzero", branchNZero}, {"ret", ret}, {"loads", loads},
        {"stores", stores}, {"movr", movr}, {"halt", halt}, {"relative", relative}
    };

    // convenient way to determine if a given opcode is 
    const int getInstBinary(std::string opcode) {
        auto iter = instruction_strings.find(opcode);
        if(iter != instruction_strings.end()) {
            return iter->second;
        } else {
            return -1;
        }
    }

} // end of namespace Instruction