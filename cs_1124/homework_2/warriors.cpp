/*
 * warriors.cpp
 * Christopher Thompson
 * Homework 2
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdlib.h>

using namespace std;

class Weapon {
public:
    Weapon(string _name, int _strength) : name(_name), strength(_strength) {}

    int getStrength() {
        return strength;
    }

    string getName() const {
        return name;
    }

    void setStrength(int _strength) {
        strength = _strength;
    }

private:
    string name;
    int strength;
};

class Warrior {
public:
    Warrior(string _name, Weapon* _weapon) : name(_name), weapon(_weapon) {}

    string getName() const {
        return name;
    }

    int getWeaponStrength() const {
        return weapon->getStrength();
    }

    string getWeaponName() const {
        return weapon->getName();
    }

    void receiveDamage(int damage) {
        weapon->setStrength(weapon->getStrength() - damage);
    }

    void setWeaponStrength(int strength) {
        weapon->setStrength(strength);
    }

    int battle(Warrior* warrior) {
        bool warrior1Dead = false;
        bool warrior2Dead = false;
        if (this->getWeaponStrength() <= 0) {
            warrior1Dead = true;
        }

        if (warrior->getWeaponStrength() <= 0) {
            warrior2Dead = true;
        }

        if (warrior1Dead && warrior2Dead) {
            cout << "\tOh, NO! They're both dead! Yuck!" << endl;
        }

        if (this->getWeaponStrength() == warrior->getWeaponStrength()) {
            this->setWeaponStrength(0);
            warrior->setWeaponStrength(0);
            cout << "\tMutual Annihilation: " << this->getName() << " and " << warrior->getName() << " die at each other's hands" << endl;
        } else {
            if (this->getWeaponStrength() > warrior->getWeaponStrength()) {
                cout << "\t" << this->getName() << " defeats " << warrior->getName();
            } else {
                cout << "\t" << warrior->getName() << " defeats " << this->getName();
            }

            this->receiveDamage(warrior->getWeaponStrength());
            warrior->receiveDamage(this->getWeaponStrength());

            if (this->getWeaponStrength() <= 0) {
                this->setWeaponStrength(0);
                cout << " and killed him in the process" << endl;
            } else if (warrior->getWeaponStrength() <= 0) {
                warrior->setWeaponStrength(0);
                cout << " and killed him in the process" << endl;
            }
        }
    }

private:
    string name;
    Weapon* weapon;
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
        if (warriors[i].getName().compare(warrior_name) == 0) {
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
        Weapon* weapon = new Weapon(((string) command[2]), atoi(command[3].c_str()));
        Warrior* newWarrior = new Warrior(((string) command[1]), weapon);
        warriors.push_back(*newWarrior);
    } else if (command[0] == "Status") {
        string plural = " warriors ";
        if (warriors.size() == 1) {
            plural = " warrior ";
        }

        cout << "There are: " << warriors.size() << plural << endl;
        
        for (int i =0; i < warriors.size(); i++) {
            cout << "\tWarrior: " << warriors[i].getName() << ", Weapon Name: " << warriors[i].getWeaponName() << ", Weapon Strength: " << warriors[i].getWeaponStrength() << endl;
        }
    } else if (command[0] == "Battle") {
        int warrior1Loc = findWarrior(command[1], warriors);
        int warrior2Loc = findWarrior(command[2], warriors);

        if (warrior1Loc == -1 || warrior2Loc == -1) {
            return 1;
        }

        Warrior* warrior1 = &warriors[warrior1Loc];
        Warrior* warrior2 = &warriors[warrior2Loc];

        cout << warrior1->getName() << " battles " << warrior2->getName() << endl;

        warrior1->battle(warrior2);

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
