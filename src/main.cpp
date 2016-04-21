#include <iostream>
#include "node.h"

using namespace std;

int main(int argc, char** argv) {

    std::vector<Node> testnodes;
    char nodes[] = {'a', 'b', 'c', 'd'};
    for (int i = 0; i < sizeof(nodes)/sizeof(nodes[0]); i++) {
        testnodes.push_back(Node(nodes[i]));
    }

    std::vector<bool> connectitit, connectut;
	connectitit.push_back(true);
	connectitit.push_back(false);

    connectut.push_back(false);
    connectut.push_back(true);
    testnodes.at(1).newConnection(testnodes.at(2), connectitit);
    testnodes.at(1).newConnection(testnodes.at(2), connectut);
    //testnode.newConnection();


    for (int i = 0; i < testnodes.size(); i++) {
        cout << "Node " << testnodes.at(i).getName() << " created!" << endl;
        cout << "Node " << testnodes.at(i).getName() << " is "
             << (testnodes.at(i).isIsolated() ? "" : "not ") << "isolated"
             << endl;

    }

    cout << "node b is connected to "
         << testnodes.at(1).getSpecificConnection(connectitit) << endl;
    std::vector<std::vector<bool>> targetNodeCon = testnodes.at(1).getConditionsForNode(testnodes.at(2));
    cout << targetNodeCon.size() << endl;
    cout << "conditions for b going to c are: ";



    for(int i = 0; i < targetNodeCon.size(); i++) {
        cout << targetNodeCon.at(i).at(0)
             << targetNodeCon.at(i).at(1);
    }
    cout << endl;




	return 0;
}
