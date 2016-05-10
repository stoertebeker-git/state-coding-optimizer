#include "helper.h"
#include "table.h"

#include <iostream>
#include <math.h>
#include <algorithm>


Table::Table(int size) : size(size){
    for(int i = 0; i < size; i++) {
        Binary* bin = new Binary(i, bitSize(size));
        table.insert(std::pair<Binary*, Node*> (bin, NULL));
        binaries.push_back(bin);
    }
}

Table::~Table() {
    for(auto &it : table)
        delete it.first;
}

bool compare(Node* a, Node* b) {
    return b->getWeight() < a->getWeight();
}

void Table::assignPriorityThree(std::vector<Node*> nodes) {

    for(Node* &node : nodes) {
        node->setWeight();
    }
    std::sort(nodes.begin(),nodes.end(),compare);
    for(Node* &node : nodes) {
        std::cout << "Weight for node " << node->getName()
                  << ": " << node->getWeight() << std::endl;
    }
    int code = 0;

    for(Node* &anchor : nodes) {
        if(anchor->getFirstNeighbours().empty())
            continue;
        else
            std::cout << "NO CODE: " << anchor->getName() << std::endl;
        if(anchor->getFirstNeighbours().size() != 0) {
            if(anchor->getNodeCode() == NULL) {
                code = findMaxHamDist()->returnInt();
                anchor->setNodeCode(binaries.at(code));
                table[binaries.at(code)] = anchor;
                std::cout << "SUCCESS! anchornode code " << printVec(binaries.at(code)->returnAsBoolVec(),true)
                          << " for node " << anchor->getName() << std::endl;
            } else {
                code = anchor->getNodeCode()->returnInt();
                std::cout << "ALREADY HAS! anchornode code " << printVec(binaries.at(code)->returnAsBoolVec(),true)
                          << " for node " << anchor->getName() << std::endl;
            }


            int max = anchor->getNodeCode()->getSize()-1;

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

    std::cout << "TRY: code " << printVec(binaries.at(int_code)->returnAsBoolVec(), true)
              << " for " << node->getName() << std::endl;

    if(!inTable(int_code)) {

        table[binaries.at(int_code)] = node;
        node->setNodeCode(binaries.at(int_code));

        std::cout << "SUCCESS! node code " << printVec(binaries.at(int_code)->returnAsBoolVec(),true)
                  << " for node " << node->getName() << std::endl;
        return true;
    } else {
        return false;
    }

}

bool Table::inTable(int i) {
    if(table[binaries.at(i)] != NULL)
        return true;
    else
        return false;
}

Binary* Table::findMaxHamDist() {
    std::pair<Binary*, int> code_with_max_distance;
    code_with_max_distance.first = binaries.at(0);
    code_with_max_distance.second = 0;
    for(int i = 0; i < binaries.size(); i++) {

        int ham_distance_for_codeword = bitSize(binaries.size());
        for(auto &it : table) {
            if(it.second == NULL)
                continue;
            int ham_distance = bitSize(binaries.size());

            for(int j = 0; j < binaries.at(i)->returnAsBoolVec().size(); j++) {
                if(binaries.at(i)->returnAsBoolVec().at(j) == it.first->returnAsBoolVec().at(j))
                    ham_distance--;
            }

            if(ham_distance < ham_distance_for_codeword)
                ham_distance_for_codeword = ham_distance;
        }
        if(ham_distance_for_codeword > code_with_max_distance.second) {
            code_with_max_distance.second = ham_distance_for_codeword;
            code_with_max_distance.first  = binaries.at(i);
        }
    }
    return code_with_max_distance.first;
}
