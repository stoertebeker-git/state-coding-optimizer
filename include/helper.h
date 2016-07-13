#ifndef HELPER_H
#define HELPER_H

#include "node.h"
#include "binary.h"
#include "table.h"

#include <vector>
#include <list>


int bitSize(int temp);
int boolVecToInt(std::vector<bool>);

std::vector<bool> charVecToBoolVec(std::vector<char> a);

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


void generateOutput(std::vector<Node*> &nodes);

void printSortedMLFile(Table* table,
                       std::vector<Binary *> conditions);

void printUnsortedMLFile(std::vector<Node*> nodes,
                         std::vector<Binary *> conditions);
//__declspec(selectany) int numNodes_test;
void setNumNodes(int nodes);

int getNumNodes();


void dontCareBits(std::vector<char> &values, std::list<std::vector<bool>> &possibleValues);

#endif // HELPER_H
