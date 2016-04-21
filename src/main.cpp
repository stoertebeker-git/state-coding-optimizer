#include <iostream>
#include "node.h"

using namespace std;

int main(int argc, char** argv) {
	
	Node testnode('a');
	Node testnode2('b');
	std::vector<bool> connectitit;
	connectitit.push_back(true);
	connectitit.push_back(false);
	testnode.newConnection(testnode2, connectitit);

	cout << "Node " << testnode.getName() << " created!" << endl;
	cout << "Node " << testnode2.getName() << " created!" << endl;

	cout << "Node " << testnode.getName() << " has the following connections" << endl;

	cout << testnode.getSpecificConnection(connectitit) << endl;
	return 0;
}