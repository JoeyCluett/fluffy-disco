#include <iostream>
#include <Instructions.h>
#include <Stack.h>
#include <Runtime.h>
#include <Assembler.h>

using namespace std;
using namespace Instruction;

int main(int argc, char* argv[]) {

    if(argc != 2) {
        std::cout << "Supply file name as argument\n";
        return 1;
    }

    Assembler a(argv[1]);    

    auto& prog = a.getBinary();

    Runtime rt;
    rt.executeProg(prog, 100);

    return 0;
}