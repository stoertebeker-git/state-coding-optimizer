#include "node.h"

Node::Node(char name) : name(name) {

}

Node::~Node() {

}

std::vector<std::vector<bool>> Node::getConditionsForNode(Node node) {
    std::vector<std::vector<bool>> matchingconditions;
    for(auto& iter : connections) {
        if(iter.second.getName() == node.getName()){
            matchingconditions.push_back(iter.first);
        }
    }
    return matchingconditions;
}

char Node::getName() {
    return name;
}

void Node::setName(char name) {
    name = name;
}

void Node::newConnection(Node node, std::vector<bool> conditions) {
	connections.insert( std::pair<std::vector<bool>,Node>(conditions,node) );
}

char Node::getSpecificConnection(std::vector<bool> condition) {
    return connections.at(condition).getName();
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
