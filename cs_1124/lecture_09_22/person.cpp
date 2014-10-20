#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Person {
public:
    Person(const string& name): name(name), spouse(nullptr) {}

    void display(ostream& os = cout) const {
        os << name << endl;
        if (spouse != nullptr) {
            os << " is married to " << spouse->name << endl;
        }
    }

    void marries(Person& betrothed) {
        spouse = &betrothed;
        betrothed.spouse = this;
    }
    
private:
    Person* spouse;
    string name;
};

int main() {
    Person john("john");
    Person mary("Mary");

    john.display();
    mary.display();

    int x = 17;
    cout << &x << endl;

    int* p = &x;

    // *OLD*: int* q = NULL;
    int* q = nullptr;
}

