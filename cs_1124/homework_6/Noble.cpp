#include <string>
#include <vector>
#include <iostream>

#include "Noble.h"
#include "Warrior.h"

using namespace std;

namespace WarriorCraft {
    
    class Warrior;
    
    Noble::Noble(const string _name) : name(_name) {}
    
    void Noble::hire(Warrior& warrior) {
        if (!warrior.isHired()) {
            warriors.push_back(&warrior);
            warrior.setBoss(this);
        }
    }
    
    void Noble::battle(Noble& noble) {
        cout << this->getName() << " battles " << noble.getName() << endl;

        int thisStrength = this->getArmyStrength();
        int nobleStrength = noble.getArmyStrength();
        
        if (thisStrength == 0 && nobleStrength == 0) {
            cout << "Oh, NO! They're both dead! Yuck!" << endl;
            return;
        } else if (thisStrength == 0) {
            cout << "He's dead, " << noble.getName() << endl;
            return;
        } else if (nobleStrength == 0) {
            cout << "He's dead, " << this->getName() << endl;
            return;
        }

        int outcome = thisStrength - nobleStrength;

        if (outcome == 0) {
            for (size_t i = 0; i < this->warriors.size(); i++) {
                this->warriors[i]->setStrength(0);
            }
            for (size_t i = 0; i < noble.warriors.size(); i++) {
                noble.warriors[i]->setStrength(0);
            }
            
            cout << "Mutual Annihalation: " << this->getName() << " and " << noble.getName() << "die at each other's hands" << endl;
        } else if (outcome > 0) {
            float ratio = thisStrength / nobleStrength;
            for (size_t i = 0; i < this->warriors.size(); i++) {
                int newStrength = int(this->warriors[i]->getStrength() * ratio);
                this->warriors[i]->setStrength(newStrength);
            }

            for (size_t i = 0; i < noble.warriors.size(); i++) {
                noble.warriors[i]->setStrength(0);
            }

            cout << this->getName() << " defeats " << noble.getName() << endl;
        } else {
            float ratio = nobleStrength / thisStrength;
            for (size_t i = 0; i < noble.warriors.size(); i++) {
                int newStrength = int(noble.warriors[i]->getStrength() * ratio);
                noble.warriors[i]->setStrength(newStrength);
            }
            for (size_t i = 0; i < this->warriors.size(); i++) {
                this->warriors[i]->setStrength(0);
            }

            cout << noble.getName() << " defeats " << this->getName() << endl;
        }


    }

    void Noble::notify(Warrior* warrior) {
        for (size_t i = 0; i < warriors.size(); i++) {
            if (warriors[i] == warrior) {
                warriors.erase(warriors.begin() + i);
                break;
            }
        }
    }

    ostream& operator<<(std::ostream& os, const Noble& noble) {
        os << noble.name << " has an army of " << noble.warriors.size() << endl;
        for (const Warrior* w : noble.warriors) {
            cout << *w;
        }
        return os;
    }

    int Noble::getArmyStrength() const {
        int totalStrength = 0;
        for (Warrior* w : warriors) {
            totalStrength += w->getStrength();
        }
        return totalStrength;
    }

    string Noble::getName() {
        return name;
    }
}
