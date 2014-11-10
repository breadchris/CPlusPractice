#ifndef WARRIOR_H
#define WARRIOR_H

#include "Noble.h"
#include <iostream>
#include <string>

namespace WarriorCraft {
    
    class Noble;
    
    class Warrior {
    public:
        Warrior(const std::string _name, int _strength);
        
        void setBoss(Noble* noble);
        
        friend std::ostream& operator<<(std::ostream& os, const Warrior& warrior);

        std::string getName();

        void runaway();

        bool isHired();

        int getStrength();

        void setStrength(int _strength);

    private:
        Noble* boss;
        std::string name;
        int strength;
    };
}

#endif
