#include <iostream>
#include "node.h"

using namespace std;

int main(int argc, char** argv) {
	
	Node testnode('a');

	cout << "Node " << testnode.getName() << " created!" << endl;
	return 0;
}