#include <iostream>
#include "node.h"
#include <ctime>

//#define RAND_MAX 3

using namespace std;

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
               testnodes.at(y).newConnection(testnodes.at(std::rand()%4), conditionslist.at(i));
        }
    }

    for (int i = 0; i < testnodes.size(); i++) {
        cout << "Node " << testnodes.at(i).getName() << " created!" << endl;
        cout << "Node " << testnodes.at(i).getName() << " is "
             << (testnodes.at(i).isIsolated() ? "" : "not ") << "isolated"
             << endl;
    }

    for(int y = 0; y < testnodes.size(); y++) {
        for(int j = 0; j < testnodes.size(); j++) {
            std::vector<std::vector<bool>> targetNodeCon =
                    testnodes.at(y).getConditionsForNode(testnodes.at(j));
            cout << testnodes.at(y).getName() << "->" << testnodes.at(j).getName() << ":";
            for(int i = 0; i < targetNodeCon.size(); i++) {
                cout << targetNodeCon.at(i).at(0)
                     << targetNodeCon.at(i).at(1)
                     << ",";
            }
            cout << " ";

        }
        cout << endl << endl;
    }



	return 0;
}
