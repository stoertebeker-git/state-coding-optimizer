#include "table.h"
#include <iostream>
#include <math.h>
#include <algorithm>

Table::Table() {
}

Table::~Table() {

}

//std::map<Node*, int> Table::getTable(){
//   return binarytable;
//}

void Table::assignPriorityThree(std::vector<Node*> nodes) {
    for(Node* &anchor : nodes) {
        if(anchor->getNodeCode() != NULL)
            continue;
        else
            std::cout << "NO CODE: " << anchor->getName() << std::endl;
        if(anchor->getThirdNeighbours().size() != 0) {
            int code = 0;

            for(int i = 0; i <= table.size(); i++) {
                if(i == table.size()) {
                    code = i;
                    std::cout << "maxsize new code " << i << std::endl;
                    break;
                }
                if(i != table.at(i)) {
                    std::cout << "new code " << i << std::endl;
                    code = i;
                    break;
                }
            }



                std::cout << "setnodecode for " << anchor->getName() << std::endl;
                anchor->setNodeCode(code);
                table.push_back(code);


            int max = anchor->getNodeCode()->returnSize()-1;

            for(Node* node : anchor->getThirdNeighbours()) {
                //if(i >= anchor->getNodeCode()->returnSize())
                //    break;
                int i = max;

                while(i >= 0 && !setCodes(anchor, node, i, max)) {
                    i--;
                }
            }
        }
    }
}

bool Table::setCodes (Node* anchor , Node* node, int i, int max) {
    if(anchor->getNodeCode()->returnAsBoolVec().at(i) == 0) {
        int int_code = anchor->getNodeCode()->returnInt() + pow(2, max-i);
        std::cout << "TRY: digit was 0 so for node "
                  << node->getName() << " code " << int_code
                  << " set" << std::endl;
        if(std::find(table.begin(), table.end(), int_code) == table.end()) {
            node->setNodeCode(int_code);
            table.push_back(int_code);
            std::cout << "SUCCEED!" << std::endl;
            return true;
        } else {
            return false;
        }
    } else {
        int int_code = anchor->getNodeCode()->returnInt() - pow(2, max-i);
        std::cout << "TRY: digit was 1 so for node " << node->getName()
                  << " code " << int_code << " set" << std::endl;
        if(std::find(table.begin(), table.end(), int_code) == table.end()) {
            node->setNodeCode(int_code);
            table.push_back(int_code);
            std::cout << "SUCCEED!" << std::endl;
            return true;

        } else {
            return false;
        }
    }
}
