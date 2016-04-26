#include "node.h"

Node::Node(char name) : name(name) {

}

Node::~Node() {

}
void Node::setOutputAt(std::vector<bool> condition, std::vector<bool> outputs) {
    output.insert(std::pair<std::vector<bool>, std::vector<bool>>(condition, outputs));
}

std::vector<bool> Node::getOutputAt(std::vector<bool> condition) {
    return output.at(condition);
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

/*std::vector<std::vector<bool>> Node::checkforOneStep() {
    for(int i = 0; i < connections.size(); i++) {
        std::vector<bool> to_compare;
        to_compare = connections.at(1);
        for(int j = 0; j < connections.size(); j++) {
            short differences = 0;
            std::vector<bool> compare;
            compare = connections.at(j);
            for(int k = 0; k < compare.size(); k++) {
                if(compare.at(k) != to_compare.at(k)) {
                    differences++;
                }
            }
            if(differences == 1) {
                cout << "ofenfoneofneonfoenfonef" << endl;
            }
        }
    }
}*/
std::vector<std::vector<bool>> Node::checkforOneStep() {
    std::vector<bool> test;
    test.push_back(false);
    test.push_back(true);
    std::map<std::vector<bool>, Node>::const_iterator it;
    it = connections.find(test);
}


char Node::getName() {
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
