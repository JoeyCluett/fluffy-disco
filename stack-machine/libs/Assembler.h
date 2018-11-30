#pragma once

#include <string>
#include <vector>
#include <utility>
#include <map>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <sstream>

#include <Module.h> // other files contain exported methods
#include <Instructions.h>

class Assembler {
private:
    // track jump locations until ready for final assembly
    std::map<int, std::string> global_index_to_name;
    std::map<std::string, int> global_name_to_dest;
    std::vector<int> final_binary;

public:
    Assembler(std::string start_file);

    std::vector<int>& getBinary(void) {
        return this->final_binary;
    }

};

Assembler::Assembler(std::string start_file) {
    string_vec_t str_vec;
    Instruction::tokenize(start_file, str_vec);

    // get the (recursive) ball rolling
    std::cout << "Beginning recursive assembly: " << start_file << std::endl;
    Module first_module(
            str_vec, 
            global_name_to_dest, 
            global_index_to_name, 
            final_binary);

    // evaluate global jumps
    for(auto iter : global_index_to_name) {
        int jump_dest = global_name_to_dest.at(iter.second);
        final_binary.at(iter.first) = jump_dest;
    }

    std::cout << "Global jump table entries: \n";
    for(auto& iter : global_name_to_dest) {
        std::cout << "  " << iter.first << " : " << iter.second << std::endl;
    }

}