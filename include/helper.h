#ifndef HELPER_H
#define HELPER_H

#include "node.h"

#include <vector>

void writeFile(std::vector<Node> &nodes, std::vector<std::vector<bool>> &conditions);
std::string printVec(std::vector<bool> vector, bool commata);
void writeMLFile(std::vector<Node> &nodes);
std::string generateNames(char start, int amount, bool commata);
#endif // HELPER_H
