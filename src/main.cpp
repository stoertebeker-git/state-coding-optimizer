#include "node.h"
#include "helper.h"

#include <ctime>
#include <iostream>
#include <math.h>

using namespace std;


void returnPriorityOne (std::vector<Node> &nodes,std::vector<std::vector<bool>> &conditions) {
    std::vector<std::map<Node, std::vector<Node>>> lists;

    for(int i = 0; i < conditions.size(); i++) {
        std::map<Node, std::vector<Node>> results;

        for(int j = 0; j < nodes.size(); j++) {
            if(!nodes.at(j).hasSpecificConnection(conditions.at(i)))
                continue;

            Node node = nodes.at(j).getSpecificConnection(conditions.at(i));

            if(results.count(node)) {
                results.at(node).push_back(nodes.at(j));
                cout << "Priority one was found" << endl;
            } else {
                std::vector<Node> resultnode;
                resultnode.push_back(nodes.at(j));
                results.insert( std::pair<Node, std::vector<Node>>(node, resultnode) );
            }
        }
        for(int j = 0; j < nodes.size(); j++) {
            //delete entries with only one connection
        }
        lists.push_back(results);
    }
}

void returnPriorityTwo (std::vector<Node> &nodes, std::vector<std::vector<bool>> &conditions) {
    for(int i = 0; i < nodes.size(); i++) {
        nodes.at(i).checkForOneStep();
    }
}

void returnPriorityThree (std::vector<Node> &nodes, std::vector<std::vector<bool>> &conditions) {
    for(int i = 0; i < nodes.size(); i++) {
        for(int j = 0; j < conditions.size(); j++) {
            if(!nodes.at(i).hasSpecificConnection(conditions.at(j)))
                continue;
            if(nodes.at(i).getOutputAt(conditions.at(j)) == conditions.at(j)) {
                cout << "Priority three was found" << endl;
            }
        }
    }
}

int main(int argc, char** argv) {

    int input_bits = 2;
    int num_nodes  = 4;

    std::vector<Node> testnodes;
    std::srand(std::time(0));

    for(int i = 0; i < num_nodes; i++) {
        testnodes.push_back(Node('a' + i));
    }

    std::vector<std::vector<bool>> conditionslist;

    for(int i = 0; i < pow(2, input_bits); i++) {
        std::vector<bool> bits;

        for(int j = input_bits - 1; j >= 0; j--) {
            bits.push_back((i >> j) & 1);
            cout << ((i >> j) & 1);
        }
        cout << endl;

        conditionslist.push_back(bits);
    }

    for(int y = 0; y < testnodes.size(); y++) {
            for(int i = 0; i < conditionslist.size(); i++) {
                if(std::rand() % 6 >= 2) {
                    testnodes.at(y).newConnection(testnodes.at(std::rand()%4), conditionslist.at(i));
                    std::vector<bool> outputGenerate;
                    for(int z = 0; z < conditionslist.at(i).size(); z++) {
                        outputGenerate.push_back(std::rand()%2);
                    }
                    testnodes.at(y).setOutputAt(conditionslist.at(i), outputGenerate);
                }
        }
    }

    for(int y = 0; y < testnodes.size(); y++) {
        for(int j = 0; j < testnodes.size(); j++) {
            std::vector<std::vector<bool>> targetNodeCon =
                    testnodes.at(y).getConditionsForNode(testnodes.at(j));
            cout << testnodes.at(y).getName() << "->" << testnodes.at(j).getName() << ":";
            for(int i = 0; i < targetNodeCon.size(); i++) {
                cout << printVec(targetNodeCon.at(i), false) << "("
                     << printVec(testnodes.at(y).getOutputAt(targetNodeCon.at(i)), false)
                     << ")" << ",";
            }
            cout << " ";
        }
        cout << endl << endl;
    }

    returnPriorityOne(testnodes, conditionslist);
    returnPriorityTwo(testnodes, conditionslist);
    returnPriorityThree(testnodes, conditionslist);
    writeFile(testnodes, conditionslist);
    writeMLFile(testnodes);
	return 0;
}

