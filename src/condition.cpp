#include <iostream>
#include "condition.h"

Condition::Condition(int intvalue, short size_in_bits) : intvalue(intvalue), size_in_bits(size_in_bits) {
    std::cout << "Condition created: " << intvalue
              << " " << printVec(returnAsBoolVec(), true) << std::endl;
}

Condition::~Condition() {

}

std::vector<bool> Condition::returnAsBoolVec() {
    std::vector<bool> bits;

    for(int j = size_in_bits - 1; j >= 0; j--)
        bits.push_back((intvalue >> j) & 1);
    return bits;
}

int Condition::returnAsInt() {
    return intvalue;
}
