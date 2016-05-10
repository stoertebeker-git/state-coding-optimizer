#include <iostream>
#include "binary.h"

Binary::Binary(int intvalue, short size_in_bits) : intvalue(intvalue), size_in_bits(size_in_bits) {
    //std::cout << "Binary created: " << intvalue
    //          << " " << printVec(returnAsBoolVec(), true) << std::endl;
}

Binary::~Binary() {

}

std::vector<bool> Binary::returnAsBoolVec() {
    std::vector<bool> bits;

    for(int j = size_in_bits - 1; j >= 0; j--)
        bits.push_back((intvalue >> j) & 1);
    return bits;
}

short Binary::getSize() {
    return size_in_bits;
}
\
int Binary::returnInt() {
    return intvalue;
}
