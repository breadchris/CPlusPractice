#include <iostream>
#include <string>

using namespace std;

class Person {
    friend ostream& operator<<(ostream& os, const Person& rhs);
public:
    Person(const string& name, int height) : name(name), height(height) {}

    void display(ostream& os = cout) const {
        os << "Name: " << name << "; Height: " << height << endl;
    }
private:
    string name;
    int height;
};

ostream& operator <<(ostream& os, const Person& rhs) {
    os << "Name: " << rhs.name << "; Height: " << rhs.height << endl;
    return os;
}

int main() {
}
