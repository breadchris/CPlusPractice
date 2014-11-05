/*
  Recitation 09
  CS1124
 
  Focus: Copy Control.  Uses dynamic array of pointers.
 */

#include <string>
#include <iostream>
using namespace std;

class Entry {
public:
    Entry(const string& aName, unsigned aRoom, unsigned aPhone,
	  Position& aPosition) 
	: name(aName), room(aRoom), phone(aPhone), pos(&aPosition) {
    }
    friend ostream& operator<<( ostream& os, const Entry& e ) {
        os << e.name << ' ' << e.room << ' ' << e.phone;
        return os;
    } // operato<<

    string getName() const {
        return name;
    }

    unsigned getNumber() const {
        return phone;
    }

    unsigned getRoom() const {
        return room;
    }

    Position* getPosition() {
        return pos;
    }

private:
    string name;
    unsigned room;
    unsigned phone;
    Position* pos;
}; // class Entry


void doNothing(Directory dir) { cout << dir << endl; }

int main() {
	
    // Model as if there are these four kinds 
    // of position in the problem:
    Position boss("Boss", 3141.59);
    Position pointyHair("Pointy Hair", 271.83);
    Position techie("Techie", 14142.13);
    Position peon("Peonissimo", 34.79);
	
    // Create a Directory
    Directory d;
    d.add("Marilyn", 123, 4567, boss);
    cout << d << endl;

    Directory d2 = d;	// What function is being used??
    d2.add("Gallagher", 111, 2222, techie);
    d2.add("Carmack", 314, 1592, techie);

    cout << "Calling doNothing\n";
    doNothing(d2);
    cout << "Back from doNothing\n";

    Directory d3;
    d3 = d2;

    cout << d3 << endl;

    cout << "Search for number given name" << endl;

    cout << d2["Gallagher"] << endl;
    cout << d2["Carmack"] << endl;
    cout << d2["Carmk"] << endl;
	
} // main
