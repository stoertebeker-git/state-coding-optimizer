#include "node.h"
#include "helper.h"
#include "binary.h"

#include <iostream>
#include <algorithm>

Node::Node(char name, int num_nodes) : name(name), num_nodes(num_nodes) {}

Node::~Node() {}

void Node::setOutputAt(Binary* condition, std::vector<bool> outputs) {
    output.insert(std::pair<Binary*, std::vector<bool>>(condition, outputs));
}

std::vector<bool> Node::getOutputAt(Binary* condition) const {
    return output.at(condition);
}

std::vector<Binary*> Node::getConditionsForNode(Node* node) {
    std::vector<Binary*> matchingconditions;

    for(auto& iter : connections) {
        if(iter.second->getName() == node->getName())
            matchingconditions.push_back(iter.first);
    }
    return matchingconditions;
}

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
                pair.second->addSecondNeighbour(pair2.second);
                pair2.second->addSecondNeighbour(pair.second);
            }
        }
    }
}

std::map<Binary*, std::vector<bool>> Node::getOutput() {
    return output;
}

char Node::getName() const {
    return name;
}

void Node::newConnection(Node* node, Binary* condition) {
    connections.insert(std::pair<Binary*,Node*>(condition,node));
}

Node* Node::getSpecificConnection(Binary* condition) {
    return connections.at(condition);
}

bool Node::hasSpecificConnection(Binary* condition) {
    return connections.count(condition) != 0;
}

std::map<Binary*, Node*>& Node::getAllConnections() {
    return connections;
}

Binary* Node::getNodeCode() const {
    return node_code;
}

void Node::setNodeCode(Binary* code) {
    node_code = code;
}

bool Node::operator<(const Node& otherNode) const {
    return this->name < otherNode.name;
}

std::vector<Node*>& Node::getFirstNeighbours() {
    return firstneighbours;
}

std::vector<Node*>& Node::getSecondNeighbours() {
    return secondneighbours;
}

void Node::addFirstNeighbour(Node* node) {
    if(std::find(firstneighbours.begin(), firstneighbours.end(), node) == firstneighbours.end())
        firstneighbours.push_back(node);
}

void Node::addSecondNeighbour(Node* node) {
    if(std::find(secondneighbours.begin(), secondneighbours.end(), node) == secondneighbours.end())
        secondneighbours.push_back(node);
}

void Node::addThirdNeighbour(Node* node) {
    if(std::find(thirdneighbours.begin(), thirdneighbours.end(), node) == thirdneighbours.end())
        thirdneighbours.push_back(node);
}

std::map<Binary *, Node *> &Node::getConnections() {
    return connections;
}

std::vector<Node *> &Node::getThirdNeighbours(){
    return thirdneighbours;
}

int Node::getNumNodes() {
    return num_nodes;
}

void Node::setWeight() {
    weight = 0;
    for(auto &it : firstneighbours)
        weight+=num_nodes*num_nodes;

    for(auto &it : secondneighbours)
        weight+=num_nodes;

    for(auto &it : thirdneighbours)
        weight++;
}

int Node::getWeight() {
    return weight;
}

bool Node::compareRev(Node* a, Node* b) {
    return b->getWeight() > a->getWeight();
}

void Node::sortAllNeighbours() {
   std::sort(firstneighbours.begin(), firstneighbours.end(), compareRev);
   std::sort(secondneighbours.begin(), secondneighbours.end(), compareRev);
   std::sort(thirdneighbours.begin(), thirdneighbours.end(), compareRev);
}
