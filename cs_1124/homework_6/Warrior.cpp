
#include <iostream>

#include "Warrior.h"
#include "Noble.h"

using namespace std;

namespace WarriorCraft {

    Warrior::Warrior(const string _name, int _strength) : name(_name), strength(_strength), boss(nullptr) {}

    void Warrior::setBoss(Noble* noble) {
        if (boss == nullptr && noble != nullptr) {
            boss = noble;
        }
    }
    
    ostream& operator<<(ostream& os, const Warrior& warrior) {
        os << "\t" << warrior.name << ": " << warrior.strength << endl;
        return os;
    }

    void Warrior::runaway() {
        cout << name << " flees in terror, abandoning his lord, " << boss->getName() << endl;
        boss->notify(this);
        boss = nullptr;
    }

    string Warrior::getName() {
        return name;
    }

    bool Warrior::isHired() {
        return boss != nullptr;
    }

    int Warrior::getStrength() {
        return strength;
    }

    void Warrior::setStrength(int _strength) {
        strength = _strength;
    }
}
