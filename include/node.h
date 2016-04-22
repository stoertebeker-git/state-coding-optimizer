#ifndef _NODE_H_
#define _NODE_H_

#include <map>
#include <vector>

class Node {
public:
	Node(char name);
	~Node();

	void newConnection(Node node, std::vector<bool> conditions);
	char getSpecificConnection(std::vector<bool> condition);
    bool hasSpecificConnection(std::vector<bool> condition);
    bool isIsolated();

    void setOutputAt(std::vector<bool> conditions, std::vector<bool> outputs);
    std::vector<bool> getOutputAt(std::vector<bool> conditions);
    std::vector<std::vector<bool>> getConditionsForNode(Node node);

    std::map<std::vector<bool>, Node> getAllConnections();

    char getName();
    void setName(char name);

    int getNodeCode() const;
    void setNodeCode(int value);


    bool operator<(const Node& otherNode) const;

private:
    std::map<std::vector<bool>, std::vector<bool>> output;
    std::map<std::vector<bool>, Node> connections;
	char name;
    int nodeCode;
};

#endif // _NODE_H_
