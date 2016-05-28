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

    void setOutputAt(Binary *output, Binary *conditions);
    Binary* getOutputAt(Binary* condition) const;

    void checkForOneStep() ;

    char getName() const;
    void setName(char name);

    Binary* getNodeCode() const;
    void setNodeCode(Binary *code);

    std::map<Binary *, Binary *> getOutput();

    bool operator<(const Node& otherNode) const;

    std::vector<Node*>& getNeighbours(short sel);

    void addNeighbour(Node* node, short sel);

    void setWeight(short k);
    int getWeight();

    int getNumNodes();

    void sortNeighbours();
    static bool compareRev(Node* a, Node* b);

private:

    std::map<Binary*, Binary*> output;
    std::vector<Node*> neighbours [3];

    std::map<Binary*, Node*> connections;

    int weight;
    int num_nodes;
    char name;

    Binary* node_code;
};

#endif // _NODE_H_
