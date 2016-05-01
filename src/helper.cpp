#include "helper.h"

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

std::string printMap(std::map<Node*, std::vector<Node*>> map) {
    ostringstream stream;
    for(auto const &pair : map) {
        stream << pair.first->getName() << "<-";
        stream << printVec(pair.second, true) << endl;
    }
    return stream.str();
}

void writeFile (std::vector<Node*> &nodes, std::vector<std::vector<bool>> &conditions) {
    ofstream sampleFile;
    sampleFile.open("SampleFile.txt");

    sampleFile <<"DESTATE: ";
                for(int i = 0; i < nodes.size(); i++) {
                    sampleFile << nodes.at(i)->getName();
                    if(i < nodes.size()-1)
                        sampleFile << ",";
                }

    string conditions_variable_string = generateNames('i', nodes.at(0)->getConditionSize(true), true);
    string output_variable_string = generateNames('v', nodes.at(0)->getConditionSize(false), true);

    sampleFile << " DEFIN: " << conditions_variable_string
               << " DEFOUT: " << output_variable_string << endl;

    for(int i = 0; i < nodes.size(); i++) {
        for(auto const &pair : nodes.at(i)->getConnections()) {
            sampleFile << "[" << conditions_variable_string << "]=("
                       << printVec(pair.first, true)
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

void assignFirstNeighbours(std::vector<Node*> &list) {
    if(list.size() <= 1) {
        std::cout << "too small" << endl;
        return;
    }
    for(auto &n : list) {
        for(auto &m : list) {
            if(n->getName() == m->getName())
                continue;
            n->addFirstNeighbour(m);
        }
    }
}


