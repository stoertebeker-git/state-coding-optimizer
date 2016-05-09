#ifndef TABLE_H
#define TABLE_H

#include <map>
#include <node.h>
#include <binary.h>

class Table {
    public:
        Table();
        ~Table();
      // std::map<Node*, int> getTable();
       void assignPriorityThree(std::vector<Node*> nodes);
    private:
       //std::map<Node*, int> binarytable;
       std::vector<int> table;
       bool setCodes (Node* anchor , Node* node, int i, int max);
};

#endif // TABLE_H

