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

class Module {
private:
    // for local branches and jumps
    std::map<int, std::string> module_index_to_name;
    std::map<std::string, int> module_name_to_dest;

public:
    Module(
            string_vec_t& prog, // string vec containing all of the 
            std::map<std::string, int>& global_name_to_dest,
            std::map<int, std::string>& global_index_to_name,
            std::vector<int>& inst_vec) {

        int s = prog.size();
        int current_prog_size = inst_vec.size();
        for(int i = 0; i < s;) {
            std::string& str = prog.at(i);
            std::cout << "Current token: " << str << std::endl;

            int result = Instruction::getInstBinary(str);
            switch(result) {
                case pushLiteral:
                    inst_vec.push_back(result);
                    inst_vec.push_back(std::stoi(prog.at(i+1)));
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
                        auto p = Instruction::splitByDot(prog.at(i+1));
                        if(p.first == "global") {
                            global_index_to_name[inst_vec.size()] = p.second;
                        }
                        else if(p.first == "local") {
                            module_index_to_name[inst_vec.size()] = p.second;
                        }
                        else {
                            throw std::runtime_error("call destination " 
                                    + prog.at(i+1) + " is neither 'global' or 'local'");
                        }
                        inst_vec.push_back(result);
                        inst_vec.push_back(-1); // place holder
                    }
                    i += 2;
                    break;
                case pushRegister:
                case popRegister:
                    inst_vec.push_back(result);
                    inst_vec.push_back(std::stoi(prog.at(i+1)));
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
                        inst_vec.push_back(-1); // place holder
                    }
                    //inst_vec.push_back(std::stoi(prog.at(i+1)));
                    i += 2; break;
                case ret:
                    inst_vec.push_back(result);
                    i++; break;
                case loads:
                case stores:
                    inst_vec.push_back(result);
                    inst_vec.push_back(std::stoi(prog.at(i+1)));
                    i += 2; break;
                case movr:
                    throw std::runtime_error("Module -> movr semantics not supported yet");
                    break;
                case halt:
                    inst_vec.push_back(result);
                    i++; break;
                case relative:
                case _goto:
                    inst_vec.push_back(result);
                    inst_vec.push_back(std::stoi(prog.at(i+1)));
                    i += 2; break;
                default:
                    // result == -1
                    // this is all keywords that are not instructions
                    if(str == "export") {
                        // place this label in the global table AND the 'local.' jump table
                        // if it already exists, error. otherwise, place it in the table
                        auto iter = global_name_to_dest.find(prog.at(i+1));
                        if(iter != global_name_to_dest.end()) {
                            // error, kill process
                            throw std::runtime_error("Entry [global." 
                                    + prog.at(i+1) + "] already exists in global jump table");
                        } else {
                            global_name_to_dest[prog.at(i+1)] = inst_vec.size();
                        }

                        iter = module_name_to_dest.find(prog.at(i+1));
                        if(iter != module_name_to_dest.end()) {
                            throw std::runtime_error("Entry [local."
                                    + prog.at(i+1) + "] already exists in module jump table");
                        } else {
                            module_name_to_dest[prog.at(i+1)] = inst_vec.size();
                        }
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
                        // place this label ONLY in the 'local.' jump table
                        auto iter = module_name_to_dest.find(prog.at(i+1));
                        if(iter != module_name_to_dest.end()) {
                            throw std::runtime_error("Entry [local."
                                    + prog.at(i+1) + "] already exists in module jump table");
                        } else {
                            module_name_to_dest[prog.at(i+1)] = inst_vec.size();
                        }
                        i += 2;
                    }
                    break;
            }
        }

        // evaluate local. jumps
        for(auto iter : module_index_to_name) {
            int jump_dest = module_name_to_dest.at(iter.second);
            inst_vec.at(iter.first + 1) = jump_dest;
        }

        std::cout << "Local jump table: \n";
        for(auto iter : module_name_to_dest) {
            std::cout << "  " << iter.first << " : " << iter.second << std::endl;
        }
    }
};
