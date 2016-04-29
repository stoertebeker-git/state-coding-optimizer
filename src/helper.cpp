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

void writeFile (std::vector<Node> &nodes, std::vector<std::vector<bool>> &conditions) {
    ofstream sampleFile;
    sampleFile.open("SampleFile.txt");

    sampleFile <<"DESTATE: ";
                for(int i = 0; i < nodes.size(); i++) {
                    sampleFile << nodes.at(i).getName();
                    if(i < nodes.size()-1)
                        sampleFile << ",";
                }

    int conditions_size = (int)conditions.at(0).size();
    string conditions_variable_string;
    char conditions_code_first = 'i';
                for(int i = 0; i < conditions_size; i++) {
                    conditions_variable_string += (char)(conditions_code_first + i);
                    if(i < conditions_size-1)
                        conditions_variable_string += ",";
                }

    int output_size = (int)nodes.at(0).getAnyOutput().size();
    string output_variable_string;
    char output_code_first = 'v';
                for(int i = 0; i < output_size; i++) {
                     output_variable_string += (char)(output_code_first + i);
                     if(i < output_size-1)
                         output_variable_string += ",";
                }
    sampleFile << " DEFIN: " << conditions_variable_string
               << " DEFOUT " << output_variable_string << endl;

    for(int i = 0; i < nodes.size(); i++) {
        for(auto const &pair : nodes.at(i).connections) {
            sampleFile << "[" << conditions_variable_string << "]=("
                       << printVec(pair.first, true)
                       << ")(" << nodes.at(i).getName() << ")"
                       << " > [" << output_variable_string << "]:("
                       << printVec(nodes.at(i).getOutputAt(pair.first), true)
                       << ")(" << pair.second.getName() << ")" << endl;
        }
    }

    sampleFile.close();
}

void writeMLFile(std::vector<Node> &nodes) {
     ofstream ml_file;
     ml_file.open("gib.tbl");
     ml_file << "input " << generateNames('i', nodes.at(0).getConditionSize(true), true) << endl;
     ml_file << "output " << generateNames('v', nodes.at(0).getConditionSize(false), true) << endl;
     for(int i = 0; i < nodes.size(); i++) {
         for(auto const &pair : nodes.at(i).getOutput()) {
             ml_file << printVec(pair.first, false)
                     << ","
                     << printVec(pair.second, false) << endl;

         }
     }
     ml_file << "end" << endl;
     ml_file.close();
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

