#include <iostream>

using namespace std;

class Foo {
public:
    Foo(int _p) : p(new int(_p)) {}

    void display() const { cout << *p << endl; }
    void setValue(int val) { *p = val; }
    void cleanUp() { delete p; }

    ~Foo() { delete p; }
    Foo(Foo& someFoo) { p = new int(*someFoo.p); }
private:
    int* p;
};

void doNothing(Foo anotherFoo) {}

void simpleFunc() {
    Foo aFoo(17);
    aFoo.display();
    aFoo.cleanUp();
}

int main() {
    simpleFunc();
}

