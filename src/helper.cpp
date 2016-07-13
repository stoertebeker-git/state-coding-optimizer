#include "helper.h"
#include "binary.h"
#include "table.h"
//#include "stdafx.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>



using namespace std;

int numNodes_test = 0;

//==============================================================================
// Util function see below.
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
//  Util function to print a vector to a string stream, its used in debug
//  messages and file i/o. This also applies for it overloaded variant
//  which prints a bool vector and not a node vector. Bool commta decides if
//  there should be a ", " between each node name.
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
//  This function returns the number of bits needed for an integer number
//==============================================================================
int bitSize (int temp) {
	if (temp == 1 || temp == 0) {
		return 1;
	}
    int bits = 1;

    while(true) {
        if(temp == 1 || temp == 0)
            return bits;

        temp = temp>>1;
        bits++;
    }
	return bits;
}


//==============================================================================
//  This function calculates the integer value of a binary number
//==============================================================================
int boolVecToInt(std::vector<bool> boolVec) {
	int result = 0;
	for (int i = 0; i < boolVec.size(); i++) {

		result += boolVec[i] * pow(2,  (boolVec.size() - (i+1)));
	}
	return result;
}

vector<bool> charVecToBoolVec(std::vector<char> a)
{
	vector<bool> result;
	for(int i = 0; i < a.size(); i++)  {
		if (a[i] =='1') {
			result.push_back(true);
		}
		else {
			result.push_back(false);
		}
		}

	return result;
}

//==============================================================================
//  This function writes the random generated automate to a readable file.
//  It only exists for testing the file i/o and to make random input files
//  possible.
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
	if (conditions.size() == 0)
		return;
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
//  This function is a util function for the file i/o which writes a number of
//  names for inputbits, outputbits or states. It counts through chars.
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
//  This function assigns a list of nodes as neighbours to each other
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
//  This function iterates through a list of nodes to find to nodes which
//  connects to another. If theres more than one connecting to one node, than
//  assignNeighbours doesnt sort it out.
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
//  Search for neighboured conditions which lead to different nodes. This is
//  a bit hacky because the Node::checkForOneStep should be in helper.cpp not
//  in the Node class
//==============================================================================
void returnPriorityTwo (std::vector<Node*> &nodes, std::vector<Binary *> conditions) {
    for(auto &n : nodes) {
        n->checkForOneStep();
    }
}

//==============================================================================
//  This function searches for nodes which generate the same output with the
//  same input.
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
//  This serves debug purposes and prints the neighbours of all nodes
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
//  This function prints the the optimized code table to a file
//==============================================================================
void printSortedMLFile(Table* table, std::vector<Binary*> conditions) {
    ofstream file;
    file.open("sortedfile.tbl");
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
    file << endl;
    int i = 0;
    for(Binary* &condition : conditions) {
        for(auto const &pair : table->getTable()) {
            file << printVec(condition->returnAsBoolVec(), false);

            if (pair.second == NULL) {
                file << placeholder << "," << placeholder << endl;
                continue;
            }
            file << printVec(pair.first->returnAsBoolVec(), false)
                 << ",";
            if (pair.second->hasSpecificConnection(condition))
                file << printVec(pair.second->getSpecificConnection(condition)->getNodeCode()->returnAsBoolVec(), false) << endl;
            else
                //MODE UNDEFINED file << placeholder << endl;
                file << printVec(pair.first->returnAsBoolVec(),false) << endl;
        }
    }
    file <<  "end" << endl;
    file.close();
}

//==============================================================================
//  This function generates a file with a unoptimizes code table. This means,
//  every node gets a code in following order
//==============================================================================
void printUnsortedMLFile(std::vector<Node*> nodes, std::vector<Binary*> conditions) {
    std::vector<Binary*> unsortedcodes;
    for(int i = 0; i < nodes.size(); i++) {
        unsortedcodes.push_back(new Binary(i, bitSize(nodes.size())));
    }

    ofstream file;
    file.open("unsortedfile.tbl");

    file << "table unsorted" << endl << "input ";
    int conditions_size = conditions.at(0)->getSize();
    string placeholder = "";
    for(int i = 0; i < unsortedcodes.at(0)->getSize(); i++) {
        placeholder += "-";
    }
    while(conditions_size --> 0)
        file << "i" << conditions_size << " ";

    int bits_nodes = bitSize(numNodes_test);
    while(bits_nodes --> 0)
        file << "r" << bits_nodes << " ";

    file << endl << "output ";
    bits_nodes = bitSize(numNodes_test);
    while(bits_nodes --> 0)
        file << "r" << bits_nodes << " ";
    file << endl;
    int i = 0;
    for(Binary* &binary : conditions) {
        for(Node* &node : nodes) {

            file << printVec(binary->returnAsBoolVec(), false)
                 << printVec(unsortedcodes.at(i)->returnAsBoolVec(), false)
                 << ",";
            if(node->hasSpecificConnection(binary)) {
				//TODO subtract 97 from each char if necessary
                //file << printVec(unsortedcodes.at(node->getSpecificConnection(binary)->getName())->returnAsBoolVec(), false);
            } else {
                //MODE UNDEFINED file << placeholder;
                file << printVec(unsortedcodes.at(i)->returnAsBoolVec(), false);
            }

            file << endl;
            i++;
        }
        for(i; i < pow(2, bitSize(nodes.size())); i++) {
            file << printVec(binary->returnAsBoolVec(), false)
                 << placeholder << "," << placeholder
                 << endl;
        }
        i = 0;
    }
    file << "end" << endl;

    for(Binary* &it3 : unsortedcodes)
        delete it3;

    file.close();
}


void setNumNodes(int nodes) {
	numNodes_test = nodes;
}

int getNumNodes() {
	return numNodes_test;
}

void dontCareBits(vector<char> &values, list<vector<bool>> &possibleValues) {
	int numberOfX = 0;
	vector<char> helper(values.size(), '0');
	list<vector<char>> helpList;

	helpList.push_back(helper);

	for (int i = 0; i < values.size(); i++) {
		list<vector<char>>::iterator iter = helpList.begin();
		if (values[i] == '0' || values[i] == '1') {


				while (iter != helpList.end()) {
					helper = *iter;
					helper[i] = values[i];
					*iter = helper;
					iter++;
				}					
		}
		else if (values[i] == 'x') {
			int k = 0;
			//sets value at index i to true and adds another element where value at index i is false
			while( k < pow(2,numberOfX)) { //pow(2,numberOfX) marks end of list, after this loop list size has doubled
				helper = *iter;
				helper[i] = '0';
				*iter = helper;

				helper[i] = '1';
				helpList.push_back(helper);
				cout << "added another element to possibleValues list" << endl;
				iter++;
				k++;
			}	
			numberOfX++;
		}
	}
	list<vector<char>>::iterator iter = helpList.begin();
	while (iter != helpList.end()) {
		possibleValues.push_back(charVecToBoolVec(*iter));
		iter++;
	}

	return;
}
