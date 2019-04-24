#pragma once

#include <vector>
#include <initializer_list>
#include <sys/mman.h> // mmap constants
#include <unistd.h> // sysconf()
#include <map>
#include <utility>

#include <Assembler.h>
#include <Instructions.h>
#include <dynarec-shortcuts.h>

using namespace Instruction;

typedef std::initializer_list<u_int8_t> INSTRUCTION;
namespace I {

    INSTRUCTION push_rax{0x50};
    INSTRUCTION push_rbx{0x53};
    INSTRUCTION push_rcx{0x51};
    INSTRUCTION push_rdx{0x52};

    const int RAX = 0;
    const int RBX = 1;
    const int RCX = 2;
    const int RDX = 3;

    const int ADD = 0;
    const int SUB = 1;
    const int MUL = 2;
    const int DIV = 3;

} // end of namespace I

class DynamicRecompiler {
private:
    std::vector<u_int8_t> recompiled_code;
    u_int8_t* finalized_exec_code = NULL;

    void new_instruction(std::initializer_list<u_int8_t> inst) {
        for(auto i : inst)
            this->recompiled_code.push_back(i);
    }

    void new_64_bit_constant(u_int64_t i64) {
        for(int i = 0; i < 64; i += 8)
            this->recompiled_code.push_back((i64 >> i) & 255);
    }

    void new_64_bit_constant(__int64_t i64) {
        this->new_64_bit_constant(*(u_int64_t*)&i64);
    }
    
    void new_32_bit_constant(u_int32_t i32) {
        for(int i = 0; i < 32; i += 8)
            this->recompiled_code.push_back((i32 >> i) & 255);
    }

    void new_32_bit_constant(__int32_t i32) {
        this->new_32_bit_constant(*(u_int32_t*)&i32);
    }

    void call_external_function(void* f) {
        // movq *addr*, %rax
        new_instruction({0x48, 0xB8}); new_64_bit_constant((u_int64_t)f);

        // callq %rax
        new_instruction({0xFF, 0xD0});
    }

    void finalize(void) {
        this->finalized_exec_code = (__uint8_t*)mmap(
            NULL, 
            sysconf(_SC_PAGESIZE), 
            PROT_WRITE | PROT_EXEC | PROT_READ,
            MAP_ANON | MAP_PRIVATE,
            -1,
            0);
        std::cout << "Final executable size: " << recompiled_code.size() << std::endl;
    }

public:
    DynamicRecompiler(Assembler& assembler);

    void operator()(int* rf); // pass the resgister file
    void operator()(void);
};

DynamicRecompiler::DynamicRecompiler(Assembler& assembler) {
    auto& prog = assembler.getBinary();
    int PC = 0;

    // during initial compilation phase, each module had to maintain 
    // a lookup table for each jump caller and callee then evaluate 
    // all of them at the end of the run. the dynarec must do the same 
    // but with integer values only

    // we're just gonna keep track of EVERY 'opcode -> executable' translation 
    // because, at this point, the original translation data is gone and I 
    // dont feel like carrying it over
    std::map<int, int> jump_destination_translation;

    // while it is possible to jump TO anywhere in the code, we are not 
    // jumping FROM every location in the code. therefore we dont as need 
    // as much storage for this part. the bytecode offset is used to index 
    // into the table above. external function calls dont need to go in here
    std::map<int, int> jump_source_map; // <executable source, bytecode offset>

    for(; PC < prog.size();) {
        // save all jump destinations as we iterate through
        //jump_destination_translation.at(PC) = recompiled_code.size();
        

        u_int8_t i = prog[PC];
        switch(i) {
            case pushLiteral:
                // mov *32-bit* %rdi
                this->new_instruction({0x48, 0xC7, 0xC7});
                {
                    int c = *(int*)&prog[PC+1];
                    //std::cout << "pushliteral: " << c << std::endl;
                    this->new_32_bit_constant(c);
                    new_instruction({0x57}); // pushq %rdi
                }
                PC += 5;
                break;
            case add:
                new_instruction({0x5B});       // popq %rbx
                new_instruction({0x58});       // popq %rax
                new_instruction({0x01, 0xD8}); // add %ebx, %eax
                new_instruction({0x50});       // pushq %rax (result of addition)
                PC++;
                break;
            case printTop:
                // copy top of stack into %rdi
                new_instruction({0x5F}); // popq %rdi
                new_instruction({0x57}); // pushq %rdi
                call_external_function((void*)print_top_callee);
                PC++;
                break;
            case branchNZero:
                new_instruction({0x48, 0x85, 0xC0}); // test %rax, %rax
                new_instruction({0x74, 7}); // jz *short*
                
                new_instruction({0xE9}); new_32_bit_constant((__int32_t)0); // jmp *rel 32-bit*

                jump_source_map[recompiled_code.size() - 4] = *(int*)&prog[PC+1];
                PC += 5;

                break;
            default:
                throw std::runtime_error("DynamicRecompiler -> unsupported instruction");
        }
    }

    // evaluate jumps
    for(auto p : jump_source_map) {
        int recompiled_index_source = p.first;
        int bytecode_index = p.second;
        int recompiled_index_dest = jump_destination_translation[bytecode_index];

        int jump_instruction_location = recompiled_index_source - 2;
        *(int*)&recompiled_code[recompiled_index_source] 
            = recompiled_index_dest - jump_instruction_location;
    }

    this->finalize();

    // copy the generated binary into the space given
    int s = recompiled_code.size();
    for(int i = 0; i < s; i++)
        finalized_exec_code[i] = recompiled_code[i];

    std::cout << "Number jump sources: " << jump_source_map.size() << std::endl;
}

// register file pointer is passed as only argument
void DynamicRecompiler::operator()(int* rf) {
    typedef void(*_M_called_function)(int*);
    auto callable = (_M_called_function)this->finalized_exec_code;
    callable(rf);
}

void DynamicRecompiler::operator()(void) {
    typedef void(*_M_called_function)(void);
    auto callable = (_M_called_function)this->finalized_exec_code;
    callable();
}
