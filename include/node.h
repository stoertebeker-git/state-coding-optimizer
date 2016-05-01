#ifndef _NODE_H_
#define _NODE_H_

#include <map>
#include <vector>

#include "condition.h"

class Node {
public:
    Node(const Node &other);
	Node(char name);
	~Node();

    void newConnection(Node* node, Condition* condition);
    Node* getSpecificConnection(std::vector<bool> condition);
    bool hasSpecificConnection(std::vector<bool> condition);
    bool isIsolated();

    void setOutputAt(Condition *conditions, std::vector<bool> outputs);
    std::vector<bool> getOutputAt(const std::vector<bool> &conditions) const;
    std::vector<bool> getAnyOutput();

    std::vector<std::vector<bool>> getConditionsForNode(Node* node);

    std::map<std::vector<bool>, Node*>& getAllConnections();

    void checkForOneStep() ;

    char getName() const;
    void setName(char name);

    int getNodeCode() const;
    void setNodeCode(int value);

    std::map<std::vector<bool>, std::vector<bool>> getOutput();

    int getConditionSize(bool select);

    bool operator<(const Node& otherNode) const;
    std::vector<Node*>& getFirstNeighbours();

    std::vector<Node*>& getSecondNeighbours();

    void addFirstNeighbour(Node* node);
    void addSecondNeighbour(Node* node);
    void addThirdNeighbour(Node* node);

    std::map<std::vector<bool>, Node*>& getConnections();

    std::vector<Node*>& getThirdNeighbours();

private:
    std::map<std::vector<bool>, std::vector<bool>> output;
    std::vector<Node*> firstneighbours;
    std::vector<Node*> secondneighbours;
    std::vector<Node*> thirdneighbours;
    std::map<std::vector<bool>, Node*> connections;

	char name;
    int nodeCode;
};

#endif // _NODE_H_
