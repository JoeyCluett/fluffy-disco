#include <iostream>
#include <Instructions.h>
#include <Stack.h>
#include <Runtime.h>
#include <Assembler.h>
#include <DynamicRecompiler.h>

#include <string.h>

using namespace std;
using namespace Instruction;

int main(int argc, char* argv[]) {

    if(argc != 3) {
        std::cout << "usage:\n" << argv[0] << " <filename> <'run'|'step'|'dasm'|'dynarec'>\n";
        return 1;
    }

    Assembler a(argv[1]);
    auto& prog = a.getBinary();   

    if(strcmp(argv[2], "run") == 0) {
        Runtime rt;
        try {
            rt.executeProg(prog);
        } catch(std::runtime_error& up) {
            cout << "\n\n\n== system state ==\n\n";
            cout << rt << endl;;
        }
    }
    else if(strcmp(argv[2], "step") == 0) {
        Runtime rt;
        while(true) {
            rt.executeProg(prog, 1);
            cout << rt << endl;
            getchar();
        }
    }
    else if(strcmp(argv[2], "dasm") == 0) {
        Instruction::dasm(prog);
    }
    else if(strcmp(argv[2], "dynarec") == 0) {
        DynamicRecompiler dr(a);
        int tmp_rf[8];
        //dr(tmp_rf);
        dr();
    }
    else {
        cout << "Invalid third option\n";
        return 1;
    }

    return 0;
}
