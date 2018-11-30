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

    std::map<std::string, Module*> module_map;

    void sanitizeTextVector(std::vector<char>& c_vec);

public:
    Assembler(std::string start_file);
};

Assembler::Assembler(std::string start_file) {
    string_vec_t str_vec;
    {
        std::ifstream is(start_file);
        std::vector<char> buf((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
        this->sanitizeTextVector(buf);
        buf.push_back('\0');
        std::stringstream ss;
        ss << &buf[0]; 
        
        std::string tmp;
        while(ss >> tmp)
            str_vec.push_back(tmp);
    }

    
}

void Assembler::sanitizeTextVector(std::vector<char>& c_vec) {
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
                throw std::runtime_error("Assembler::sanitizeTextVector -> unknown state");
        }
    }

}
