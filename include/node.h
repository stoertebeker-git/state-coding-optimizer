#ifndef _NODE_H_
#define _NODE_H_

#include <map>
#include <vector>
#include <string>

class Binary;

class Node {
public:

    Node(std::string name);
    ~Node();

    void newConnection(Node* node, Binary* condition);
    Node* getSpecificConnection(Binary *condition);
    bool hasSpecificConnection(Binary *condition);

    std::vector<Binary*> getConditionsForNode(Node* node);
    std::map<Binary*, Binary*> getOutput();
    std::map<Binary *, Node *> &getAllConnections();

    void setOutputAt(Binary *output, Binary *conditions);
    Binary* getOutputAt(Binary* condition) const;

    void checkForOneStep() ;

    std::string getName() const;
    void setName(std::string name);

    Binary* getNodeCode() const;
    void setNodeCode(Binary *code);


    bool operator<(const Node& otherNode) const;

    std::vector<Node*>& getNeighbours(short sel);

    void addNeighbour(Node* node, short sel);

    void setWeight(short k);
    int getWeight();

    void sortNeighbours();
    static bool compareRev(Node* a, Node* b);

private:

    std::map<Binary*, Binary*> output;
    std::vector<Node*> neighbours [3];

    std::map<Binary*, Node*> connections;

    int weight;
    std::string name;

    Binary* node_code;
};

#endif // _NODE_H_
