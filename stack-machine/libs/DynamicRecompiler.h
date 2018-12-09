#pragma once

#include <vector>
#include <initializer_list>
#include <Assembler.h>
#include <Instructions.h>

using namespace Instruction;

typedef std::initializer_list<u_int8_t> INSTRUCTION;
namespace I {

    INSTRUCTION push_rax{50};
    INSTRUCTION push_rbx{53};
    INSTRUCTION push_rcx{51};
    INSTRUCTION push_rdx{52};

}

class DynamicRecompiler {
private:
    std::vector<u_int8_t> recompiled_code;
    void push_instruction(std::initializer_list<u_int8_t> inst) {
        for(auto i : inst)
            this->recompiled_code.push_back(i);
    }

public:
    DynamicRecompiler(Assembler& assembler);

};

DynamicRecompiler::DynamicRecompiler(Assembler& assembler) {
    auto& iVec = assembler.getBinary();

    for(auto i : iVec) {
        switch(i) {
            case 
        }
    }

}