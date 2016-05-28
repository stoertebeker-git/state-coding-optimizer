#include "helper.h"
#include "table.h"

#include <iostream>
#include <math.h>
#include <algorithm>


//==============================================================================
//  by creation of a table object, this constructor creates a selection of
//  possible codes which can later be inserted in the code table. This code
//  is initializes with all existing samle binaries and the code NULL
//==============================================================================
Table::Table(int size) : size(size){
    for(int i = 0; i < size; i++) {
        Binary* bin = new Binary(i, bitSize(size));
        table.insert(std::pair<Binary*, Node*> (bin, NULL));
        binaries.push_back(bin);
    }
}

//==============================================================================
//  when deleting a table object, also delete all contents of the code table
//==============================================================================
Table::~Table() {
    for(auto &it : table)
        delete it.first;
}

//==============================================================================
//  Function to compare to Node variables in the std::sort method. Returns
//  true if a's weight is bigger then b's.
//==============================================================================
bool compare(Node* a, Node* b) {
    //compare method for the std::sort
    return b->getWeight() < a->getWeight();
}
//--> BUG! If a anchor doesnt find a code for a neighbour and if this neighbours
// only neighbour is the anchor, the neighbour gets assigned a new code which
// blocks the code for later priorities but has no use.
//==============================================================================
//  Sorts nodes by their priority. Starts with priority one neighbours and
//  finishes with priority three neighbours. It also sets the weight for a
//  node (see node.cpp).
//==============================================================================
void Table::assignCodes(std::vector<Node*> nodes) {
    //code for anchor nodes
    int code = 0;

    //run three times for all three priorities
    for(int k = 0; k < 3; k++) {

        for(Node* &node : nodes) {
            //set weights for all nodes to make them sortable
            //this is according to the number of nieghbours
            node->setWeight(k);
        }

        //sort the nodes list
        std::sort(nodes.begin(),nodes.end(),compare);

        for(Node* &node : nodes) {
            std::cout << "Weight for node " << node->getName()
                      << ": " << node->getWeight() << std::endl;
        }

        std::cout << "===ASSIGN PRIORITY " << k+1 << " NEIGHBOURS===" << std::endl;
        for(Node* &anchor : nodes) {
            if(k == 0)
                //sort the neighbours list in each node by weight
                anchor->sortNeighbours();
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

                while(i >= 0 && !setCodes(anchor, node, i, max, k))
                    i--;
            }
        }
    }
    //for all nodes without a code, set codes
    for(Node* node : nodes) {
        if(!node->getNodeCode()) {
            code = findMaxHamDist()->returnInt();
            node->setNodeCode(binaries.at(code));
            table[binaries.at(code)] = node;
        }
    }
}

//==============================================================================
//  Tries to set a new code for a node if it has none. It returns true if a
//  node either has a code or if the function was able to assign a new one.
//  max-i represents the anchor codes bit which the function should try to flip
//  and implement as a new code.
//==============================================================================
bool Table::setCodes (Node* anchor , Node* node, int i, int max, int k) {

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
        success[k]++;
        return true;
    } else {
        //return false if the while loop should "try" again
        return false;
    }
}


//==============================================================================
//  Checks if the table already contains a code. If it does contain it, the
//  entry at binaries.at(i) is not NULL
//==============================================================================
bool Table::inTable(int i) {
    if(table[binaries.at(i)])
        return true;
    return false;
}

//==============================================================================
//  This abomination is a tool to find a code with the maximum hamming distance
//  to the set of existing codes. It works but is ugly as hell.
//==============================================================================
Binary* Table::findMaxHamDist() {
    //this is for finding the maximum hamming distance to
    //the set of existing codes
    std::pair<Binary*, int> code_with_max_distance(binaries.at(0),0);

    for(int i = 0; i < binaries.size(); i++) {

        int ham_distance_for_codeword = bitSize(binaries.size());
        for(auto &it : table) {
            //if the table does not contain a node pointer at the given
            //binary then go on else skip to next place in the table
            if(!it.second)
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

//==============================================================================
//  return the list of sample binaries
//==============================================================================
std::vector<Binary*> Table::getBinaries() {
    return binaries;
}

//==============================================================================
//  return the sortes list of codes
//==============================================================================
std::map<Binary*, Node*> Table::getTable() {
    return table;
}

//==============================================================================
//  return the number of successfully assigned neighbours
//==============================================================================
int Table::getSuccess(int k) {
    return success[k];
}
