#ifndef CONDITION_H
#define CONDITION_H

#include <vector>

#include "helper.h"
#include "node.h"

class Condition{
    public:
        Condition(int intvalue, short size_in_bits);
        ~Condition();

        std::vector<bool> returnAsBoolVec();

        int returnAsInt();

    private:
        int intvalue;
        short size_in_bits;

};

#endif // CONDITION_H

