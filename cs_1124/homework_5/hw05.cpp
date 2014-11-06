
#include <iostream>
#include <vector>

using namespace std;

class Nobel;

class Protector {
protected:
    string name;
    int strength;
    Nobel* boss;
public:
    Protector (const string _name, int _strength)
        : name(_name), strength(_strength), boss(nullptr) {}

    virtual void defend() = 0;

    bool hire(Nobel* nobel) {
        if (isDead() || boss != nullptr) {
            return false;
        } else {
            boss = nobel;
            return true;
        }
    }

    bool isDead() { return strength == 0; }

    int getStrength() { return strength; }

    void setStrength(int _strength) {
        strength = _strength;
    }
};

class Nobel {
protected:
    bool dead;
    string name;
public:
    Nobel (const string _name) : name(_name), dead(false) {}

    bool isDead() { return dead; }

    virtual void battle(Nobel& nobel) {
        int thisStrength = this->getStrength();
        int challengerStrength = nobel.getStrength();

        cout << name << " battles " << nobel.name << endl;

        if (this->isDead() && nobel.isDead()) {
            cout << "Oh, NO! They're both dead! Yuck!" << endl;
            return;
        } else if (this->isDead()) {
            cout << "He's dead, " << nobel.name << endl;
            return;
        } else if (nobel.isDead()) {
            cout << "He's dead, " << name << endl;
            return;
        }

        if (!nobel.getStrength()) {
            cout << name << " defeats " << nobel.name << endl;
            nobel.die();
            return;
        }

        if (!this->getStrength()) {
            cout << nobel.name << " defeats " << name << endl;
            this->die();
            return;
        }

        this->battleCry();
        nobel.battleCry();

        int outcome = thisStrength - challengerStrength;

        if (outcome == 0) {
            this->die();
            nobel.die();
            
            cout << "Mutual Annihalation: " << name << " and " << nobel.name << "die at each other's hands" << endl;
        } else if (outcome > 0) {
            float ratio = thisStrength / challengerStrength;
            this->takeDamage(ratio);

            nobel.die();

            cout << name << " defeats " << nobel.name << endl;
        } else {
            float ratio = challengerStrength / thisStrength;
            nobel.takeDamage(ratio);

            this->die();

            cout << nobel.name << " defeats " << name << endl;
        }
    }

    virtual int getStrength() = 0;
    virtual void die() = 0;
    virtual void takeDamage(float ratio) = 0;
    virtual void battleCry() {}

    friend ostream& operator<<(ostream& os, Nobel& nobel) {
        os << nobel.name;
        return os;
    }
};

class Lord : public Nobel {
private:
    vector<Protector*> protectors;
public:
    Lord (const string _name) : Nobel(_name) {}

    void die() {
        dead = true;
    }

    void takeDamage(float ratio) {
        for (size_t i = 0; i < protectors.size(); i++) {
            int newStrength = int(protectors[i]->getStrength() * ratio);
            protectors[i]->setStrength(newStrength);
        }
    }

    void hires(Protector& protector) {
        if (dead || !protector.hire(this)) {
            return;
        }

        protectors.push_back(&protector);
    }
    
    int getStrength() {
        int protectorStrength = 0;
        for (Protector* p : protectors) {
            protectorStrength += p->getStrength();
        }

        return protectorStrength;
    }

    void battleCry() {
        for (Protector* p : protectors) {
            if (!p->isDead()) {
                p->defend();
            }
        }
    }
};

class PersonWithStrengthToFight : public Nobel {
private:
    int strength;
public:
    PersonWithStrengthToFight (const string _name, int _strength) : Nobel(_name), strength(_strength) {}

    void die() {
        dead = true;
        strength = 0;
    }

    void takeDamage(float ratio) {
        strength = int(strength * ratio);
    }

    int getStrength() {
        return strength;
    }
};

class Wizard : public Protector {
private:
public:
    Wizard (const string _name, int _strength) : Protector(_name, _strength) {}

    void defend() {
        cout << "POOF!" << endl;
    }
};

class Warrior : public Protector {
private:
public:
    Warrior (const string _name, int _strength) : Protector(_name, _strength) {}
};

class Archer : public Warrior {
private:
public:
    Archer (const string _name, int _strength) : Warrior(_name, _strength) {}

    void defend() {
        cout << "TWANG! " << name << " says: Take that in the name of my lord, " << *boss << endl;
    }
};
class Swordsman : public Warrior {
private:
public:
    Swordsman (const string _name, int _strength) : Warrior(_name, _strength) {}

    void defend() {
        cout << "CLANG! " << name << " says: Take that in the name of my lord, " << *boss << endl;
    }
};

int main() {
    Lord sam("Sam");
    Archer samantha("Samantha", 200);
    sam.hires(samantha);
    Lord joe("Joe");
    PersonWithStrengthToFight randy("Randolf the Elder", 250);  
    joe.battle(randy);  
    joe.battle(sam);    
    Lord janet("Janet");    
    Swordsman hardy("TuckTuckTheHardy", 100);
    Swordsman stout("TuckTuckTheStout", 80);
    janet.hires(hardy); 
    janet.hires(stout); 
    PersonWithStrengthToFight barclay("Barclay the Bold", 300); 
    janet.battle(barclay);  
    janet.hires(samantha);  
    Archer pethora("Pethora", 50);  
    Archer thora("Thorapleth", 60);
    Wizard merlin("Merlin", 150);
    janet.hires(pethora);
    janet.hires(thora);
    sam.hires(merlin);
    janet.battle(barclay);  
    sam.battle(barclay);    
    joe.battle(barclay);
}
