#ifndef TABLE_H
#define TABLE_H

#include <map>
#include <node.h>
#include <binary.h>

class Table {
    public:
        Table(std::vector<Node*> nodes);
        ~Table();
       std::map<Node*, int> getTable();
    private:
       std::map<Node*, int> binarytable;
};

#endif // TABLE_H

