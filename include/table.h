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

    void assignPriorityOne(std::vector<Node*> nodes);
    int searchFreeCode(int range);
    bool inTable(int i);
    bool setCodes (Node* anchor , Node* node, int i, int max);
    void calcFreeCodes();
    std::vector<Binary*> getBinaries();
    std::map<Binary*, Node*> getTable();

private:
    std::map<Binary*, Node*> table;
    std::vector<Binary*> binaries;
    int num_free_codes;
    int size;
};

#endif // TABLE_H

