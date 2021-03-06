#ifndef HELPER_H
#define HELPER_H

#include "node.h"
#include "binary.h"
#include "table.h"

#include <vector>

void writeFile(std::vector<Node*> &nodes,
               std::vector<Binary *> conditions);

std::string printVec(std::vector<bool> vector,
                     bool commata);

std::string printVec(std::vector<Node*> vector,
                     bool commata);

std::string generateNames(char start,
                          int amount,
                          bool commata);

void assignNeighbours(std::vector<Node*> &list,
                      short sel);

void returnPriorityOne (std::vector<Node*> nodes,
                        std::vector<Binary *> conditions);

void returnPriorityTwo (std::vector<Node*> &nodes,
                        std::vector<Binary*> conditions);

void returnPriorityThree (std::vector<Node*> &nodes,
                          std::vector<Binary *> conditions);

void printAutomate(std::vector<Node*> &nodes);

void generateRandomConnections(std::vector<Node*> &nodes,
                               std::vector<Binary*> &conditions,
                               std::vector<Binary*> &outputs,
                               int numoutten);

void generateOutput(std::vector<Node*> &nodes);

void printSortedMLFile(Table* table,
                       std::vector<Binary *> conditions);

void printUnsortedMLFile(std::vector<Node*> nodes,
                         std::vector<Binary *> conditions);

int bitSize(int temp);
#endif // HELPER_H
