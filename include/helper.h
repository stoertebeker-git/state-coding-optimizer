#ifndef HELPER_H
#define HELPER_H

#include "node.h"
#include "condition.h"

#include <vector>

void writeFile(std::vector<Node*> &nodes, std::vector<Condition *> conditions);
std::string printVec(std::vector<bool> vector, bool commata);
std::string printVec(std::vector<Node*> vector, bool commata);

std::string printMap(std::map<Node*, std::vector<Node*>> map);

void writeMLFile(std::vector<Node*> &nodes);
std::string generateNames(char start, int amount, bool commata);
void assignNeighbours(std::vector<Node*> &list, short select);

void returnPriorityOne (std::vector<Node*> nodes, std::vector<Condition *> conditions);
void returnPriorityTwo (std::vector<Node*> &nodes, std::vector<Condition*> conditions);
void returnPriorityThree (std::vector<Node*> &nodes, std::vector<Condition *> conditions);
void printAutomate(std::vector<Node*> &nodes);
void generateRandomConnections(std::vector<Node*> &nodes, std::vector<Condition*> &conditions, int numoutten);
void generateOutput(std::vector<Node*> &nodes);
#endif // HELPER_H
