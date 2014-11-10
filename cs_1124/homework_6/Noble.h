#ifndef NOBLE_H
#define NOBLE_H

#include "Warrior.h"
#include <iostream>
#include <string>
#include <vector>

namespace WarriorCraft {
    
    class Warrior;
    
    class Noble {
    public:
        Noble(const std::string _name);
        
        void hire(Warrior& warrior);
        
        void battle(Noble& noble);

        void notify(Warrior* warrior);

        friend std::ostream& operator<<(std::ostream& os, const Noble& noble);

        int getArmyStrength() const;

        std::string getName();
    private:
        std::string name;
        std::vector<Warrior*> warriors;
    };
}

#endif
