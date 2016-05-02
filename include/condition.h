#ifndef CONDITION_H
#define CONDITION_H

#include <vector>

#include "helper.h"

class Node;

class Condition{
    public:
        Condition(int intvalue, short size_in_bits);
        ~Condition();

        std::vector<bool> returnAsBoolVec();

        int returnInt();

        short returnSize();

    private:
        int intvalue;
        short size_in_bits;
//this is heavy work in progress
};

#endif // CONDITION_H

