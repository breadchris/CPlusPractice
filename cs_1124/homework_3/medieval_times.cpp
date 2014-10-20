/*
 * medieval_times.cpp
 * Christopher Thompson
 * Homework 3
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

using namespace std;

class Warrior {
public:
    Warrior(const string _name, int _strength) : name(_name), strength(_strength) {}

    string getName() const {
        return name;
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
};

class Noble {
public:
    Noble(const string& _name) : name(_name) {
        dead = false;
    }

    void hire(Warrior& warrior) {
        warriors.push_back(&warrior);
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

    void battle(Noble& challenger) {
        int thisStrength = this->getArmyStrength();
        int challengerStrength = challenger.getArmyStrength();
        
        if (thisStrength == 0 && challengerStrength == 0) {
            cout << "Oh, NO! They're both dead! Yuck!" << endl;
            return;
        } else if (thisStrength == 0) {
            cout << this->getName() << " is too busy being dead to fight." << endl;
            return;
        } else if (challengerStrength == 0) {
            cout << challenger.getName() << " is too busy being dead to fight." << endl;
            return;
        }

        int outcome = thisStrength - challengerStrength;

        cout << this->getName() << " battles " << challenger.getName() << endl;

        if (outcome == 0) {
            for (size_t i = 0; i < this->warriors.size(); i++) {
                this->warriors[i]->setStrength(0);
            }
            for (size_t i = 0; i < challenger.warriors.size(); i++) {
                challenger.warriors[i]->setStrength(0);
            }
            
            cout << "Mutual Annihalation: " << this->getName() << " and " << challenger.getName() << "die at each other's hands" << endl;
        } else if (outcome > 0) {
            float ratio = thisStrength / challengerStrength;
            for (size_t i = 0; i < this->warriors.size(); i++) {
                int newStrength = int(this->warriors[i]->getStrength() * ratio);
                this->warriors[i]->setStrength(newStrength);
            }

            for (size_t i = 0; i < challenger.warriors.size(); i++) {
                challenger.warriors[i]->setStrength(0);
            }

            cout << this->getName() << " defeats " << challenger.getName() << endl;
        } else {
            float ratio = challengerStrength / thisStrength;
            for (size_t i = 0; i < challenger.warriors.size(); i++) {
                int newStrength = int(challenger.warriors[i]->getStrength() * ratio);
                challenger.warriors[i]->setStrength(newStrength);
            }
            for (size_t i = 0; i < this->warriors.size(); i++) {
                this->warriors[i]->setStrength(0);
            }

            cout << challenger.getName() << " defeats " << this->getName() << endl;
        }

    }

    void fire(Warrior& warrior) {
        for (size_t i = 0; i < warriors.size(); i++) {
            if (warrior.getName() == warriors[i]->getName()) {
                cout << warriors[i]->getName() << " has been fired by " << this->name << endl;
                warriors.erase(warriors.begin() + i);
                return;
            }
        }
    }

    string getName() const {
        return name;
    }

private:
    string name;
    vector<Warrior*> warriors;
    bool dead;
};

int main() {
    Noble art("King Arthur");
    Noble lance("Lancelot du Lac");
    Noble jim("Jim");
    Noble linus("Linus Torvalds");
    Noble billie("Bill Gates");

    Warrior cheetah("Tarzan", 10);
    Warrior wizard("Merlin", 15);
    Warrior theGovernator("Conan", 12);
    Warrior nimoy("Spock", 15);
    Warrior lawless("Xena", 20);
    Warrior mrGreen("Hulk", 8);
    Warrior dylan("Hercules", 3);

    jim.hire(nimoy);
    lance.hire(theGovernator);
    art.hire(wizard);
    lance.hire(dylan);
    linus.hire(lawless);
    billie.hire(mrGreen);
    art.hire(cheetah);

    jim.display();
    lance.display();
    art.display();
    linus.display();
    billie.display();

    art.fire(cheetah);
    art.display();

    art.battle(lance);
    jim.battle(lance);
    linus.battle(billie);
    billie.battle(lance);
}
