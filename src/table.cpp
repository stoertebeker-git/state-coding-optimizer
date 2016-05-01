#include "table.h"

Table::Table() {
}

Table::~Table() {};

std::map<Node*, int> Table::getTable(){
    return binarytable;
}
