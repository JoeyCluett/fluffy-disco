#pragma once

#include <string>
#include <vector>
#include <utility>
#include <map>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <sstream>

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
    const int relative    = 18; // goto relative
    const int _goto       = 19;
    const int callrel     = 20; // call relative

    const std::map<std::string, const int> instruction_strings = {
        {"pushlit", pushLiteral}, {"push1", push_1}, {"push0", push_0},
        {"add", add}, {"sub", subtract}, {"mul", multiply}, {"div", divide},
        {"ptop", printTop}, {"call", call}, {"pushr", pushRegister}, {"popr", popRegister},
        {"bzero", branchZero}, {"bnzero", branchNZero}, {"ret", ret}, {"loads", loads},
        {"stores", stores}, {"movr", movr}, {"halt", halt}, {"grel", relative},
        {"goto", _goto}, {"crel", callrel}
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

    void sanitizeTextVector(std::vector<char>& c_vec) {
        const int state_default = 0;
        const int state_comment = 1;
        int current_state = state_default;

        for(int i = 0; i < c_vec.size(); i++) {
            char& current_char = c_vec[i];

            switch(current_state) {
                case state_default:
                    if(current_char == '#') {
                        current_char = ' ';
                        current_state = state_comment;
                    }
                    break;
                case state_comment:
                    if(current_char == '\n') {
                        current_state = state_default;
                    } else {
                        current_char = ' ';
                    }
                    break;
                default:
                    throw std::runtime_error("Instruction::sanitizeTextVector -> unknown state");
            }
        }
    }

    void tokenize(std::string filename, std::vector<std::string>& token_list) {
        token_list.clear(); // just in case
        std::ifstream is(filename);
        std::vector<char> buf((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
        Instruction::sanitizeTextVector(buf);
        buf.push_back('\0');
        std::stringstream ss;
        ss << &buf[0]; 
        
        std::string tmp;
        while(ss >> tmp)
            token_list.push_back(tmp);

    }

    auto splitByDot(std::string str) -> std::pair<std::string, std::string> {
        std::string a, b;
        
        const int state_a = 0;
        const int state_b = 1;
        int current_state = state_a;

        for(char ch : str) {
            switch(current_state) {
                case state_a:
                    if(ch == '.') {
                        current_state = state_b;
                    } else {
                        a.push_back(ch);
                    }
                    break;
                case state_b:
                    b.push_back(ch);
                    break;
                default:
                    throw std::runtime_error("Instruction::splitByDot -> unknown state");
            }
        }

        return {a, b};
    }

} // end of namespace Instruction