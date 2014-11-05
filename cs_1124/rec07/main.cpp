#include <iostream>
#include <fstream>
#include <sstream>

#include "Registrar.h"

using namespace std;
using namespace BrooklynPoly;

void processCommand(const vector<string>& command, Registrar& registrar) {
    if (command.size() == 0) {
        cerr << "Did not enter vaild command" << endl;
    }

    if (command[0] == "PrintReport"& command.size() == 1) {
        cout << registrar << endl;
    } else if (command[0] == "AddCourse" && command.size() == 2) {
        registrar.addCourse(command[1]);
    } else if (command[0] == "AddStudent" && command.size() == 2) {
        registrar.addStudent(command[1]);
    } else if (command[0] == "CancelCourse" && command.size() == 2) {
        registrar.cancelCourse(command[1]);
    } else if (command[0] == "EnrollStudentInCourse" && command.size() == 3) {
        registrar.enrollStudentInCourse(command[1], command[2]);
    } else if (command[0] == "Purge" && command.size() == 2) {
        registrar.purge();
    }
}

void readInput(const string& file) {
    Registrar registrar;

    ifstream inputFile(file);
    if (!inputFile) {
        cerr << "Unable to open file: " << file << endl;
    }

    string line;
    while (getline(inputFile, line)) {
        stringstream ss(line);
        string word;
        vector<string> command;
        while (ss >> word) {
            command.push_back(word);
        }
        processCommand(command, registrar);
    }

    inputFile.close();
}

int main() {
    string file = "input";
	readInput(file);
}
