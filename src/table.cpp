#include "table.h"
#include <iostream>
#include <math.h>
#include <algorithm>
#include <helper.h>

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

void Table::assignPriorityThree(std::vector<Node*> nodes) {

    for(Node* &node : nodes) {
        node->setWeight();
        std::cout << "Weight for node " << node->getName()
                  << ": " << node->getWeight() << std::endl;
    }
    int code = 0;

    for(Node* &anchor : nodes) {
        if(anchor->getNodeCode() != NULL)
            continue;
        else
            std::cout << "NO CODE: " << anchor->getName() << std::endl;
        if(anchor->getFirstNeighbours().size() != 0) {

            code = findMinHamDis()->returnInt();

            std::cout << "SUCCESS! anchornode code " << printVec(binaries.at(code)->returnAsBoolVec(),true)
                      << " for node " << anchor->getName() << std::endl;
            anchor->setNodeCode(binaries.at(code));
            table[binaries.at(code)] = anchor;



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


        table[binaries.at(int_code)] = node;
        node->setNodeCode(binaries.at(int_code));

        std::cout << "SUCCESS! node code " << printVec(binaries.at(int_code)->returnAsBoolVec(),true)
                  << " for node " << node->getName() << std::endl;
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
    auto search = table.find(binaries.at(i));
    if(table[binaries.at(i)] != NULL) {
        std::cout << "in table return true" << std::endl;
        return true;
    } else {
        return false;
    }
}

Binary* Table::findMinHamDis() {
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
                if(binaries.at(i)->returnAsBoolVec().at(j) == it.first->returnAsBoolVec().at(j)) {
                    /*std::cout << binaries.at(i)->returnAsBoolVec().at(j) << " equals "
                              << it.first->returnAsBoolVec().at(j) << " at Node "
                              << it.second->getName() << std::endl;
                    */
                    ham_distance--;
                } else {
                    /* std::cout << binaries.at(i)->returnAsBoolVec().at(j) << " does not equal "
                              << it.first->returnAsBoolVec().at(j) << " at Node "
                              << it.second->getName() << std::endl;
                    */
                }
            }
            if(ham_distance < ham_distance_for_codeword)
                ham_distance_for_codeword = ham_distance;
            //std::cout << "distance= " << ham_distance_for_codeword << "!!new trial code" << std::endl;

        }
        if(ham_distance_for_codeword > code_with_max_distance.second) {
            code_with_max_distance.second = ham_distance_for_codeword;
            code_with_max_distance.first  = binaries.at(i);
        }
        std::cout << "RESULT: " << printVec(code_with_max_distance.first->returnAsBoolVec(),true)
                  << " with distance " << code_with_max_distance.second << std::endl;
    }
    return code_with_max_distance.first;
}

