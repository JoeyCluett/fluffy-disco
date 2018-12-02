#pragma once

#include "Instructions.h"
#include "Stack.h"

#include <initializer_list>
#include <vector>
#include <stdexcept>
#include <iostream>

using namespace Instruction;

#define NUM_REGISTERS 8

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

    void executeProg(std::vector<int>& prog) {
        while(true)
            this->executeProg(prog, 1000);
    }

    // assumes one instruction per cycle
    void executeProg(std::vector<int>& prog, int cycles) {
        #define PC programCounter

        for(int i = 0; i < cycles; i++) {
            int inst = prog[programCounter];

            //std::cout << "Program counter: " << programCounter << "\n";
            //std::cout << "  " << Instruction::printOpcode(inst) << "\n\n";

            switch(inst) {
                case pushLiteral:
                    this->stack.pushLiteral(prog[PC+1]);
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
                    std::cout << this->stack.getTop() 
                    << std::endl;
                    programCounter++;
                    break;
                case call:
                    this->stack.pushLiteral(programCounter + 2);
                    programCounter = prog[programCounter+1];
                    break;
                case pushRegister:
                    this->stack.pushLiteral(this->registers[prog[PC+1]]);
                    programCounter += 2;
                    break;
                case popRegister:
                    this->registers[prog[PC+1]] = this->stack.getTop();
                    this->stack.popTop();
                    programCounter += 2;
                    break;
                case branchZero:
                    if(!this->stack.getTop())
                        programCounter = prog[PC + 1];
                    else
                        programCounter += 2;
                    break;
                case branchNZero:
                    if(this->stack.getTop())
                        programCounter = prog[PC + 1];
                    else
                        programCounter += 2;
                    break;
                case ret:
                    programCounter = this->stack.getTop();
                    this->stack.popTop();
                    break;
                case loads:
                    this->registers[0] = this->stack.getIndex(prog[PC+1]);
                    programCounter += 2;
                    break;
                case stores:
                    this->stack.getIndex(prog[PC+1]) = this->registers[0];
                    programCounter += 2;
                    break;
                case movr:
                    throw std::runtime_error("'movr' semantics are not supported yet");
                    break;
                case halt:
                    //this->isHalted = true;
                    throw std::runtime_error("Runtime -> VM HALT");
                    return; break;
                case relative:
                    programCounter += prog[PC+1];
                    break;
                default:
                    throw std::runtime_error("Runtime -> unknown instruction: " + std::to_string(inst));
            }
        }
    }
};

// dont need to drag this into other parts of the program
#undef NUM_REGISTERS