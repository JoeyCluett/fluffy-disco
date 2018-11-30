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
    std::string module_name;
    std::vector<int> code_segment;

    std::map<int, std::string> module_index_to_name;
    std::map<std::string, int> module_name_to_dest;

    // if any label is 'export'ed, it goes here so it 
    // can be accessed in the global lookup table
    std::map<std::string, int> exported_routines;

    bool has_export = false;

public:
    Module(string_vec_t& prog) {
        int s = prog.size();
        for(int i = 0; i < s;) {
            std::string& str = prog.at(i);

            int result = Instruction::getInstBinary(str);
            switch(result) {
                case pushLiteral:
                case push_1:
                case push_0:
                case add:
                case subtract:
                case multiply:
                case divide:
                case printTop:
                case call:
                case pushRegister:
                case popRegister:
                case branchZero: // branch if top of stack is zero
                case branchNZero: // branch if top of stack is not zero
                case ret:
                case loads:
                case stores:
                case movr:
                case halt:
                case relative:
                default:
                    // result == -1

            }
        }
    }
};
