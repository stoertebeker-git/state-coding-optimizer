#include "node.h"
#include "helper.h"
#include "binary.h"
#include "table.h"

#include <ctime>
#include <iostream>
#include <math.h>

using namespace std;

int main(int argc, char** argv) {

    short input_bits = 2;
    int num_nodes  = 8;
    int probability_of_generation = 4;

    std::vector<Node*> testnodes;
    std::vector<Binary*> conditions;

    std::srand(std::time(0));

    for(int i = 0; i < num_nodes; i++)
        testnodes.push_back(new Node('a' + i, num_nodes));

    for(int i = 0; i < pow(2, input_bits); i++)
        conditions.push_back(new Binary(i, input_bits));

    generateRandomConnections(testnodes, conditions, probability_of_generation);

    printAutomate(testnodes);

    returnPriorityOne(testnodes, conditions);
    returnPriorityTwo(testnodes, conditions);
    returnPriorityThree(testnodes, conditions);
    writeFile(testnodes, conditions);
    generateOutput(testnodes);

    Table* table = new Table(pow(2,bitSize(num_nodes)));
    table->assignPriorityThree(testnodes);
    for(Node* &n : testnodes) {
        if(n->getNodeCode() != NULL)
            cout << n->getName() << ": " << printVec(n->getNodeCode()->returnAsBoolVec(), false) << endl;
    }

    // Clean up
    delete table;
    for(auto &n : testnodes)
        delete n;

    for(auto &n : conditions)
        delete n;

    return 0;
}

