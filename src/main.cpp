#include "node.h"
#include "helper.h"
#include "binary.h"
#include "table.h"

#include <ctime>
#include <iostream>
#include <math.h>

using namespace std;

int main(int argc, char** argv) {

    short input_bits = 3;
    int num_nodes  = 10;
    int probability_of_generation = 3;

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

    for(auto &n : testnodes)
        delete n;

    for(auto &n : conditions)
        delete n;

    return 0;
}

