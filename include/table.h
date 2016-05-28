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

    void assignCodes(std::vector<Node*> nodes);
    int searchFreeCode(int range);
    bool inTable(int i);
    bool setCodes (Node* anchor , Node* node, int i, int max, int k);
    void calcFreeCodes();
    int getSuccess(int k);
    std::vector<Binary*> getBinaries();
    std::map<Binary*, Node*> getTable();

private:
    std::map<Binary*, Node*> table;
    std::vector<Binary*> binaries;
    int num_free_codes;
    int size;
    int success[3] = {0,0,0};
};

#endif // TABLE_H

