#include "condition.h"

Condition::Condition(int intvalue, short size_in_bits) {
    intvalue = intvalue;
    size_in_bits = size_in_bits;
}

Condition::~Condition() {

}

std::vector<bool> Condition::returnAsBoolVec() {
    std::vector<bool> boolvector;
    return boolvector;
}

int Condition::returnAsInt() {
    return intvalue;
}
