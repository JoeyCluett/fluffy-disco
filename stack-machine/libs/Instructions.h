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

    // base instruction set
    const u_int8_t pushLiteral = 0;
    const u_int8_t push_1      = 1;
    const u_int8_t push_0      = 2;
    const u_int8_t add         = 3;
    const u_int8_t subtract    = 4;
    const u_int8_t multiply    = 5;
    const u_int8_t divide      = 6;
    const u_int8_t printTop    = 7;
    const u_int8_t call        = 8;
    const u_int8_t pushRegister = 9;
    const u_int8_t popRegister = 10;
    const u_int8_t branchZero  = 11; // branch if top of stack is zero
    const u_int8_t branchNZero = 12; // branch if top of stack is not zero
    const u_int8_t ret         = 13;
    const u_int8_t loads       = 14;
    const u_int8_t stores      = 15;
    const u_int8_t movr        = 16;
    const u_int8_t halt        = 17;
    const u_int8_t _goto       = 18;
    const u_int8_t stamp       = 19;

    // ISA extensions
    const u_int8_t decs    = 20;
    const u_int8_t incs    = 21;
    const u_int8_t decr    = 22;
    const u_int8_t incr    = 23;
    const u_int8_t addi    = 24;
    const u_int8_t subi    = 25;
    const u_int8_t popnone = 26;
    const u_int8_t pushret = 27;
    const u_int8_t bgtl    = 28;
    const u_int8_t bltl    = 29;
    const u_int8_t bgtz    = 30;
    const u_int8_t bltz    = 31;
    const u_int8_t bgtr    = 32;
    const u_int8_t bltr    = 33;

    const std::map<std::string, const u_int8_t> instruction_strings = {
        // standard ISA
        {"pushlit", pushLiteral}, {"push1", push_1}, {"push0", push_0},
        {"add", add}, {"sub", subtract}, {"mul", multiply}, {"div", divide},
        {"ptop", printTop}, {"call", call}, {"pushr", pushRegister}, {"popr", popRegister},
        {"bzero", branchZero}, {"bnzero", branchNZero}, {"ret", ret}, {"loads", loads},
        {"stores", stores}, {"movr", movr}, {"halt", halt}, {"goto", _goto},
        {"stamp", stamp},

        // ISA extensions
        {"decs", decs}, {"incs", incs}, {"decr", decr}, {"incr", incr}, {"popnone", popnone},
        {"pushret", pushret}, {"bgtl", bgtl}, {"bltl", bltl}, {"bgtz", bgtz}, {"bltz", bltz},
        {"bgtr", bgtr}, {"bltr", bltr},

        // optional ISA extension decorator
        {"x.decs", decs}, {"x.incs", incs}, {"x.decr", decr}, {"x.incr", incr}, {"x.popnone", popnone},
        {"x.pushret", pushret}, {"x.bgtl", bgtl}, {"x.bltl", bltl}, {"x.bgtz", bgtz}, {"x.bltz", bltz},
        {"x.bgtr", bgtr}, {"x.bltr", bltr}
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

    const char* printOpcode(u_int8_t opcode) {
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
                return "move register"; break;
            case halt:
                return "halt"; break;
            case _goto:
                return "goto"; break;
            case stamp:
                return "timestamp"; break;
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

    void dasm(std::vector<u_int8_t>& prog) {
        std::cout << "\n\nDisassembly:\n";

        auto pad_addr = [](int in, int sz) -> std::string {
            auto s = std::to_string(in);
            while(s.size() < sz)
                s = "0" + s;
            return s;
        };

        for(int i  = 0; i < prog.size();) {
            u_int8_t opcode = prog.at(i);
            std::cout << "[" << pad_addr(i, 5) << "]: (" << (int)opcode << ") ";
            switch(opcode) {
                case pushLiteral:
                    std::cout <<  "push literal " << *(int*)&prog.at(i+1) << std::endl; 
                    i += 5; break;
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
                    std::cout << "call " << *(int*)&prog.at(i+1) << std::endl; 
                    i += 5; break;
                case pushRegister:
                    std::cout << "push register " << (int)prog.at(i+1) << "\n"; 
                    i += 2; break;
                case popRegister:
                    std::cout << "pop register " << (int)prog.at(i+1) << "\n"; 
                    i += 2; break;
                case branchZero: // branch if top of stack is zero
                    std::cout <<  "branch if zero " << *(int*)&prog.at(i+1) << "\n"; 
                    i += 5; break;
                case branchNZero: // branch if top of stack is not zero
                    std::cout <<  "branch if not zero " << *(int*)&prog.at(i+1) << "\n"; 
                    i += 5; break;
                case ret:
                    std::cout << "return\n"; 
                    i++; break;
                case loads:
                    std::cout << "load stack " << *(int*)&prog.at(i+1) << "\n"; 
                    i += 5; break;
                case stores:
                    std::cout << "store stack " << *(int*)&prog.at(i+1) << "\n"; 
                    i += 5; break;
                case movr:
                    std::cout << "move register " << (int)prog.at(i+1) 
                        << ' ' << (int)prog.at(i+2) << "\n"; 
                    i += 3; break;
                case halt:
                    std::cout << "halt\n"; 
                    i++; break;
                case _goto:
                    std::cout << "goto global " << *(int*)&prog.at(i+1) << "\n"; 
                    i += 5; break;
                case stamp:
                    std::cout << "timestamp\n";
                    i++; break;

                // =====================================================
                // extended ISA
                // =====================================================

                case decs:
                    std::cout << "decrement stack\n";
                    i++; break;
                case incs:
                    std::cout << "increment stack\n";    
                    i++; break;
                case decr:
                    std::cout << "decrement register [" << (int)prog.at(i+1) << "]\n";
                    i += 2; break;
                case incr:
                    std::cout << "increment register [" << (int)prog.at(i+1) << "]\n";
                    i += 2; break;
                case addi:
                    std::cout << "add immediate '" << *(int*)&prog.at(i+1) << "' to stack\n";
                    i += 5; break;

                case subi:
                    std::cout << "subtract immediate '" << *(int*)&prog.at(i+1) << "' to stack\n";
                    i += 5; break;

                case popnone:
                    std::cout << "delete top of stack\n";
                    i++; break;

                case pushret:
                    std::cout << "push register [" << (int)prog.at(i+1) << "] and return\n";
                    i += 2; break;

                case bgtl:
                    std::cout << "branch if greater than '" << *(int*)&prog.at(i+1) << "'\n";
                    i += 5; break;
                
                case bltl:
                    std::cout << "branch if less than '" << *(int*)&prog.at(i+1) << "'\n";
                    i += 5; break;
                
                case bgtz:
                case bltz:
                    
                    i++; break;

                case bgtr:
                case bltr:
                    
                    i += 2; break;

                default:
                    throw std::runtime_error("DASM: unknown opcode"); break;
            }
        }
    }

} // end of namespace Instruction