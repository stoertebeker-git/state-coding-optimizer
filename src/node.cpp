#include "node.h"
#include "helper.h"
#include "condition.h"

#include <iostream>
#include <algorithm>

Node::Node(const Node& other) {
    this->name = other.name;
    this->nodeCode = other.nodeCode;
    this->output = other.output;
    this->firstneighbours = other.firstneighbours;
    this->secondneighbours = other.secondneighbours;
    this->connections = other.connections;

    std::cout << "node " << name << " copied" << std::endl;
}

Node::Node(char name) : name(name) {
    //std::cout << "node " << name << " created" << std::endl;
}

Node::~Node() {
    //std::cout << "node " << name << " deleted" << std::endl;
}

void Node::setOutputAt(Condition* condition, std::vector<bool> outputs) {
    output.insert(std::pair<Condition*, std::vector<bool>>(condition, outputs));
}

std::vector<bool> Node::getOutputAt(Condition* condition) const {
    return output.at(condition);
}

std::vector<bool> Node::getAnyOutput() {
    for(auto const &pair : output) {
        return pair.second;
    }
}

std::vector<Condition*> Node::getConditionsForNode(Node* node) {
    std::vector<Condition*> matchingconditions;

    for(auto& iter : connections) {
        if(iter.second->getName() == node->getName()){
            matchingconditions.push_back(iter.first);
        }
    }
    return matchingconditions;
}

void Node::checkForOneStep() {
    for(auto const &pair : connections) {

        Condition* to_compare = pair.first;
        if(name == pair.second->getName())
            continue;

        for(auto const &pair2 : connections) {
            short differences = 0;
            Condition* compare = pair2.first;
            if(pair2.second->getName() == pair.second->getName())
                continue;
            if(pair2.second->getName() == name)
                continue;

            for(int k = 0; k < compare->returnSize(); k++) {
                if(compare->returnAsBoolVec().at(k) != to_compare->returnAsBoolVec().at(k)) {
                    differences++;
                }
            }
            if(differences == 1) {
                pair.second->addSecondNeighbour(pair2.second);
                pair2.second->addSecondNeighbour(pair.second);
            }
        }
    }
}

std::map<Condition*, std::vector<bool>> Node::getOutput() {
    return output;
}

char Node::getName() const {
    return name;
}

void Node::setName(char name) {
    name = name;
}

void Node::newConnection(Node* node, Condition* condition) {
    connections.insert(std::pair<Condition*,Node*>(condition,node));
}

Node* Node::getSpecificConnection(Condition* condition) {
    return connections.at(condition);
}

bool Node::hasSpecificConnection(Condition* condition) {
    return connections.count(condition) != 0;
}

std::map<Condition*, Node*>& Node::getAllConnections() {
    return connections;
}

int Node::getNodeCode() const {
    return nodeCode;
}

void Node::setNodeCode(int value) {
    nodeCode = value;
}

bool Node::isIsolated() {
    for(auto& iter : connections) {
        if(iter.second->getName() != name){
            return false;
        }
    }
    return true;
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

int Node::getConditionSize(bool select) {
    for(auto const &pair : output) {
        if(output.size() == 0)
            continue;
        if(select)
            return pair.first->returnSize();
        else
            return pair.second.size();
    }
}

void Node::addFirstNeighbour(Node* node) {
    //std::cout << "adding neighbour " << node->getName() << " to " << name << std::endl;
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

std::map<Condition *, Node *> &Node::getConnections() {
    return connections;
}

std::vector<Node *> &Node::getThirdNeighbours(){
    return thirdneighbours;
}
