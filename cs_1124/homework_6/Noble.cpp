#include <string>
#include <vector>

#include "Nobel.h"
#include "Warrior.h"

using namespace std;

namespace WarriorCraft {
    
    class Warrior;
    
    Noble::Noble(const string _name) : name(_name) {}
    
    void Noble::hire(Warrior* warrior) {
        if (!warrior.isHired()) {
            warriors.push_back(warrior);
            warrior->setBoss(this);
        }
    }
    
    void Noble::battle(Noble* noble) {
        
    }
}
