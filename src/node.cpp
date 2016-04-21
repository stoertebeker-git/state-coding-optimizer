#include "node.h"

Node::Node(char name) : name(name) {

}

Node::~Node() {

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
	connections.at(condition);
}