#include "node.h"
#include "helper.h"
#include "binary.h"

#include <iostream>
#include <algorithm>

Node::Node(char name, int num_nodes) : name(name), num_nodes(num_nodes) {
    std::vector<Node *> sample;
    for(int i = 0; i < 3; i ++)
        neighbours[i] = sample;

}

Node::~Node() {

}

//==============================================================================
// Desc: Set a new output binary at a specific input.
// Arguments: The output Binary to set, a conditon under which it should be generated
// Returns: void
//==============================================================================
void Node::setOutputAt(Binary* output_, Binary* condition) {
    output.insert(std::pair<Binary*, Binary*>(condition, output_));
}

//==============================================================================
// Desc: return the output binary at a specific input
// Arguments: a binary condition at which the output should be found
// Returns: A binary output at a certain condition
//==============================================================================
Binary* Node::getOutputAt(Binary* condition) const {
    return output.at(condition);
}

//==============================================================================
// Desc: return all the inputs leading from this node to another one.
// Arguments: A node to find the condition to connect to
// Returns: a vector of matching binary conditions
//==============================================================================
std::vector<Binary*> Node::getConditionsForNode(Node* node) {
    std::vector<Binary*> matchingconditions;

    for(auto& iter : connections) {
        if(iter.second->getName() == node->getName())
            matchingconditions.push_back(iter.first);
    }
    return matchingconditions;
}

//==============================================================================
// Desc: check if there a inputs which lead to multiple nodes and check if these
//  inputs are one flipped binary apart
// Arguments: void
// Returns: void
//==============================================================================
void Node::checkForOneStep() {
    for(auto const &pair : connections) {

        Binary* to_compare = pair.first;
        if(name == pair.second->getName())
            continue;

        for(auto const &pair2 : connections) {
            short differences = 0;
            Binary* compare = pair2.first;
            if(pair2.second->getName() == pair.second->getName())
                continue;
            if(pair2.second->getName() == name)
                continue;

            for(int k = 0; k < compare->getSize(); k++) {
                if(compare->returnAsBoolVec().at(k) != to_compare->returnAsBoolVec().at(k))
                    differences++;
            }
            if(differences == 1) {
                pair.second->addNeighbour(pair2.second, 1);
                pair2.second->addNeighbour(pair.second, 1);
            }
        }
    }
}

//==============================================================================
// Desc: get all inputs and the connected outputs
// Arguments: void
// Returns: return all outputs and the conditions under which they occur
//==============================================================================
std::map<Binary*, Binary*> Node::getOutput() {
    return output;
}

//==============================================================================
// Desc: get a nodes name
// Arguments: void
// Returns: name of the node
//==============================================================================
char Node::getName() const {
    return name;
}

//==============================================================================
// Desc: create a new connection from this node to another under a certain
//  condition(input)
// Arguments: a Node which should be connected to, the condition by which it should
// Returns: void
//==============================================================================
void Node::newConnection(Node* node, Binary* condition) {
    connections.insert(std::pair<Binary*,Node*>(condition,node));
}

//==============================================================================
// Desc: return the node which this node connects to with given input.
// Arguments: a binary condition
// Returns: the node to which this node connects under given condition
//==============================================================================
Node* Node::getSpecificConnection(Binary* condition) {
    return connections.at(condition);
}

//==============================================================================
// Desc: check if a node connects anywhere with given input
// Arguments: a binary condition
// Returns: a succes flag boolean
//==============================================================================
bool Node::hasSpecificConnection(Binary* condition) {
    return connections.count(condition) != 0;
}

//==============================================================================
// Desc: return all connected nodes connectecting input
// Arguments: void
// Returns: a map of all connections
//==============================================================================
std::map<Binary*, Node*>& Node::getAllConnections() {
    return connections;
}

//==============================================================================
// Desc: get the set node code. This exists mainly to make the file IO easier
// Arguments: void
// Returns: the binary code of this node
//==============================================================================
Binary* Node::getNodeCode() const {
    return node_code;
}

//==============================================================================
// Desc: set a new code for this node
// Arguments: binary code to set for a node
// Returns: void
//==============================================================================
void Node::setNodeCode(Binary* code) {
    node_code = code;
}

//==============================================================================
// Desc: overloaded operator < to use between two nodes. It compares the name of this
//  node to the name of the other. It had to be implemented for a std:: function
//  and makes no sense, which is ok.
// Arguments: A node to compare
// Returns: boolean
//==============================================================================
bool Node::operator<(const Node& otherNode) const {
    return this->name < otherNode.name;
}

//==============================================================================
// Desc: add a node to the list of nodes which should get a neighbouring code to
//  this node. Only add, if the map does not already contain this node.
// Arguments: a neighbouring node to add, a selector for the priority
// Returns: void
//==============================================================================
void Node::addNeighbour(Node* node, short sel) {
    if(std::find(neighbours[sel].begin(), neighbours[sel].end(), node) == neighbours[sel].end())
        neighbours[sel].push_back(node);
}

//==============================================================================
// Desc: return the nodes to be coded with a single step. short select lets you
//  select the priority.
// Arguments: selector for priority of neighbours
// Returns: selected neighbouring nodes
//==============================================================================
std::vector<Node*>& Node::getNeighbours(short sel) {
    return neighbours[sel];
}

//==============================================================================
// Desc: return the number of initially created nodes, this is not useful for a
//  dynamic adding and deleting of nodes, but it is not required in this project
//  so num_nodes exists for convenience.
// Arguments: void
// Returns: number of initially created nodes
//==============================================================================
int Node::getNumNodes() {
    return num_nodes;
}

//==============================================================================
// Desc: set an artificial weight for a node, based on the number of neighbours.
//  it helps sorting the nodes in the code assignment process.
// Arguments: the priority
// Returns: void
//==============================================================================
void Node::setWeight(short k) {
    weight = 0;
    for(int i = 2; i >= k; i--)
        weight += neighbours[i].size()*std::pow(num_nodes, 2-i);
}

//==============================================================================
// Desc: return the nodes weight
// Arguments: and endless void ;(
// Returns: the weight of a node
//==============================================================================
int Node::getWeight() {
    return weight;
}

//==============================================================================
// Desc: compare function for the std::sort function.
// Arguments: two nodes to compare
// Returns: which one has the bigger weight boolean
//==============================================================================
bool Node::compareRev(Node* a, Node* b) {
    return b->getWeight() > a->getWeight();
}

//==============================================================================
// Desc: sort all neighbours by weight
// Arguments: pls
// Returns: stop
//==============================================================================
void Node::sortNeighbours() {
    for(int i = 0; i < 3; i ++)
        std::sort(neighbours[i].begin(), neighbours[i].end(), compareRev);
}
