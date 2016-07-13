#include "helper.h"
#include "binary.h"
#include "table.h"

#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
//==============================================================================
// Desc: Util function see below.
// Arguments: vector of bools to print, bool for the commata option
// Returns: string stream for files
//==============================================================================
std::string printVec(std::vector<bool> vector, bool commata) {
    ostringstream stream;
    bool isFirst = true;

    for(auto const &n : vector) {

        if(isFirst)
            isFirst = false;
        else {
            if(commata)
                stream << ",";
        }
        stream << n;
    }
    return stream.str();
}

//==============================================================================
// Desc: Util function to print a vector to a string stream, its used in debug
//  messages and file i/o. This also applies for it overloaded variant
//  which prints a bool vector and not a node vector. Bool commta decides if
//  there should be a ", " between each node name.
// Arguments: vector of nodes to print, bool for the commata option
// Returns: string stream for files
//==============================================================================
std::string printVec(std::vector<Node*> vector, bool commata) {
    ostringstream stream;
    bool isFirst = true;

    for(auto const &n : vector) {

        if(isFirst)
            isFirst = false;
        else {
            if(commata)
                stream << ",";
        }
        stream << n->getName();

    }
    return stream.str();
}

//==============================================================================
// Desc: This function returns the number of bits needed for an integer number
// Arguments: an integer which represents a decimal
// Returns: the number of binary digits of this decimal
//==============================================================================
int bitSize (int temp) {

    int bits = 0;
    while(true) {
        if(temp == 1)
            return bits;

        if(temp%2 == 1)
            temp++;

        temp = temp/2;
        bits++;
    }
}

//==============================================================================
// Desc: This function writes the random generated automate to a readable file.
//  It only exists for testing the file i/o and to make random input files
//  possible.
// Arguments: vector of nodes to write, vector of all conditions
// Returns: void
//==============================================================================
void writeFile (std::vector<Node*> &nodes, std::vector<Binary*> conditions) {
    ofstream sampleFile;
    sampleFile.open("samplefile.txt");

    sampleFile <<"DEFSTATE: ";
    for(int i = 0; i < nodes.size(); i++) {
        sampleFile << nodes.at(i)->getName();
        if(i < nodes.size()-1)
            sampleFile << ",";
    }

    string conditions_variable_string = generateNames('i', conditions.at(0)->getSize(), true);
    string output_variable_string = generateNames('v', conditions.at(0)->getSize(), true);

    sampleFile << " DEFIN: " << conditions_variable_string
               << " DEFOUT: " << output_variable_string << endl;

    for(int i = 0; i < nodes.size(); i++) {
        for(auto const &pair : nodes.at(i)->getAllConnections()) {
            sampleFile << "[" << conditions_variable_string << "]=("
                       << printVec(pair.first->returnAsBoolVec(), true)
                       << ")(" << nodes.at(i)->getName() << ")"
                       << " > [" << output_variable_string << "]:("
                       << printVec(nodes.at(i)->getOutputAt(pair.first)->returnAsBoolVec(), true)
                       << ")(" << pair.second->getName() << ")" << endl;
        }
    }
    sampleFile.close();
}

//==============================================================================
// Desc: This function is a util function for the file i/o which writes a number of
//  names for inputbits, outputbits or states. It counts through chars.
// Arguments: char which is the first display name, amount of nodes, commataoption
// Returns: string stream
//==============================================================================
std::string generateNames(char start, int amount, bool commata) {
    ostringstream stream;
    bool isFirst = true;

    for(int i = 0; i < amount; i++) {

        if(isFirst)
            isFirst = false;
        else {
            if(commata)
                stream << ",";
        }
        stream << (char)(start+i);
    }
    return stream.str();
}

//==============================================================================
// Desc: This function assigns a list of nodes as neighbours to each other
// Arguments: the nodelist crossassign, which priority neighbours
// Returns: void
//==============================================================================
void assignNeighbours(std::vector<Node*> &list, short sel) {
    if(list.size() <= 1)
        return;
    for(auto &anchor : list) {
        for(auto &node : list) {
            if(anchor->getName() == node->getName())
                continue;
            anchor->addNeighbour(node, sel);
        }
    }
}

