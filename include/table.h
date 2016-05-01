#ifndef TABLE_H
#define TABLE_H

#include <map>
#include "node.h"
class Table {
    public:
        Table();
        ~Table();
        std::map<Node*, int> getTable();
    private:
        std::map<Node*, int> binarytable;
};

#endif // TABLE_H

