#include <iostream>

using namespace std;

class Pet {
public:
    Pet (const string& theName) : name(theName) {}
    void display() const { cout << "Pet" << endl; }
private:
    string name;
};

class Cat : public Pet {
public:
private:
};

class Slug : public Pet {
public:
private:
};

class Roach : public Pet {
public:
private:
};

void foo(const Pet& aPet) {
    aPet.display();
}

int main() {
    Pet peeve;
    peeve.display();

    cerr << "=======================\n";

}

