/*
 * medieval_times.cpp
 * Christopher Thompson
 * Homework 1
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

struct Warrior {
    string name;
    int strength;
};

int readFile();

int main() {
    return readFile();
}

void printVector(const vector<string>& v) {
    for (int i = 0; i < v.size(); i++) {
        cout << v[i] << endl;
    }
}

int findWarrior(const string& warrior_name, const vector<Warrior>& warriors) {
    for (int i = 0; i < warriors.size(); i++) {
        if (warriors[i].name.compare(warrior_name) == 0) {
            return i;
        }
    }

    return -1;
}

int processCommand(const vector<string>& command, vector<Warrior>& warriors) {

    if (command.size() < 1) {
        return 1;
    }

    if (command[0] == "Warrior") {
        Warrior newWarrior;
        newWarrior.name = command[1];
        newWarrior.strength = atoi(command[2].c_str());
        warriors.push_back(newWarrior);
    } else if (command[0] == "Status") {
        string plural = " warriors ";
        if (warriors.size() == 1) {
            plural = " warrior ";
        }

        cout << "There are: " << warriors.size() << plural << endl;
        
        for (int i =0; i < warriors.size(); i++) {
            cout << "\tWarrior: " << warriors[i].name << ", strength: " << warriors[i].strength << endl;
        }
    } else if (command[0] == "Battle") {
        int warrior1Loc = findWarrior(command[1], warriors);
        int warrior2Loc = findWarrior(command[2], warriors);

        if (warrior1Loc == -1 || warrior2Loc == -1) {
            return 1;
        }

        Warrior& warrior1 = warriors[warrior1Loc];
        Warrior& warrior2 = warriors[warrior2Loc];

        cout << warrior1.name << " battles " << warrior2.name << endl;

        bool warrior1Dead = false;
        bool warrior2Dead = false;
        if (warrior1.strength <= 0) {
            warrior1Dead = true;
        }

        if (warrior2.strength <= 0) {
            warrior2Dead = true;
        }

        if (warrior1Dead && warrior2Dead) {
            cout << "\tOh, NO! They're both dead! Yuck!" << endl;
        } else if (warrior1Dead) {
            cout << "\t" << warrior2.name << " wins because " << warrior1.name << " was already dead" << endl;
        } else if (warrior2Dead) {
            cout << "\t" << warrior1.name << " wins because " << warrior2.name << " was already dead" << endl;
        }

        if (warrior1.strength == warrior2.strength) {
            warrior1.strength = 0;
            warrior2.strength = 0;
            cout << "\tMutual Annihilation: " << warrior1.name << " and " << warrior2.name << " die at each other's hands" << endl;
        } else {
            if (warrior1.strength > warrior2.strength) {
                cout << "\t" << warrior1.name << " defeats " << warrior2.name;
            } else {
                cout << "\t" << warrior2.name << " defeats " << warrior1.name;
            }
            warrior1.strength -= warrior2.strength;
            warrior2.strength -= warrior1.strength;

            if (warrior1.strength <= 0) {
                warrior1.strength = 0;
                cout << " and killed him in the process" << endl;
            } else if (warrior2.strength <= 0) {
                warrior2.strength = 0;
                cout << " and killed him in the process" << endl;
            }
        }
    } else {
        return 1;
    }
}

int readFile() {
    ifstream inputFile("input");
    if (!inputFile) {
        cerr << "Unable to open file: input" << endl;
        return 1;
    }

    string line;
    vector<Warrior> warriors;

    while (getline(inputFile, line)) {
        stringstream ss(line);
        string word;
        vector<string> command;
        while (ss >> word) {
            command.push_back(word);
        }
        
        processCommand(command, warriors);
    }
}

