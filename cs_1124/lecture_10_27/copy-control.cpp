/*
  Copy Control -- Big 3
  Sec A
 */

#include <iostream>
using namespace std;

class Foo {
public:
    Foo(int x) { p = new int(x); }
    void display() const { cout << *p << endl; }
    void setValue(int val) { *p = val; }
    //    void cleanUp() { delete p; }
    ~Foo() { delete p; } // Destructor
    // copy constructor
    Foo(const Foo& someFoo) { p = new int(*someFoo.p); }

    //    aFoo = yetAnotherFoo;
    //    aFoo.operator=(yetAnotherFoo);
    Foo& operator=(const Foo& rhs) {
	if (this !   void setValue(int val) { *p = val; }
    //    void cleanUp() { delete p; }
    ~Foo() { delete p; } // Destructor
    // copy constructor
    Foo(const Foo& someFoo) { p = new int(*someFoo.p); }

    //    aFoo = yetAnotherFoo;
    //    aFoo.operator=(yetAnotherFoo);
    Foo& operator=(const Foo& rhs) {
	if (this != &rhs) { // Not doing self-assignment
	    // Free up the heap for the left-hand-side
	    delete p;
	    // Copy & allocate
	    p = new int(*rhs.p);
	}
	return *this;
    }

private:
    int* p;
};

void doNothing(Foo anotherFoo) { anotherFoo.setValue(42); }

void simpleFunc() {
    Foo aFoo(17);
    aFoo.display();
    //    aFoo.cleanUp();
    doNothing(aFoo);
    aFoo.display();

    Foo yetAnotherFoo(6);

    aFoo = yetAnotherFoo;
    //    aFoo.operator=(yetAnotherFoo);

}

int main() {
    simpleFunc();
}
