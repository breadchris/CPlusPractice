#include "Noble.h"
#include "Warrior.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;
using namespace WarriorCraft;

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
	
    cout << jim << endl;
    cout << lance << endl;
	   cout << art << endl;
    cout << linus << endl;
    cout << billie << endl;

    cheetah.runaway();
    cout << art << endl;
	
    art.battle(lance);
    jim.battle(lance);
    linus.battle(billie);
    billie.battle(lance);
}

/* OUTPUT
Jim has an army of 1
        Spock: 15
Lancelot du Lac has an army of 2
        Conan: 12
        Hercules: 3
King Arthur has an army of 2
        Merlin: 15
        Tarzan: 10
Linus Torvalds has an army of 1
        Xena: 20
Bill Gates has an army of 1
        Hulk: 8
Tarzan flees in terror, abandoning his lord, King Arthur
King Arthur has an army of 1
        Merlin: 15
King Arthur battles Lancelot du Lac
Mutual Annihalation: King Arthur and Lancelot du Lac die at each other's hands
Jim battles Lancelot du Lac
He's dead, Jim
Linus Torvalds battles Bill Gates
Linus Torvalds defeats Bill Gates
Bill Gates battles Lancelot du Lac
Oh, NO!  They're both dead!  Yuck!
*/