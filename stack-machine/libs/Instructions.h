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

    // convenient way to determine if a given opcode is supported
    const int getInstBinary(std::string opcode) {
        auto iter = instruction_strings.find(opcode);
        if(iter != instruction_strings.end()) {
            return iter->second;
        } else {
            return -1;
        }
    }

const char* printOpcode(int opcode) {
        switch(opcode) {
            case pushLiteral:
                return "push literal"; break;
            case push_1:
                return "push one"; break;
            case push_0:
                return "push zero"; break;
            case add:
                return "add"; break;
            case subtract:
                return "subtract"; break;
            case multiply:
                return "multiply"; break;
            case divide:
                return "divide"; break;
            case printTop:
                return "print top"; break;
            case call:
                return "call"; break;
            case pushRegister:
                return "push register"; break;
            case popRegister:
                return "pop register"; break;
            case branchZero: // branch if top of stack is zero
                return "branch if zero"; break;
            case branchNZero: // branch if top of stack is not zero
                return "branch if not zero"; break;
            case ret:
                return "return"; break;
            case loads:
                return "load stack"; break;
            case stores:
                return "store stack"; break;
            case movr:
                return "move register [NOT SUPPORTED]"; break;
            case halt:
                return "halt"; break;
            case relative: // goto relative
                return "goto relative"; break;
            case _goto:
                return "goto global"; break;
            case callrel: // call relative
                return "call relative"; break;
            default:
                return "UNKNOWN OPCODE"; break;
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

    void dasm(std::vector<int>& prog) {
        std::cout << "\n\nDisassembly:\n";

        for(int i  = 0; i < prog.size();) {
            int opcode = prog.at(i);
            std::cout << "[" << i << "]: (" << opcode << ") ";
            switch(opcode) {
                case pushLiteral:
                    std::cout <<  "push literal " << prog.at(i+1) << std::endl; 
                    i += 2; break;
                case push_1:
                    std::cout << "push one\n"; 
                    i++; break;
                case push_0:
                    std::cout << "push zero\n"; 
                    i++; break;
                case add:
                    std::cout << "add\n"; 
                    i++; break;
                case subtract:
                    std::cout << "subtract\n"; 
                    i++; break;
                case multiply:
                    std::cout << "multiply\n"; 
                    i++; break;
                case divide:
                    std::cout << "divide\n"; 
                    i++; break;
                case printTop:
                    std::cout << "print top\n"; 
                    i++; break;
                case call:
                    std::cout << "call " << prog.at(i+1) << std::endl; 
                    i += 2; break;
                case pushRegister:
                    std::cout << "push register " << prog.at(i+1) << "\n"; 
                    i += 2; break;
                case popRegister:
                    std::cout << "pop register " << prog.at(i+1) << "\n"; 
                    i += 2; break;
                case branchZero: // branch if top of stack is zero
                    std::cout <<  "branch if zero " << prog.at(i+1) << "\n"; 
                    i += 2; break;
                case branchNZero: // branch if top of stack is not zero
                    std::cout <<  "branch if not zero " << prog.at(i+1) << "\n"; 
                    i += 2; break;
                case ret:
                    std::cout << "return\n"; 
                    i++; break;
                case loads:
                    std::cout << "load stack " << prog.at(i+1) << "\n"; 
                    i += 2; break;
                case stores:
                    std::cout << "store stack " << prog.at(i+1) << "\n"; 
                    i += 2; break;
                case movr:
                    std::cout << "move register [NOT SUPPORTED]\n"; 
                    i++; break;
                case halt:
                    std::cout << "halt\n"; 
                    i++; break;
                case relative: // goto relative
                    std::cout << "goto relative " << prog.at(i+1) << "\n"; 
                    i += 2; break;
                case _goto:
                    std::cout << "goto global " << prog.at(i+1) << "\n"; 
                    i += 2; break;
                case callrel: // call relative
                    std::cout << "call relative " << prog.at(i+1) << "\n"; 
                    i += 2; break;
                default:
                    throw std::runtime_error("DASM: unknown opcode"); break;
            }
        }
    }

} // end of namespace Instruction