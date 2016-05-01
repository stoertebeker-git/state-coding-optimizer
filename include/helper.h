#ifndef HELPER_H
#define HELPER_H

#include "node.h"

#include <vector>

void writeFile(std::vector<Node*> &nodes, std::vector<std::vector<bool>> &conditions);
std::string printVec(std::vector<bool> vector, bool commata);
std::string printVec(std::vector<Node*> vector, bool commata);

std::string printMap(std::map<Node*, std::vector<Node*>> map);

void writeMLFile(std::vector<Node*> &nodes);
std::string generateNames(char start, int amount, bool commata);
void assignFirstNeighbours(std::vector<Node*> &list);
#endif // HELPER_H
