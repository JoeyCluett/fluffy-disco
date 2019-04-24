#pragma once

#include <string>
#include <vector>
#include <utility>
#include <map>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <sstream>

#include <Instructions.h>

typedef std::vector<std::string> string_vec_t;

using namespace Instruction;

// assumes little-endian format
static void push_literal_to_instruction_vec(std::vector<u_int8_t>& vec, int lit) {
    vec.push_back(lit & 255);
    vec.push_back((lit >> 8) & 255);
    vec.push_back((lit >> 16) & 255);
    vec.push_back((lit >> 24) & 255);
}

class Module {
private:
    // for local branches and jumps
    std::map<int, std::string> module_index_to_name; // location to jump from
    std::map<std::string, int> module_name_to_dest;  // location to jump to

    void place_module_jump_destination(std::string name, int dest) {
        auto iter = this->module_name_to_dest.find(name);
        if(iter != this->module_name_to_dest.end()) {
            throw std::runtime_error("'local." + name + "' already has an entry");
        } else {
            this->module_name_to_dest[name] = dest;
        }
    }

    void place_module_jump_source(int src, std::string dest) {
        this->module_index_to_name[src] = dest;
    }

    void place_global_jump_destination(
            std::map<std::string, int>& global_name_to_dest, 
            std::string name, int dest) {
        auto iter = global_name_to_dest.find(name);
        if(iter != global_name_to_dest.end()) {
            throw std::runtime_error("'global." + name + "' already has an entry");
        } else {
            global_name_to_dest[name] = dest;
        }
    }

    void place_global_jump_source(
            std::map<int, std::string>& global_index_to_name, 
            int src, std::string dest) {
        global_index_to_name[src] = dest;
    }

    auto clean_register_reference(std::string s) -> int {
        if(s[0] == 'r' || s[0] == '>') {
            s.erase(s.begin());
            return std::stoi(s);
        }
        else {
            // assume a properly formatted string
            return std::stoi(s);
        }
    }

