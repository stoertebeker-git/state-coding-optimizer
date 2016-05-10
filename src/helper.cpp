#include "helper.h"
#include "binary.h"
#include "table.h"

#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

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

int bitSize (int temp) {

    int bits = 0;
    while(true) {
        if(temp == 1) {
            return bits;
        }
        if(temp%2 == 1) {
            temp++;
        }
        temp = temp/2;
        bits++;
    }
}

std::string printMap(std::map<Node*, std::vector<Node*>> map) {
    ostringstream stream;
    for(auto const &pair : map) {
        stream << pair.first->getName() << "<-"
               << printVec(pair.second, true) << endl;
    }
    return stream.str();
}

void writeFile (std::vector<Node*> &nodes, std::vector<Binary*> conditions) {
    ofstream sampleFile;
    sampleFile.open("samplefile.txt");

    sampleFile <<"DESTATE: ";
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
        for(auto const &pair : nodes.at(i)->getConnections()) {
            sampleFile << "[" << conditions_variable_string << "]=("
                       << printVec(pair.first->returnAsBoolVec(), true)
                       << ")(" << nodes.at(i)->getName() << ")"
                       << " > [" << output_variable_string << "]:("
                       << printVec(nodes.at(i)->getOutputAt(pair.first), true)
                       << ")(" << pair.second->getName() << ")" << endl;
        }
    }
    sampleFile.close();
}

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

void assignNeighbours(std::vector<Node*> &list, short select) {
    if(list.size() <= 1)
        return;
    for(auto &n : list) {
        for(auto &m : list) {
            if(n->getName() == m->getName())
                continue;
            if(select == 1)
                n->addFirstNeighbour(m);
            else if(select == 2)
                n->addSecondNeighbour(m);
            else if(select == 3)
                n->addThirdNeighbour(m);
            else return;
        }
    }
}

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
            assignNeighbours(h.second, true);
    }
}
void returnPriorityTwo (std::vector<Node*> &nodes, std::vector<Binary *> conditions) {
    for(auto &n : nodes) {
        n->checkForOneStep();
    }
}

void returnPriorityThree (std::vector<Node*> &nodes, std::vector<Binary*> conditions) {
    std::vector<Node*> results;
    for(auto &n : nodes) {
        for(int j = 0; j < conditions.size(); j++) {
            if(!n->hasSpecificConnection(conditions.at(j)))
                continue;
            if(n->getOutputAt(conditions.at(j)) == conditions.at(j)->returnAsBoolVec()) {
                results.push_back(n);
            }
        }
    }
    assignNeighbours(results,3);
}

void generateRandomConnections(std::vector<Node*> &nodes, std::vector<Binary*> &conditions, int numoutten) {
    for(int y = 0; y < nodes.size(); y++) {
        for(int i = 0; i < conditions.size(); i++) {
            if(std::rand() % 11 <= numoutten) {
                nodes.at(y)->newConnection(nodes.at(std::rand()%(nodes.size())), conditions.at(i));
                std::vector<bool> outputGenerate;
                for(int z = 0; z < conditions.at(i)->getSize(); z++) {
                    outputGenerate.push_back(std::rand()%2);
                }
                nodes.at(y)->setOutputAt(conditions.at(i), outputGenerate);
            }
        }
    }
}

void generateOutput(std::vector<Node*> &nodes) {
    for(auto &n : nodes) {
        cout << endl
             << "===================================================" << endl
             << "Found Patterns for Node: " << n->getName()   << endl
             << "Prio 1: " << printVec(n->getFirstNeighbours(),true)  << endl
             << "Prio 2: " << printVec(n->getSecondNeighbours(),true) << endl
             << "Prio 3: " << printVec(n->getThirdNeighbours(), true) << endl
             << "===================================================" << endl;
    }
}

void printSortedMLFile(Table* table, std::vector<Binary*> conditions) {
    ofstream file;
    file.open("sortedfile.tbl");
    file << "input ";

    int conditions_size = conditions.at(0)->getSize();
    string placeholder = "";
    for(int i = 0; i < table->getBinaries().at(0)->getSize(); i++) {
        placeholder += "-";
    }
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
    file <<  "end" << endl;

    file.close();
}

void printUnsortedMLFile(std::vector<Node*> nodes, std::vector<Binary*> conditions) {
    std::vector<Binary*> unsortedcodes;
    for(int i = 0; i < nodes.size(); i++) {
        unsortedcodes.push_back(new Binary(i, bitSize(nodes.size())));
    }

    ofstream file;
    file.open("unsortedfile.tbl");

    file << "input ";
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
    file << endl;
    int i = 0;
    for(Binary* &binary : conditions) {
        for(Node* &node : nodes) {

            file << printVec(binary->returnAsBoolVec(), false)
                 << printVec(unsortedcodes.at(i)->returnAsBoolVec(), false)
                 << ",";
                 if(node->hasSpecificConnection(binary)) {
                         file << printVec(unsortedcodes.at(node->getSpecificConnection(binary)->getName()-'a')->returnAsBoolVec(), false);
                 } else {
                         file << placeholder;
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
