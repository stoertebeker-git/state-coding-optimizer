#include "node.h"
#include <iostream>

Node::Node(char name) : name(name) {

}

Node::~Node() {

}
void Node::setOutputAt(std::vector<bool> condition, std::vector<bool> outputs) {
    output.insert(std::pair<std::vector<bool>, std::vector<bool>>(condition, outputs));
}


std::vector<bool> Node::getOutputAt(std::vector<bool> &condition) {
    return output.at(condition);
}

std::vector<bool> Node::getAnyOutput() {
    for(auto const &pair : output) {
        return pair.first;
    }
}

std::vector<std::vector<bool>> Node::getConditionsForNode(Node &node) {
    std::vector<std::vector<bool>> matchingconditions;
    for(auto& iter : connections) {
        if(iter.second.getName() == node.getName()){
            matchingconditions.push_back(iter.first);
        }
    }
    return matchingconditions;
}



void Node::checkForOneStep() {
    for(auto const &pair : connections) {

        std::vector<bool> to_compare = pair.first;
        if(name == pair.second.getName())
            continue;

        for(auto const &pair2 : connections) {
            short differences = 0;
            std::vector<bool> compare = pair2.first;
            if(pair2.second.getName() == pair.second.getName())
                continue;
            if(pair2.second.getName() == name)
                continue;

            for(int k = 0; k < compare.size(); k++) {
                if(compare.at(k) != to_compare.at(k)) {
                    differences++;
                }
            }
            if(differences == 1) {
                std::cout << "Priority two was found from " << name << " to " << pair.second.getName()
                          << " and " << pair2.second.getName() << std::endl;
            }
        }
    }
}

char Node::getName() const{
    return name;
}

void Node::setName(char name) {
    name = name;
}

void Node::newConnection(Node node, std::vector<bool> condition) {
    connections.insert( std::pair<std::vector<bool>,Node>(condition,node) );
}

char Node::getSpecificConnection(std::vector<bool> condition) {
    return connections.at(condition).getName();
}

bool Node::hasSpecificConnection(std::vector<bool> condition) {
    return connections.count(condition) != 0;
}

std::map<std::vector<bool>, Node> Node::getAllConnections() {
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
        if(iter.second.getName() != name){
            return false;
        }
    }
    return true;
}

bool Node::operator<(const Node& otherNode) const {
   return this->name < otherNode.name;
}
