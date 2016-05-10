#ifndef TABLE_H
#define TABLE_H

#include <map>
#include <node.h>
#include <binary.h>

class Table {
    public:
        Table(int size);
        ~Table();

       void assignPriorityThree(std::vector<Node*> nodes);
       int searchFreeCode(int range);
       Binary* findMaxHamDist();
       bool inTable(int i);
    private:
       std::map<Binary*, Node*> table;
       std::vector<Binary*> binaries;
       int size;
       bool setCodes (Node* anchor , Node* node, int i, int max);
};

#endif // TABLE_H

