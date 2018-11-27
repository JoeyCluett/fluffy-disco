#include <iostream>
#include <Instructions.h>
#include <Stack.h>
#include <Runtime.h>

using namespace std;
using namespace Instruction;

int main(int argc, char* argv[]) {

    std::vector<int> sample_program = {
        pushLiteral, 1,
        push_1,
        push_1,
        add,
        subtract,
    };

    Runtime rt;

    for(int i = 0; i < 5; i++) {
        rt.executeProg(sample_program, 1);
        cout << rt << endl;
    }

    return 0;
}