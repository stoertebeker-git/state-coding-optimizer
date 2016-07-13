#include <iostream>
#include "binary.h"

Binary::Binary(int intvalue, short size_in_bits) : intvalue(intvalue), size_in_bits(size_in_bits) {
    //std::cout << "Binary created: " << intvalue
    //          << " " << printVec(returnAsBoolVec(), true) << std::endl;
}

Binary::~Binary() {

}

//==============================================================================
// Desc: returns the value of a binary object as a bool vector
// Arguments: None
// Returns: the content of the binary as a vector of bools
//==============================================================================
std::vector<bool> Binary::returnAsBoolVec() {
    std::vector<bool> bits;

    for(int j = size_in_bits - 1; j >= 0; j--)
        bits.push_back((intvalue >> j) & 1);
    return bits;
}

//==============================================================================
// Desc: returns the size in bits of a binary object
// Arguments: none
// Returns: integer which represents the bitlength
//==============================================================================
short Binary::getSize() {
    return size_in_bits;
}
\
//==============================================================================
// Desc: returns the integer value of a binary object
// Arguments: none
// Returns: integer representing the decimal value of a binary
//==============================================================================
int Binary::returnInt() {
    return intvalue;
}
