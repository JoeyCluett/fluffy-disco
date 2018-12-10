#pragma once

#include <iostream>

// this address is inserted into the executable
void print_top_callee(int i) {
    // 'i' needs to be in %rdi
    std::cout << "print_top_callee: " << i << std::endl;
}