//==============================================================================
// Desc: This function iterates through a list of nodes to find to nodes which
//  connects to another. If theres more than one connecting to one node, than
//  assignNeighbours doesnt sort it out.
// Arguments: a vector of nodes to search through, a vector of conditions to search with
// Returns: void
//==============================================================================
void returnPriorityOne (std::vector<Node*> nodes,std::vector<Binary*> conditions) {

    for(int i = 0; i < conditions.size(); i++) {
        std::map<Node*, std::vector<Node*>> results;

        for(auto &n : nodes) {
            if(!n->hasSpecificConnection(conditions.at(i)))
                continue;

            Node* node = n->getSpecificConnection(conditions.at(i));

            if(results.count(node)) {
                results.at(node).push_back(n);
            } else {
                std::vector<Node*> resultnode;
                resultnode.push_back(n);
                results.insert( std::pair<Node*, std::vector<Node*>>(node, resultnode) );
            }
        }

        for(auto &h : results)
            assignNeighbours(h.second, 0);
    }
}

//==============================================================================
// Desc Search for neighboured conditions which lead to different nodes. This is
//  a bit hacky because the Node::checkForOneStep should be in helper.cpp not
//  in the Node class
// Arguments: a vector of node to search through, a vector of conditions to do this
// Returns: void
//==============================================================================
void returnPriorityTwo (std::vector<Node*> &nodes, std::vector<Binary *> conditions) {
    for(auto &n : nodes) {
        n->checkForOneStep();
    }
}

//==============================================================================
// Desc: This function searches for nodes which generate the same output with the
//  same input.
// Arguments: a vector of nodes to search through, a list of conditions to do so
// Returns: void
//==============================================================================
void returnPriorityThree(std::vector<Node *> &nodes, std::vector<Binary *> conditions) {
    std::vector<Node*> results;
    for(Binary* &condition : conditions) {
        for(Node* &anchor : nodes) {

            results.push_back(anchor);

            if(anchor->hasSpecificConnection(condition)) {
                for(Node* &node : nodes) {
                    if(node->getName()==anchor->getName())
                        continue;
                    if(!node->hasSpecificConnection(condition))
                        continue;
                    if(node->getOutputAt(condition) == anchor->getOutputAt(condition))
                        results.push_back(node);
                }
            }
            assignNeighbours(results,2);
            results.clear();
        }

    }
}

//==============================================================================
// Desc: This is function which helps the random automaton generation by generating
//  random connections between nodes.
// Arguments: a vector of nodes to connect, the conditions by which they
//  connect, and the outputs which they will generate
// Returns: void
//==============================================================================
void generateRandomConnections(std::vector<Node*> &nodes, std::vector<Binary*> &conditions, std::vector<Binary*> &outputs, int numoutten) {
    for(Node* node : nodes) {
        for(Binary* condition : conditions) {
            if(std::rand() % 11 <= numoutten) {
                node->newConnection(nodes.at(std::rand() % nodes.size()), condition);
                node->setOutputAt(outputs.at(std::rand() % outputs.size()), condition);
            }
        }
    }
}

//==============================================================================
// Desc: This serves debug purposes and prints the neighbours of all nodes
// Arguments: a vector of nodes whose neighbours should be printed
// Returns: void
//==============================================================================
void generateOutput(std::vector<Node*> &nodes) {
    for(auto &n : nodes) {
        cout << endl
             << "===================================================" << endl
             << "Found Patterns for Node: " << n->getName()   << endl
             << "Prio 1: " << printVec(n->getNeighbours(0),true)  << endl
             << "Prio 2: " << printVec(n->getNeighbours(1),true) << endl
             << "Prio 3: " << printVec(n->getNeighbours(2), true) << endl
             << "===================================================" << endl;
    }
}

