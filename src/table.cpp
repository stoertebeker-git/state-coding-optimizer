#include "helper.h"
#include "table.h"

#include <iostream>
#include <math.h>
#include <algorithm>


Table::Table(int size) : size(size){
    for(int i = 0; i < size; i++) {
        //creating a table with possible binaries and one with possible binaries
        //and empty node pointers
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
    //compare method for the std::sort
    return b->getWeight() < a->getWeight();
}

void Table::assignPriorityOne(std::vector<Node*> nodes) {
    //code for anchor nodes
    int code = 0;

    for(Node* &node : nodes) {
        //set weights for all nodes to make them sortable
        //this is according to the number of nieghbours
        node->setWeight();
    }

    //sort the nodes list
    std::sort(nodes.begin(),nodes.end(),compare);

    for(Node* &node : nodes) {
        std::cout << "Weight for node " << node->getName()
                  << ": " << node->getWeight() << std::endl;
    }

    //run three times for all three priorities
    for(int k = 0; k < 3; k++) {
        std::cout << "===ASSIGN PRIORITY " << k+1 << " NEIGHBOURS===" << std::endl;
        for(Node* &anchor : nodes) {
            if(k == 0)
                //sort the neighbours list in each node by weight
                anchor->sortAllNeighbours();
            //if node has no neighbours go to next anchor
            if(anchor->getNeighbours(k).empty())
                continue;

            if(anchor->getNodeCode() == NULL) {
                //find code with maximum hamming distance for the new anchorcode
                code = findMaxHamDist()->returnInt();
                anchor->setNodeCode(binaries.at(code));
                table[binaries.at(code)] = anchor;
                std::cout << "SUCCESS! anchornode code " << printVec(binaries.at(code)->returnAsBoolVec(),true)
                          << " for node " << anchor->getName() << std::endl;
            } else {
                //if node already has a fixed code, set this one as anchor code
                code = anchor->getNodeCode()->returnInt();
                std::cout << "ALREADY HAS! anchornode code " << printVec(binaries.at(code)->returnAsBoolVec(),true)
                          << " for node " << anchor->getName() << std::endl;
            }

            //the maximum number of bits to be possibly flipped
            int max = anchor->getNodeCode()->getSize()-1;

            for(Node* node : anchor->getNeighbours(k)) {

                //counter for the bits which should be tried to flip
                int i = max;

                while(i >= 0 && !setCodes(anchor, node, i, max))
                    i--;
            }
        }
    }
    //for all nodes without a code, set codes
    for(Node* node : nodes) {
        if(node->getNodeCode() == NULL) {
            code = findMaxHamDist()->returnInt();
            node->setNodeCode(binaries.at(code));
            table[binaries.at(code)] = node;
        }
    }
}
bool Table::setCodes (Node* anchor , Node* node, int i, int max) {

    if(node->getNodeCode())
        return true;
    int int_code = anchor->getNodeCode()->returnInt();

    //xor the relevant bit in int_code
    int_code = int_code ^ (1 << (max-i));

    std::cout << "TRY: code " << printVec(binaries.at(int_code)->returnAsBoolVec(), true)
              << " for " << node->getName() << std::endl;

    if(!inTable(int_code)) {

        table[binaries.at(int_code)] = node;
        node->setNodeCode(binaries.at(int_code));

        std::cout << "SUCCESS! node code " << printVec(binaries.at(int_code)->returnAsBoolVec(),true)
                  << " for node " << node->getName() << std::endl;
        return true;
    } else {
        //return false if the while loop should "try" again
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
    //this is for finding the maximum hamming distance to
    //the set of existing codes
    std::pair<Binary*, int> code_with_max_distance(binaries.at(0),0);

    for(int i = 0; i < binaries.size(); i++) {

        int ham_distance_for_codeword = bitSize(binaries.size());
        for(auto &it : table) {
            //if the table does not contain a node pointer at the given
            //binary then go on else skip to next place in the table
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

std::vector<Binary*> Table::getBinaries() {
    return binaries;
}
std::map<Binary*, Node*> Table::getTable() {
    return table;
}
