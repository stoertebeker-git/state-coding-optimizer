#include "node.h"
#include "helper.h"
#include "binary.h"
#include "table.h"

#include <ctime>
#include <iostream>
#include <math.h>
#include <string>

using namespace std;

int main(int argc, char** argv) {

    int input_bits = 1, output_bits = 1,
            num_nodes  = 4, probability_of_generation = 5, seed = std::time(0);
    std::srand(seed);

    for(int i = 0; i < argc; i++) {
        string arg = string(argv[i]);
        if(arg == "-i") {
            input_bits = atoi(argv[i+1]);
        } else if (arg == "-n") {
            num_nodes = atoi(argv[i+1]);
        } else if (arg == "-o") {
            output_bits = atoi(argv[i+1]);
        }else if (arg == "-p") {
            probability_of_generation = atoi(argv[i+1]);
        } else if (arg == "--help") {
            cout << "===========================================================" << endl
                 << "              MEGA NODE CODE OPTIMIZER V" << std::rand()%10 << "." << std::rand()%1000 << endl
                 << "This software has mega copyright until 1984" << endl
                 << "This software is protected by law mainly on the moon" << endl
                 << "--help -> display this help!" << endl
                 << "-p     -> chance of nodeconnections from 0 to 10" << endl
                 << "-i     -> number of input bits" << endl
                 << "-n     -> number of nodes" << endl
                 << "-o     -> number of output bits"
                 << "--seed -> random seed same seed same generation" << endl
                 << "this shit was solely written by me :(" << endl
                 << "I also want to greet my mom and dad and thank my friends" << endl
                 << "Simon and guterwhine for supporting me in those dark times" << endl
                 << "==========================================================" << endl;


            return 0;
        } else if (arg == "--seed") {
            seed = atoi(argv[i+1]);
        }
    }

    std::srand(seed);
    std::vector<Node*> testnodes;
    std::vector<Binary*> conditions;
    std::vector<Binary*> outputs;

    for(int i = 0; i < num_nodes; i++)
        testnodes.push_back(new Node('a' + i, num_nodes));
    //this pow usage is stupid, bit shifts should be used. Make a function in helper.cpp
    for(int i = 0; i < pow(2, input_bits); i++)
        conditions.push_back(new Binary(i, input_bits));

    for(int i = 0; i < pow(2, output_bits); i++)
        outputs.push_back(new Binary(i, output_bits));

    generateRandomConnections(testnodes,
                              conditions,
                              outputs,
                              probability_of_generation);

    returnPriorityOne(testnodes, conditions);
    returnPriorityTwo(testnodes, conditions);
    returnPriorityThree(testnodes, conditions);

    writeFile(testnodes, conditions);
    generateOutput(testnodes);

    Table* table = new Table(pow(2,bitSize(num_nodes)));
    table->assignCodes(testnodes);
    for(Node* &n : testnodes) {
        if(n->getNodeCode() != NULL)
            cout << n->getName() << ": " << printVec(n->getNodeCode()->returnAsBoolVec(), false) << endl;
    }

    cout << "PRIO 1 NODES NEIGHBOURED = " << table->getSuccess(0) << endl
         << "PRIO 2 NODES NEIGHBOURED = " << table->getSuccess(1) << endl
         << "PRIO 3 NODES NEIGHBOURED = " << table->getSuccess(2) << endl;

    printSortedMLFile(table, conditions);
    printUnsortedMLFile(testnodes, conditions);

    // Clean up
    delete table;
    for(auto &n : testnodes)
        delete n;

    for(auto &n : conditions)
        delete n;

    return 0;
}