//==============================================================================
// Desc: This function prints the the optimized code table to a file
// Arguments: the table object and the automaton conditions to print to a file
// Returns: void
//==============================================================================
void printSortedMLFile(Table* table, std::vector<Binary*> conditions) {
    ofstream file;
    file.open("/home/stoertebeker/Documents/Studium/ads/projekt/Minilog/Minilog/WorkDir/sortedfile.tbl");
    file << "table sorted" << endl <<"input ";

    int conditions_size = conditions.at(0)->getSize();
    string placeholder = "";
    for(int i = 0; i < table->getBinaries().at(0)->getSize(); i++)
        placeholder += "-";

    while(conditions_size --> 0)
        file << "i" << conditions_size << " ";
    int bits_nodes = table->getBinaries().at(0)->getSize();
    while(bits_nodes --> 0)
        file << "r" << bits_nodes << " ";

    file << endl << "output ";
    bits_nodes = table->getBinaries().at(0)->getSize();

    while(bits_nodes --> 0)
        file << "r" << bits_nodes << " ";
    file << "o" << endl;
    int i = 0;
    for(Binary* &condition : conditions) {
        for(auto const &pair : table->getTable()) {
            if(!pair.second)
                continue;
            file << printVec(condition->returnAsBoolVec(), false);
            file << printVec(pair.first->returnAsBoolVec(), false)
                 << ",";

            if (pair.second->hasSpecificConnection(condition))
                file << printVec(pair.second->getSpecificConnection(condition)->getNodeCode()->returnAsBoolVec(), false)
                     << printVec(pair.second->getOutputAt(condition)->returnAsBoolVec(),false) << endl;
            else
                file << placeholder << "-" << endl;
        }
    }
    file <<  "end" << endl;
    file.close();
}

//==============================================================================
// Desc: This function generates a file with a unoptimizes code table. This means,
//  every node gets a code in following order
// Arguments: The list of nodes to generate an unoptimized code table with, and
//  the conditions which connect them
// Returns: void
//==============================================================================
void printUnsortedMLFile(std::vector<Node*> nodes, std::vector<Binary*> conditions) {
    std::vector<Binary*> unsortedcodes;
    for(int i = 0; i < nodes.size(); i++) {
        unsortedcodes.push_back(new Binary(i, bitSize(nodes.size())));
    }

    ofstream file;
    file.open("/home/stoertebeker/Documents/Studium/ads/projekt/Minilog/Minilog/WorkDir/unsortedfile.tbl");

    file << "table unsorted" << endl << "input ";
    int conditions_size = conditions.at(0)->getSize();
    string placeholder = "";
    for(int i = 0; i < unsortedcodes.at(0)->getSize(); i++) {
        placeholder += "-";
    }
    while(conditions_size --> 0)
        file << "i" << conditions_size << " ";

    int bits_nodes = bitSize(nodes.at(0)->getNumNodes());
    while(bits_nodes --> 0)
        file << "r" << bits_nodes << " ";

    file << endl << "output ";
    bits_nodes = bitSize(nodes.at(0)->getNumNodes());
    while(bits_nodes --> 0)
        file << "r" << bits_nodes << " ";
    file << "o" << endl;
    int i = 0;
    for(Binary* &binary : conditions) {
        for(Node* &node : nodes) {

            file << printVec(binary->returnAsBoolVec(), false)
                 << printVec(unsortedcodes.at(i)->returnAsBoolVec(), false)
                 << ",";
            if(node->hasSpecificConnection(binary)) {
                file << printVec(unsortedcodes.at(node->getSpecificConnection(binary)->getName()-'a')->returnAsBoolVec(), false)
                     << printVec(node->getOutputAt(binary)->returnAsBoolVec(), false);
            } else { 
                //file << printVec(unsortedcodes.at(i)->returnAsBoolVec(), false);
                file << placeholder << "-";
            }

            file << endl;
            i++;
        }
        i = 0;
    }
    file << "end" << endl;

    for(Binary* &it3 : unsortedcodes)
        delete it3;

    file.close();
}
