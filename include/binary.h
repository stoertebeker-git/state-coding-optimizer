#ifndef BINARY_H
#define BINARY_H

#include <vector>

#include "helper.h"

class Node;

class Binary{
public:
    Binary(int intvalue, short size_in_bits);
    ~Binary();

    std::vector<bool> returnAsBoolVec();

    int returnInt();
    short getSize();

private:
    int intvalue;
    short size_in_bits;
};

#endif // BINARY_H

