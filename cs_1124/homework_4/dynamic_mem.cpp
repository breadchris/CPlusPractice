#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdlib.h>

using namespace std;

class Warrior {
public:
    Warrior(const string& _name, int _strength)
        : name(_name), strength(_strength) {}

    string getName() const {
        return name;
    }

    void hire() {
        hired = true;
    }

    void fire() {
        hired = false;
    }

    bool getHired() {
        return hired;
    }

    void receiveDamage(int damage) {
        strength -= damage;
    }

    void setStrength(int _strength) {
        strength = _strength;
    }

    int getStrength() const {
        return strength;
    }

    void display() {
        cout << name << ": " << strength << endl;
    }
private:
    string name;
    int strength;
    bool hired;
};

class Noble {
public:
    Noble(const string& _name) : name(_name) {
        dead = false;
    }

    void hire(Warrior* warrior) {
        warrior->hire();
        warriors.push_back(warrior);
    }

    void display() {
        cout << name << " has an army of " << warriors.size() << endl;
        for (size_t i = 0; i < warriors.size(); i++) {
            cout << "\t";
            warriors[i]->display();
        }
    }

    int getArmyStrength() {
        int totalStrength = 0;
        for (size_t i = 0; i < warriors.size(); i++) {
            totalStrength += warriors[i]->getStrength();
        }
        return totalStrength;
    }

    int getArmySize() {
        return warriors.size();
    }

    void battle(Noble* challenger) {
        int thisStrength = this->getArmyStrength();
        int challengerStrength = challenger->getArmyStrength();
        
        if (thisStrength == 0 && challengerStrength == 0) {
            cout << "Oh, NO! They're both dead! Yuck!" << endl;
            return;
        } else if (thisStrength == 0) {
            cout << this->getName() << " is too busy being dead to fight." << endl;
            return;
        } else if (challengerStrength == 0) {
            cout << challenger->getName() << " is too busy being dead to fight." << endl;
            return;
        }

        int outcome = thisStrength - challengerStrength;

        cout << this->getName() << " battles " << challenger->getName() << endl;

        if (outcome == 0) {
            for (size_t i = 0; i < this->warriors.size(); i++) {
                this->warriors[i]->setStrength(0);
            }
            for (size_t i = 0; i < challenger->warriors.size(); i++) {
                challenger->warriors[i]->setStrength(0);
            }
            
            cout << "Mutual Annihalation: " << this->getName() << " and " << challenger->getName() << "die at each other's hands" << endl;
        } else if (outcome > 0) {
            float ratio = thisStrength / challengerStrength;
            for (size_t i = 0; i < this->warriors.size(); i++) {
                int newStrength = int(this->warriors[i]->getStrength() * ratio);
                this->warriors[i]->setStrength(newStrength);
            }

            for (size_t i = 0; i < challenger->warriors.size(); i++) {
                challenger->warriors[i]->setStrength(0);
            }

            cout << this->getName() << " defeats " << challenger->getName() << endl;
        } else {
            float ratio = challengerStrength / thisStrength;
            for (size_t i = 0; i < challenger->warriors.size(); i++) {
                int newStrength = int(challenger->warriors[i]->getStrength() * ratio);
                challenger->warriors[i]->setStrength(newStrength);
            }
            for (size_t i = 0; i < this->warriors.size(); i++) {
                this->warriors[i]->setStrength(0);
            }

            cout << challenger->getName() << " defeats " << this->getName() << endl;
        }

    }

    void fire(const string& name) {
        for (size_t i = 0; i < warriors.size(); i++) {
            if (name == warriors[i]->getName()) {
                cout << warriors[i]->getName() << " has been fired by " << this->name << endl;
                warriors[i]->fire();
                warriors.erase(warriors.begin() + i);
                return;
            }
        }

        cout << "You don't work for me " << name << "! -- " << this->name << endl;
    }

    string getName() const {
        return name;
    }

private:
    string name;
    vector<Warrior*> warriors;
    bool dead;
};

void processCommand(const vector<string>& command, vector<Warrior*>& warriors, vector<Noble*>& nobles) {
    if (command[0] == "Noble") {
        Noble* noble = new Noble(command[1]);
        nobles.push_back(noble);
    } else if (command[0] == "Warrior") {
        Warrior* warrior = new Warrior(command[1], atoi(command[2].c_str()));
        warriors.push_back(warrior);
    } else if (command[0] == "Hire") {
        Noble* noble = nullptr;
        for (size_t i = 0; i < nobles.size(); i++) {
            if (nobles[i]->getName() == command[1]) {
                noble = nobles[i];
            }
        }

        Warrior* warrior = nullptr;
        for (size_t i = 0; i < warriors.size(); i++) {
            if (warriors[i]->getName() == command[2]) {
                warrior = warriors[i];
            }
        }

        if (warrior != nullptr && noble != nullptr) {
            noble->hire(warrior);
        }
    } else if (command[0] == "Fire") {
        Noble* noble = nullptr;
        for (size_t i = 0; i < nobles.size(); i++) {
            if (nobles[i]->getName() == command[1]) {
                noble = nobles[i];
            }
        }

        if (noble != nullptr) {
            noble->fire(command[2]);
        }
    } else if (command[0] == "Battle") {
        Noble* noble1 = nullptr;
        for (size_t i = 0; i < nobles.size(); i++) {
            if (nobles[i]->getName() == command[1]) {
                noble1 = nobles[i];
            }
        }

        Noble* noble2 = nullptr;
        for (size_t i = 0; i < nobles.size(); i++) {
            if (nobles[i]->getName() == command[2]) {
                noble2 = nobles[i];
            }
        }

        if (noble1 != nullptr && noble2 != nullptr) {
            noble1->battle(noble2);
        }
    } else if (command[0] == "Clear") {
        for (size_t i = 0; i < nobles.size(); i++) {
            delete nobles[i];
        }
        nobles.clear();

        for (size_t i = 0; i < warriors.size(); i++) {
            delete warriors[i];
        }
        warriors.clear();

    } else if (command[0] == "Status") {
        cout << "Status" << endl;
        cout << "======" << endl;

        cout << "Nobles:" << endl;
        if (nobles.size() == 0) {
            cout << "NONE" << endl;
        }

        for (size_t i = 0; i < nobles.size(); i++) {
            nobles[i]->display();
        }

        cout << "Unemployed Warriors:" << endl;
        bool has_unemployed = false;
        for (size_t i = 0; i < warriors.size(); i++) {
            if (!warriors[i]->getHired()) {
                has_unemployed = true;
                warriors[i]->display();
            }
        }

        if (!has_unemployed) {
            cout << "NONE" << endl;
        }
    }
}

void readInput(const string& file) {
    ifstream inputFile(file);
    if (!inputFile) {
        cerr << "Unable to open file: " << file << endl;
    }

    string line;
    vector<Warrior*> warriors;
    vector<Noble*> nobles;

    while (getline(inputFile, line)) {
        stringstream ss(line);
        string word;
        vector<string> command;
        while (ss >> word) {
            command.push_back(word);
        }
        processCommand(command, warriors, nobles);
    }
}

int main() {
    string file = "nobleWarriors.txt";
	readInput(file);
}
