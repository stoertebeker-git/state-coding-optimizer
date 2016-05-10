#include "table.h"
#include <iostream>
#include <math.h>
#include <algorithm>

Table::Table() {
}

Table::~Table() {

}

std::vector<Node*> anchors;

void Table::assignPriorityThree(std::vector<Node*> nodes) {
    int code = 0;

    for(Node* &anchor : nodes) {
        if(anchor->getNodeCode() != NULL)
            continue;
        else
            std::cout << "NO CODE: " << anchor->getName() << std::endl;
        if(anchor->getFirstNeighbours().size() != 0) {

            for(int i = 0; i <= table.size(); i++) {
                if(table.empty()) {
                    std::cout << "maxsize new code " << i << std::endl;
                    break;
                }
                code = searchFreeCode(anchor->getNumNodes());
            }



            std::cout << "setnodecode for " << anchor->getName()
                      << std::endl;
            anchor->setNodeCode(code);
            table.push_back(code);


            int max = anchor->getNodeCode()->returnSize()-1;

            for(Node* node : anchor->getFirstNeighbours()) {

                int i = max;

                while(i >= 0 && !setCodes(anchor, node, i, max)) {
                    i--;
                }
            }
        }
    }
}

bool Table::setCodes (Node* anchor , Node* node, int i, int max) {
    if(node->getNodeCode())
        return true;
    int int_code = anchor->getNodeCode()->returnInt();

    if(anchor->getNodeCode()->returnAsBoolVec().at(i) == 0)
        int_code += pow(2, max-i);
    else
        int_code -= pow(2, max-i);


    std::cout << "TRY: digit was sth so for node "
              << node->getName() << " code " << int_code
              << " set" << std::endl;

    if(!inTable(int_code)) {

        node->setNodeCode(int_code);
        table.push_back(int_code);
        std::cout << "SUCCEED!" << std::endl;
        return true;
    } else {
        return false;
    }

}

int Table::searchFreeCode(int range) {
    for(int i = 0; i < range; i++) {
        if(!inTable(i)) {
            std::cout << "returned " << i << std::endl;
            return i;
        }\
    }
}

bool Table::inTable(int i) {
    return (std::find(table.begin(), table.end(), i) != table.end());
}
