#pragma once

#include <vector>
#include <iostream>

class Stack {
private:
    std::vector<int> work_stack;

public:

    friend std::ostream& operator<<(std::ostream& os, Stack& s) {
        for(int i : s.work_stack)
            os << i << ' ';
        os << std::endl;
        return os;
    }

    int getTop(void) {
        return this->work_stack.back();
    }

    void pushLiteral(int operand) {
        this->work_stack.push_back(operand);
    }

    void popTop(void) {
        this->work_stack.pop_back();
    }

    void add(void) {
        auto iter = this->work_stack.end();
        *(iter-2) += *(iter-1);
        this->popTop();
    }

    void subtract(void) {
        auto iter = this->work_stack.end();
        *(iter-2) -= *(iter-1);
        this->popTop();
    }

    void multiply(void) {
        auto iter = this->work_stack.end();
        *(iter-2) *= *(iter-1);
        this->popTop();
    }

    void divide(void) {
        auto iter = this->work_stack.end();
        *(iter-2) /= *(iter-1);
        this->popTop();
    }

};