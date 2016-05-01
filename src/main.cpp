#include "node.h"
#include "helper.h"
#include "condition.h"
#include "table.h"

#include <ctime>
#include <iostream>
#include <math.h>

using namespace std;

int main(int argc, char** argv) {

    int input_bits = 3;
    int num_nodes  = 4;
    int probability_of_generation = 3;

    std::vector<Node*> testnodes;
    std::srand(std::time(0));

    for(int i = 0; i < num_nodes; i++)
        testnodes.push_back(new Node('a' + i));


    std::vector<Condition*> conditions;

    for(int i = 0; i < pow(2, input_bits); i++)
        conditions.push_back(new Condition(i, input_bits));


    generateRandomConnections(testnodes, conditions, probability_of_generation);

    //printAutomate(testnodes);

    //returnPriorityOne(testnodes, conditionslist);
    //returnPriorityTwo(testnodes, conditionslist);
    //returnPriorityThree(testnodes, conditionslist);
    //writeFile(testnodes, conditionslist);

    //generateOutput(testnodes);
    for(auto &n : testnodes)
        delete n;

    for(auto &n : conditions)
        delete n;

    return 0;
}

