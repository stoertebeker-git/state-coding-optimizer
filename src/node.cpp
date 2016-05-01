#include "node.h"
#include "helper.h"
#include <iostream>

Node::Node() : name('\0') {
    std::cout << "node without name created" << std::endl;
}

Node::Node(const Node& other) {
    this->name = other.name;
    this->nodeCode = other.nodeCode;
    this->output = other.output;
    this->firstneighbours = other.firstneighbours;
    this->secondneighbours = other.secondneighbours;
    this->connections = other.connections;

    //std::cout << "node " << name << " copied" << std::endl;
}

Node::Node(char name) : name(name) {
    std::cout << "node " << name << " created" << std::endl;
}

Node::~Node() {
    //std::cout << "node " << name << " deleted" << std::endl;
}

void Node::setOutputAt(std::vector<bool> condition, std::vector<bool> outputs) {
    output.insert(std::pair<std::vector<bool>, std::vector<bool>>(condition, outputs));
}

std::vector<bool> Node::getOutputAt(const std::vector<bool> &condition) const {
    return output.at(condition);
}

std::vector<bool> Node::getAnyOutput() {
    for(auto const &pair : output) {
        return pair.first;
    }
}

std::vector<std::vector<bool>> Node::getConditionsForNode(Node* node) {
    std::vector<std::vector<bool>> matchingconditions;

    for(auto& iter : connections) {
        if(iter.second->getName() == node->getName()){
            matchingconditions.push_back(iter.first);
        }
    }

    return matchingconditions;
}

void Node::checkForOneStep() {
    for(auto const &pair : connections) {

        std::vector<bool> to_compare = pair.first;
        if(name == pair.second->getName())
            continue;

        for(auto const &pair2 : connections) {
            short differences = 0;
            std::vector<bool> compare = pair2.first;
            if(pair2.second->getName() == pair.second->getName())
                continue;
            if(pair2.second->getName() == name)
                continue;

            for(int k = 0; k < compare.size(); k++) {
                if(compare.at(k) != to_compare.at(k)) {
                    differences++;
                }
            }
            if(differences == 1) {
                std::cout << "Priority two was found from " << name << " to " << pair.second->getName()
                          << " and " << pair2.second->getName() << std::endl;
            }
        }
    }
}

std::map<std::vector<bool>, std::vector<bool>> Node::getOutput() {
    return output;
}

char Node::getName() const{
    return name;
}

void Node::setName(char name) {
    name = name;
}

void Node::newConnection(Node* node, std::vector<bool> &condition) {
    connections.insert( std::pair<std::vector<bool>,Node*>(condition,node) );
}

Node* Node::getSpecificConnection(std::vector<bool> condition) {
    return connections.at(condition);
}

bool Node::hasSpecificConnection(std::vector<bool> condition) {
    return connections.count(condition) != 0;
}

std::map<std::vector<bool>, Node*>& Node::getAllConnections() {
    return connections;
}

int Node::getNodeCode() const
{
    return nodeCode;
}

void Node::setNodeCode(int value)
{
    nodeCode = value;
}

bool Node::isIsolated(){
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

std::vector<Node*>& Node::getSecondNeighbours()
{
    return secondneighbours;
}

int Node::getConditionSize(bool select) {
    for(auto const &pair : output) {
        if(output.size() == 0)
            continue;
        if(select)
            return pair.first.size();
        else
            return pair.second.size();
    }
}

void Node::addFirstNeighbour(Node* node) {
    std::cout << "adding neighbour " << node->getName() << " to " << name << std::endl;
    firstneighbours.push_back(node);
    std::cout << "neighbors of " << name << ": " << printVec(firstneighbours, true) << std::endl;
}

std::map<std::vector<bool>, Node*>& Node::getConnections() {
    return connections;
}
