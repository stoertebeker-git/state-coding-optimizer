#ifndef _NODE_H_
#define _NODE_H_

#include <map>
#include <vector>

class Node {
public:
	Node(char name);
	~Node();

	char getName();
	void setName(char name);
	void newConnection(Node node, std::vector<bool> conditions);

private:
	std::map<std::vector<bool>, Node> connections;
	char name;
};

#endif // _NODE_H_