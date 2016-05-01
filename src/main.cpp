#include "node.h"
#include "helper.h"

#include <ctime>
#include <iostream>
#include <math.h>

using namespace std;

int main(int argc, char** argv) {

    int input_bits = 2;
    int num_nodes  = 4;

    std::vector<Node*> testnodes;
    std::srand(std::time(0));

    for(int i = 0; i < num_nodes; i++) {
        testnodes.push_back(new Node('a' + i));
    }

    std::vector<std::vector<bool>> conditionslist;

    for(int i = 0; i < pow(2, input_bits); i++) {
        std::vector<bool> bits;

        for(int j = input_bits - 1; j >= 0; j--) {
            bits.push_back((i >> j) & 1);
        }
        cout << endl;
        conditionslist.push_back(bits);
    }

    generateRandomConnections(testnodes, conditionslist);

    printAutomate(testnodes);

    returnPriorityOne(testnodes, conditionslist);
    returnPriorityTwo(testnodes, conditionslist);
    returnPriorityThree(testnodes, conditionslist);
    writeFile(testnodes, conditionslist);

    for(auto &n : testnodes) {
        cout << endl << "Found Patterns for Node: " << n->getName()   << endl
             << "Prio 1: " << printVec(n->getFirstNeighbours(),true)  << endl
             << "Prio 2: " << printVec(n->getSecondNeighbours(),true) << endl
             << "===================================================" << endl;
    }

    for(auto &n : testnodes) {
        delete n;
    }
    return 0;
}

