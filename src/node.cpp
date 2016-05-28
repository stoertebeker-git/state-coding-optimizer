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
//  Set a new output binary at a specific input.
//==============================================================================
void Node::setOutputAt(Binary* output_, Binary* condition) {
    output.insert(std::pair<Binary*, Binary*>(condition, output_));
}

//==============================================================================
//  return the output binary at a specific input
//==============================================================================
Binary* Node::getOutputAt(Binary* condition) const {
    return output.at(condition);
}

//==============================================================================
//  return all the inputs leading from this node to another one.
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
//  check if there a inputs which lead to multiple nodes and check if these
//  inputs are one flipped binary apart
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
//  get all inputs and the connected outputs
//==============================================================================
std::map<Binary*, Binary*> Node::getOutput() {
    return output;
}

//==============================================================================
//  get a nodes name
//==============================================================================
char Node::getName() const {
    return name;
}

//==============================================================================
//  create a new connection from this node to another under a certain
//  condition(input)
//==============================================================================
void Node::newConnection(Node* node, Binary* condition) {
    connections.insert(std::pair<Binary*,Node*>(condition,node));
}

//==============================================================================
//  return the node which this node connects to with given input.
//==============================================================================
Node* Node::getSpecificConnection(Binary* condition) {
    return connections.at(condition);
}

//==============================================================================
//  check if a node connects anywhere with given input
//==============================================================================
bool Node::hasSpecificConnection(Binary* condition) {
    return connections.count(condition) != 0;
}

//==============================================================================
//  return all connected nodes connectecting input
//==============================================================================
std::map<Binary*, Node*>& Node::getAllConnections() {
    return connections;
}

//==============================================================================
//  get the set node code. This exists mainly to make the file IO easier
//==============================================================================
Binary* Node::getNodeCode() const {
    return node_code;
}

//==============================================================================
//  set a new code for this node
//==============================================================================
void Node::setNodeCode(Binary* code) {
    node_code = code;
}

//==============================================================================
//  overloaded operator < to use between two nodes. It compares the name of this
//  node to the name of the other. It had to be implemented for a std:: function
//  and makes no sense, which is ok.
//==============================================================================
bool Node::operator<(const Node& otherNode) const {
    return this->name < otherNode.name;
}

//==============================================================================
//  add a node to the list of nodes which should get a neighbouring code to
//  this node. Only add, if the map does not already contain this node.
//==============================================================================
void Node::addNeighbour(Node* node, short sel) {
    if(std::find(neighbours[sel].begin(), neighbours[sel].end(), node) == neighbours[sel].end())
        neighbours[sel].push_back(node);
}

//==============================================================================
//  return the nodes to be coded with a single step. short select lets you
//  select the priority.
//==============================================================================
std::vector<Node*>& Node::getNeighbours(short sel) {
    return neighbours[sel];
}

//==============================================================================
//  return the number of initially created nodes, this is not useful for a
//  dynamic adding and deleting of nodes, but it is not required in this project
//  so num_nodes exists for convenience.
//==============================================================================
int Node::getNumNodes() {
    return num_nodes;
}

//==============================================================================
//  set an artificial weight for a node, based on the number of neighbours.
//  it helps sorting the nodes in the code assignment process.
//==============================================================================
void Node::setWeight(short k) {
    weight = 0;
    for(int i = 2; i >= k; i--)
        weight += neighbours[i].size()*std::pow(num_nodes, 2-i);
}

//==============================================================================
//  return the nodes weight
//==============================================================================
int Node::getWeight() {
    return weight;
}

//==============================================================================
//  compare function for the std::sort function.
//==============================================================================
bool Node::compareRev(Node* a, Node* b) {
    return b->getWeight() > a->getWeight();
}

//==============================================================================
//  sort all neighbours by weight
//==============================================================================
void Node::sortNeighbours() {
    for(int i = 0; i < 3; i ++)
        std::sort(neighbours[i].begin(), neighbours[i].end(), compareRev);
}
