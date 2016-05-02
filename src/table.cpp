#include "table.h"

Table::Table(std::vector<Node *> nodes) {
}

Table::~Table() {

}

std::map<Node*, int> Table::getTable(){
   return binarytable;
}
