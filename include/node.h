#ifndef _NODE_H_
#define _NODE_H_

#include <map>
#include <vector>

class Binary;

class Node {
public:

    Node(char name, int num_nodes);
    ~Node();

    void newConnection(Node* node, Binary* condition);
    Node* getSpecificConnection(Binary *condition);
    bool hasSpecificConnection(Binary *condition);

    std::vector<Binary*> getConditionsForNode(Node* node);
    std::map<Binary*, Node*>& getConnections();
    std::map<Binary *, Node *> &getAllConnections();

    void setOutputAt(Binary *conditions, std::vector<bool> outputs);
    std::vector<bool> getOutputAt(Binary* condition) const;

    void checkForOneStep() ;

    char getName() const;
    void setName(char name);

    Binary* getNodeCode() const;
    void setNodeCode(Binary *code);

    std::map<Binary*, std::vector<bool> > getOutput();

    bool operator<(const Node& otherNode) const;

    std::vector<Node*>& getNeighbours(short select);

    void addFirstNeighbour(Node* node);
    void addSecondNeighbour(Node* node);
    void addThirdNeighbour(Node* node);

    void setWeight();
    int getWeight();

    int getNumNodes();

    void sortAllNeighbours();
    static bool compareRev(Node* a, Node* b);

private:

    std::map<Binary*, std::vector<bool>> output;
    std::vector<Node*> firstneighbours;
    std::vector<Node*> secondneighbours;
    std::vector<Node*> thirdneighbours;
    std::map<Binary*, Node*> connections;

    int weight;
    int num_nodes;
    char name;

    Binary* node_code;
};

#endif // _NODE_H_
