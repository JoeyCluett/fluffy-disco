#pragma once

#include "Instructions.h"
#include "Stack.h"

#include <initializer_list>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <functional>

// needed to get timestamps
#include <sys/time.h>

using namespace Instruction;

#define NUM_REGISTERS 8
#ifndef PC
#define PC programCounter
#endif // PC

class Runtime {
private:
    Stack stack;
    int registers[NUM_REGISTERS];
    int programCounter = 0;

    std::vector<u_int8_t>* tmp_prog = NULL;

    // dont judge, just want to ensure compiler creates an actual lookup table
    const std::vector<std::function<void(void)>> instruction_lut = {
        // [0] pushLiteral
        [&]() {
            this->stack.pushLiteral(*(int*)&(*this->tmp_prog)[PC+1]);
            this->programCounter += 5;
        },
        
        // [1] push_1
        [&]() {
            this->stack.pushLiteral(1);
            this->programCounter++;
        },

        // [2] push_0
        [&]() {
            this->stack.pushLiteral(0);
            this->programCounter++;
        },

        // [3] add
        [&]() {
            this->stack.add();
            this->programCounter++;
        },

        // [4] subtract
        [&]() {
            this->stack.subtract();
            this->programCounter++;
        },

        // [5] multiply
        [&]() {
            this->stack.multiply();
            this->programCounter++;
        },

        // [6] divide
        [&]() {
            this->stack.divide();
            this->programCounter++;
        },

        // [7] printTop
        [&]() {
            std::cout << this->stack.getTop() << std::endl;
            this->programCounter++;
        },

        // [8] call
        [&]() {
            this->stack.pushLiteral(programCounter + 5);
            this->programCounter = *(int*)&(*this->tmp_prog)[programCounter+1];
        },

        // [9] pushRegister
        [&]() {
            this->stack.pushLiteral(this->registers[(*this->tmp_prog)[PC+1]]);
            this->programCounter += 2;
        },

        // [10] popRegister
        [&]() {
            this->registers[(*this->tmp_prog)[PC+1]] = this->stack.getTop();
            this->stack.popTop();
            this->programCounter += 2;
        },

        // [11] branchZero
        [&]() {
            if(!this->stack.getTop())
                this->programCounter = *(int*)&(*this->tmp_prog)[PC + 1];
            else
                this->programCounter += 5;
        },

        // [12] branchNZero
        [&]() {
            if(this->stack.getTop())
                this->programCounter = *(int*)&(*this->tmp_prog)[PC + 1];
            else
                this->programCounter += 5;
        },

        // [13] ret
        [&]() {
            this->programCounter = this->stack.getTop();
            this->stack.popTop();
        }.

        // [14] loads
        [&]() {
            this->registers[0] = this->stack.getIndex(*(int*)&(*this->tmp_prog)[PC+1]);
            this->programCounter += 5;
        },

        // [15] stores
        [&]() {
            this->stack.getIndex(*(int*)&(*this->tmp_prog)[PC+1]) = this->registers[0];
            this->programCounter += 5; 
        },

        // [16] movr
        [&]() {
            this->registers[(*this->tmp_prog)[PC+1]] = this->registers[(*this->tmp_prog)[PC+2]];
            this->programCounter += 3;
        },

        // [17] halt
        [&]() {
            throw std::runtime_error("Runtime -> VM HALT");
        },

        // [18] _goto
        [&]() {
            this->programCounter = *(int*)&(*this->tmp_prog)[PC+1];
        },

        // [19] stamp
        [&]() {
            timeval tv;
            gettimeofday(&tv, NULL);
            __int64_t s = tv.tv_sec * 1000000L + tv.tv_usec;
            int tf = (s / 1000); // ms timestamp
            this->stack.pushLiteral(tf);
            this->programCounter++;
        }
    };

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

    void executeProg(std::vector<u_int8_t>& prog) {
        while(true)
            this->executeProg(prog, 10000);
    }

    // assumes one instruction per cycle
    void executeProg(std::vector<u_int8_t>& prog, int cycles) {

        for(int i = 0; i < cycles; i++) {
            int inst = prog[programCounter];

            //std::cout << "Program counter: " << programCounter << "\n";
            //std::cout << "  " << Instruction::printOpcode(inst) << "\n\n";

            // index into an explicitly created lookup table
            this->instruction_lut[inst]();

            /*switch(inst) {
                case pushLiteral:
                    this->stack.pushLiteral(*(int*)&prog[PC+1]);
                    programCounter += 5;
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
                    this->stack.pushLiteral(programCounter + 5);
                    programCounter = *(int*)&prog[programCounter+1];
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
                        programCounter = *(int*)&prog[PC + 1];
                    else
                        programCounter += 5;
                    break;
                case branchNZero:
                    if(this->stack.getTop())
                        programCounter = *(int*)&prog[PC + 1];
                    else
                        programCounter += 5;
                    break;
                case ret:
                    programCounter = this->stack.getTop();
                    this->stack.popTop();
                    break;
                case loads:
                    this->registers[0] = this->stack.getIndex(*(int*)&prog[PC+1]);
                    programCounter += 5;
                    break;
                case stores:
                    this->stack.getIndex(*(int*)&prog[PC+1]) = this->registers[0];
                    programCounter += 5;
                    break;
                case movr:
                    this->registers[prog[PC+1]] = this->registers[prog[PC+2]];
                    programCounter += 3;
                    break;
                case halt:
                    throw std::runtime_error("Runtime -> VM HALT");
                    return; break;
                case _goto:
                    programCounter = *(int*)&prog[PC+1];
                    break;
                case stamp:
                    {
                        timeval tv;
                        gettimeofday(&tv, NULL);
                        __int64_t s = tv.tv_sec * 1000000L + tv.tv_usec;
                        int tf = (s / 1000); // ms timestamp
                        this->stack.pushLiteral(tf);
                    }
                    programCounter++;
                    break;
                default:
                    throw std::runtime_error("Runtime -> unknown instruction: " + std::to_string(inst));
            }*/
        }
    }
};

// dont need to drag this into other parts of the program
#undef NUM_REGISTERS