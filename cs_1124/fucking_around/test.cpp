#include <iostream>

using namespace std;

class Base {
public:
    Base() {}

    Base operator=(const Base& b) {}
private:
};

class Derived : public Base {
public:
    Derived() {}
private:
};

int main() {
    // Protected is restricted to the class
    return 0;
}

