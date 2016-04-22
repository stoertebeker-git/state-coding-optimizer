#include <iostream>
#include "node.h"
#include <ctime>

//#define RAND_MAX 3

using namespace std;

void returnPriorityOne (std::vector<Node> nodes,std::vector<std::vector<bool>> conditions) {

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

    //for(int i = 0;)
}

void returnPriorityTwo (std::vector<Node> nodes, std::vector<std::vector<bool>> conditions) {

}

void returnPriorityThree (std::vector<Node> nodes, std::vector<std::vector<bool>> conditions) {
    for(int i = 0; i < nodes.size(); i++) {
        for(int j = 0; j < conditions.size(); j++) {
            if(nodes.at(i).getOutputAt(conditions.at(j)) == conditions.at(j)) {
                cout << "Priority three was found" << endl;
            }
        }
    }
}

int main(int argc, char** argv) {

    std::vector<Node> testnodes;
    std::srand(std::time(0));

    char nodes[] = {'a', 'b', 'c', 'd'};

    for (int i = 0; i < sizeof(nodes)/sizeof(nodes[0]); i++) {
        testnodes.push_back(Node(nodes[i]));
    }

    std::vector<std::vector<bool>> conditionslist;
    for(int i = 0; i <= 3; i++) {

        std::vector<bool> cond;

        cond.push_back((i >> 1) & 1);
        cond.push_back(i & 1);

        conditionslist.push_back(cond);
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
                cout << targetNodeCon.at(i).at(0)
                     << targetNodeCon.at(i).at(1)
                     << "(" << testnodes.at(y).getOutputAt(targetNodeCon.at(i)).at(0)
                     << testnodes.at(y).getOutputAt(targetNodeCon.at(i)).at(1) << ")"
                     <<  " ,";
            }
            cout << " ";
        }
        cout << endl << endl;
    }

    returnPriorityOne(testnodes,conditionslist);

	return 0;
}


