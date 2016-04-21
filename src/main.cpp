#include <iostream>
#include "node.h"

using namespace std;

int main(int argc, char** argv) {

    std::vector<Node> testnodes;
    char nodes[] = {'a', 'b', 'c', 'd'};
    for (int i = 0; i < sizeof(nodes)/sizeof(nodes[0]); i++) {
        testnodes.push_back(Node(nodes[i]));
    }

	std::vector<bool> connectitit;
	connectitit.push_back(true);
	connectitit.push_back(false);
    //testnode.newConnection(testnode2, connectitit);
    for (int i = 0; i < testnodes.size(); i++) {
        cout << "Node " << testnodes.at(i).getName() << " created!" << endl;
    }


    //testnode.newConnection();

    //cout << testnode.getSpecificConnection(connectitit) << endl;
	return 0;
}
