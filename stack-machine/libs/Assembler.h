#pragma once

#include <string>
#include <vector>
#include <utility>
#include <map>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <sstream>

class Assembler {
private:

    std::map<int, std::string> jump_loc;
    std::map<std::string, int> jump_name;

    void sanitizeTextVector(std::vector<char>& c_vec);

public:
    Assembler(std::string start_file);



};

Assembler::Assembler(std::string start_file) {
    std::ifstream is(start_file);
    std::vector<char> buf((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
    this->sanitizeTextVector(buf);

    for(char c : buf)
        std::cout << c;
    std::cout << std::endl;

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
