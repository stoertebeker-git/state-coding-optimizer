#include <iostream>
#include "node.h"

using namespace std;

int main(int argc, char** argv) {

    std::vector<Node> testnodes;
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

    testnodes.at(1).newConnection(testnodes.at(2), conditionslist.at(1));
    testnodes.at(1).newConnection(testnodes.at(2), conditionslist.at(3));
    testnodes.at(1).newConnection(testnodes.at(2), conditionslist.at(0));
    testnodes.at(2).newConnection(testnodes.at(1), conditionslist.at(0));

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