    int clean_stack_reference(std::string s) {
        if(s[0] == 's' || s[0] == '$') {
            s.erase(s.begin());
            return stoi(s);
        }
        else {
            return stoi(s);
        }
    }

public:
    Module(
            string_vec_t& prog, // string vec containing all of the 
            std::map<std::string, int>& global_name_to_dest,
            std::map<int, std::string>& global_index_to_name,
            std::vector<u_int8_t>& inst_vec) {

        int s = prog.size();
        int current_prog_size = inst_vec.size();
        for(int i = 0; i < s; /* FSM advances index */) {
            std::string& str = prog.at(i);
            std::cout << "Current token: " << str << std::endl;

            int result = Instruction::getInstBinary(str);
            switch(result) {
                case pushLiteral:
                    inst_vec.push_back(result);
                    push_literal_to_instruction_vec(inst_vec, std::stoi(prog.at(i+1)));
                    //inst_vec.push_back(std::stoi(prog.at(i+1)));
                    i += 2; break;
                case push_1:
                case push_0:
                case add:
                case subtract:
                case multiply:
                case divide:
                case printTop:
                    inst_vec.push_back(result);
                    i++; break;
                case call:
                    {
                        inst_vec.push_back(result);
                        auto p = Instruction::splitByDot(prog.at(i+1));
                        if(p.first == "global") {
                            this->place_global_jump_source(
                                global_index_to_name, inst_vec.size(), p.second);
                        }
                        else if(p.first == "local") {
                            this->place_module_jump_source(inst_vec.size(), p.second);
                        }
                        else {
                            throw std::runtime_error("call destination " 
                                    + prog.at(i+1) + " is neither 'global' or 'local'");
                        }
                        push_literal_to_instruction_vec(inst_vec, -1);
                        //inst_vec.push_back(-1); // place holder
                    }
                    i += 2;
                    break;
                case pushRegister:
                case popRegister:
                    inst_vec.push_back(result);
                    //push_literal_to_instruction_vec(inst_vec, std::stoi(prog.at(i+1)));
                    inst_vec.push_back(clean_register_reference(prog.at(i+1)));
                    i += 2; break;
                case branchZero: // branch if top of stack is zero
                case branchNZero: // branch if top of stack is not zero
                    inst_vec.push_back(result);
                    {
                        auto p = Instruction::splitByDot(prog.at(i+1));
                        if(p.first == "global") {
                            global_index_to_name[inst_vec.size()] = p.second;
                        }
                        else if(p.first == "local") {
                            module_index_to_name[inst_vec.size()] = p.second;
                        }
                        else {
                            throw std::runtime_error("branch destination " 
                                    + prog.at(i+1) + " is neither 'global' or 'local'");
                        }
                        push_literal_to_instruction_vec(inst_vec, -1);
                        //inst_vec.push_back(-1); // place holder
                    }
                    i += 2; break;
                case ret:
                    inst_vec.push_back(result);
                    i++; break;
                case loads:
                case stores:
                    inst_vec.push_back(result);
                    //push_literal_to_instruction_vec(inst_vec, std::stoi(prog.at(i+1)));
                    inst_vec.push_back(clean_stack_reference(prog.at(i+1)));
                    i += 2; break;
                case movr:
                    inst_vec.push_back(result);
                    inst_vec.push_back(clean_register_reference(prog.at(i+1)));
                    inst_vec.push_back(clean_register_reference(prog.at(i+2)));
                    i += 3;
                    break;
                case halt:
                    inst_vec.push_back(result);
                    i++; break;
                case _goto:
                    inst_vec.push_back(result);
                    {
                        auto p = Instruction::splitByDot(prog.at(i+1));
                        if(p.first == "global") {
                            global_index_to_name[inst_vec.size()] = p.second;
                        }
                        else if(p.first == "local") {
                            module_index_to_name[inst_vec.size()] = p.second;
                        }
                        else {
                            throw std::runtime_error("branch destination " 
                                    + prog.at(i+1) + " is neither 'global' or 'local'");
                        }
                        push_literal_to_instruction_vec(inst_vec, -1);
                        //inst_vec.push_back(-1); // place holder
                    }
                    //inst_vec.push_back(std::stoi(prog.at(i+1)));
                    i += 2; break;
                case stamp:
                    inst_vec.push_back(result);
                    i++; break;

                // ======================================================
                // ISA extensions
                // ======================================================
                case decs:
                case incs:
                    inst_vec.push_back(result);
                    i++; break;

                case decr:
                case incr:
                    inst_vec.push_back(result);
                    inst_vec.push_back(clean_register_reference(prog.at(i+1)));
                    i += 2; break;

                case addi:
                case subi:
                    inst_vec.push_back(result);
                    push_literal_to_instruction_vec(inst_vec, stoi(prog.at(i+1)));
                    i += 2; break;

                case popnone:
                    inst_vec.push_back(result);
                    i++; break;

                case pushret:
                    inst_vec.push_back(result);
                    inst_vec.push_back(clean_register_reference(prog.at(i+1)));
                    i += 2; break;

                case bgtl:
                case bltl:
                    inst_vec.push_back(result);
                    push_literal_to_instruction_vec(inst_vec, stoi(prog.at(i+1)));
                    i += 2; break;
                
                case bgtz:
                case bltz:
                    inst_vec.push_back(result);
                    i++; break;

                case bgtr:
                case bltr:
                    inst_vec.push_back(result);
                    inst_vec.push_back(clean_register_reference(prog.at(i+1)));
                    i += 2; break;

                default:
                    // result == -1
                    // this is all keywords that are not opcodes
                    if(str == "export") {
                        // place this label in the global table AND the 'local.' jump table
                        // if it already exists, error. otherwise, place it in the table
                        std::string s = prog.at(i+1);
                        this->place_global_jump_destination(
                                global_name_to_dest, s, inst_vec.size());

                        this->place_module_jump_destination(s, inst_vec.size());

                        i += 2;
                    }
                    else if(str == "uses") {
                        // recursively instantiate another Module
                        // this will continue to build up the final binary
                        string_vec_t new_prog;
                        {
                            Instruction::tokenize(prog.at(i+1) + ".asm", new_prog);
                            for(auto& str : new_prog)
                                std::cout << str << ' ';
                            std::cout << std::endl;
                        }

                        Module new_module(new_prog, global_name_to_dest, global_index_to_name, inst_vec);
                        i += 2;
                    }
                    else if(str == "label") {
                        // place this label ONLY in the local jump table
                        this->place_module_jump_destination(prog.at(i+1), inst_vec.size());
                        i += 2;
                    }
                    else {
                        throw std::runtime_error("Module: During final assembly; Unknown instruction opcode: " + str);
                    }
                    break;
            }
        }

        // evaluate local. jumps
        for(auto iter : module_index_to_name) {
            int jump_dest = module_name_to_dest.at(iter.second);
            
            int* i_ptr = (int*)&inst_vec[iter.first];
            *i_ptr = jump_dest;
            //inst_vec.at(iter.first) = jump_dest;
        }

        /*std::cout << "Local jump table: \n";
        for(auto iter : module_name_to_dest) {
            std::cout << "  " << iter.first << " : " << iter.second << std::endl;
        }*/
    }
};
