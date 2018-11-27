#pragma once

#include "Instructions.h"
#include "Stack.h"

#include <initializer_list>
#include <vector>
#include <stdexcept>
#include <iostream>

using namespace Instruction;

#define NUM_REGISTERS 4

class Runtime {
private:
    Stack stack;
    int registers[NUM_REGISTERS];
    int programCounter = 0;

public:
    Runtime(void) {
        for(int i = 0; i < NUM_REGISTERS; i++) {
            this->registers[i] = 0;
        }
    }

    friend std::ostream& operator<<(std::ostream& os, Runtime& rt) {
        for(int i = 0; i < NUM_REGISTERS; i++)
            os << i << " : " << rt.registers[i] << std::endl;
        os << "stack: " << rt.stack;
        return os;
    }

    // assumes one instruction per cycle
    void executeProg(std::vector<int>& prog, int cycles) {
        for(int i = 0; i < cycles; i++) {
            int inst = prog[programCounter];

            switch(inst) {
                case pushLiteral:
                    this->stack.pushLiteral(prog[programCounter+1]);
                    programCounter += 2;
                    break;
                case push_1:
                    this->stack.pushLiteral(1);
                    programCounter++;
                    break; 
                case push_0:
                    this->stack.pushLiteral(0);
                    programCounter++;
                    break;
                case add:
                    this->stack.add();
                    programCounter++;
                    break;
                case subtract:
                    this->stack.subtract();
                    programCounter++;
                    break;
                case multiply:
                    this->stack.multiply();
                    programCounter++;
                    break;
                case divide:
                    this->stack.divide();
                    programCounter++;
                    break; 
                case printTop:
                    std::cout << this->stack.getTop();
                    programCounter++;
                    break;
                case call:
                    programCounter = prog[programCounter+1];
                    break;
                case pushRegister:
                    this->stack.pushLiteral(this->registers[prog[programCounter+1]]);
                    programCounter++;
                    break;
                default:
                    throw std::runtime_error("Runtime -> unknown instruction");
            }
        }
    }
};

// dont need to drag this into other parts of the program
#undef NUM_REGISTERS