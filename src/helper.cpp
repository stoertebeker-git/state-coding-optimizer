#include "helper.h"

#include <iostream>
#include <fstream>

using namespace std;


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
            sampleFile << "[" << conditions_variable_string << "]=(";

            bool isFirst = true;
            for(auto const &n : pair.first) {
                if(isFirst)
                    isFirst = false;
                else
                    sampleFile << ",";

                sampleFile << n;
            }
                       //<< pair.first.at(0) << "," << pair.first.at(1)
            sampleFile << ")(" << nodes.at(i).getName() << ")"
                       << " > [" << output_variable_string << "]:(";

            isFirst = true;
            for(auto const &n : nodes.at(i).getOutputAt(pair.first)) {
                if(isFirst)
                    isFirst = false;
                else
                    sampleFile << ",";

                sampleFile << n;
            }

            sampleFile << ")(" << pair.second.getName() << ")" << endl;
        }
    }

    sampleFile.close();
}
