#include "helper.h"
#include "table.h"

#include <iostream>
#include <math.h>
#include <algorithm>


//==============================================================================
// Desc: by creation of a table object, this constructor creates a selection of
//  possible codes which can later be inserted in the code table. This code
//  is initializes with all existing samle binaries and the code NULL
// Arguments: size of table
// Returns: no
//==============================================================================
Table::Table(int size) : size(size){
    for(int i = 0; i < size; i++) {
        Binary* bin = new Binary(i, bitSize(size));
        table.insert(std::pair<Binary*, Node*> (bin, NULL));
        binaries.push_back(bin);
    }
}

//==============================================================================
// Desc: when deleting a table object, also delete all contents of the code table
// Arguments: kill me
// Returns: pls
//==============================================================================
Table::~Table() {
    for(auto &it : table)
        delete it.first;
}

//==============================================================================
// Desc: Function to compare to Node variables in the std::sort method. Returns
//  true if a's weight is bigger then b's.
// Arguments: to nodes to compare
// Returns: if node a has a higher weight than b
//==============================================================================
bool compare(Node* a, Node* b) {
    //compare method for the std::sort
    return b->getWeight() < a->getWeight();
}

//==============================================================================
// Desc: Sorts nodes by their priority. Starts with priority one neighbours and
//  finishes with priority three neighbours. It also sets the weight for a
//  node (see node.cpp).
// Arguments: a vector of nodes to generate codes for
// Returns: void
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
            //check if all neighbours already have codes
            bool oneIsNull = false;
            for(Node* neighbour : anchor->getNeighbours(k)) {
                if(!neighbour->getNodeCode()) {
                    oneIsNull = true;
                    break;
                }

            }
            if(!oneIsNull)
               std::cout << "SKIPPED: all neighbours have codes!" << std::endl;

            //if node has no neighbours go to next anchor

            if(anchor->getNeighbours(k).empty() || !oneIsNull)
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
// Desc: Tries to set a new code for a node if it has none. It returns true if a
//  node either has a code or if the function was able to assign a new one.
//  max-i represents the anchor codes bit which the function should try to flip
//  and implement as a new code.
// Arguments: a node which is the master, a node which is the slave to set a code for
//  multiple ints to flip bits.
// Returns: a success boolean
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
// Desc: Checks if the table already contains a code. If it does contain it, the
//  entry at binaries.at(i) is not NULL
// Arguments: table index to check at
// Returns: boolean if there is sth. at this index
//==============================================================================
bool Table::inTable(int i) {
    if(table[binaries.at(i)])
        return true;
    return false;
}

//==============================================================================
// Desc: This abomination is a tool to find a code with the maximum hamming distance
//  to the set of existing codes. It works but is ugly as hell.
// Arguments: void
// Returns: a binary object which has the maximum ham dist. to the other codes
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
// Desc: return the list of sample binaries
// Arguments: void
// Returns: a vector of sample binaries
//==============================================================================
std::vector<Binary*> Table::getBinaries() {
    return binaries;
}

//==============================================================================
// Desc: return the sorted list of codes
// Arguments: void
// Returns: the map which contains both binaries and nodes
//==============================================================================
std::map<Binary*, Node*> Table::getTable() {
    return table;
}

//==============================================================================
// Desc: return the number of successfully assigned neighbours
// Arguments: the priority for which the number of successfull nodes should be shown
// Returns: the number of successfully coded nodes
//==============================================================================
int Table::getSuccess(int k) {
    return success[k];
}
