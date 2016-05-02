#ifndef _NODE_H_
#define _NODE_H_

#include <map>
#include <vector>

class Condition;

class Node {
public:
    Node(const Node &other);
	Node(char name);
	~Node();

    void newConnection(Node* node, Condition* condition);
    Node* getSpecificConnection(Condition *condition);
    bool hasSpecificConnection(Condition *condition);
    bool isIsolated();

    void setOutputAt(Condition *conditions, std::vector<bool> outputs);
    std::vector<bool> getOutputAt(Condition* condition) const;
    std::vector<bool> getAnyOutput();

    std::vector<Condition*> getConditionsForNode(Node* node);

    std::map<Condition *, Node *> &getAllConnections();

    void checkForOneStep() ;

    char getName() const;
    void setName(char name);

    int getNodeCode() const;
    void setNodeCode(int value);

    std::map<Condition *, std::vector<bool> > getOutput();

    int getConditionSize(bool select);

    bool operator<(const Node& otherNode) const;
    std::vector<Node*>& getFirstNeighbours();

    std::vector<Node*>& getSecondNeighbours();

    void addFirstNeighbour(Node* node);
    void addSecondNeighbour(Node* node);
    void addThirdNeighbour(Node* node);

    std::map<Condition*, Node*>& getConnections();

    std::vector<Node*>& getThirdNeighbours();

private:
    std::map<Condition*, std::vector<bool>> output;
    std::vector<Node*> firstneighbours;
    std::vector<Node*> secondneighbours;
    std::vector<Node*> thirdneighbours;
    std::map<Condition*, Node*> connections;

	char name;
    int nodeCode;
};

#endif // _NODE_H_
