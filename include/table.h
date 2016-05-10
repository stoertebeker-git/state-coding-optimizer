#ifndef TABLE_H
#define TABLE_H

#include <map>
#include <node.h>
#include <binary.h>

class Table {

public:
    Table(int size);
    ~Table();

    Binary* findMaxHamDist();

    void assignPriorityThree(std::vector<Node*> nodes);
    int searchFreeCode(int range);
    bool inTable(int i);
    bool setCodes (Node* anchor , Node* node, int i, int max);

    std::vector<Binary*> getBinaries();

private:
    std::map<Binary*, Node*> table;
    std::vector<Binary*> binaries;

    int size;
};

#endif // TABLE_H

