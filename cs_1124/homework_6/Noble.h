#ifndef NOBEL_H
#define NOBLE_H

#include <string>
#include <vector>

namespace WarriorCraft {
    
    class Warrior;
    
    class Noble {
    private:
        Nobel(const std::string _name);
        
        void hire(Warrior* warrior);
        
        void battle(Noble* noble);
    public:
        string name;
        std::vector<Warrior*> warriors;
    };
}

#endif