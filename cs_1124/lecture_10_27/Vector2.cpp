#include <iostream>
#include <cstdlib>
using namespace std;

/*
  Vector Class
  Demonstrates:
     Good example of copy control
     Dynamic arrays and pointer arithmetic
     Square bracket operator
     Implementation of an important data structure
  Sec A
 */

class Vector {
public:
    Vector() : theSize(0), theCapacity(0), data(nullptr) { }
    Vector(size_t size);

    // Copy control
    ~Vector() { delete [] data; }
    Vector(const Vector& rhs);
    Vector& operator=(const Vector& rhs);

    void push_back(int val) {
	if (theSize == theCapacity) {
	    theCapacity = 2 * theCapacity + 1;
	    int* p = new int[theCapacity];
	    for (size_t index = 0; index < theSize; ++index) {
		p[index] = data[index];
	    }
	    delete [] data;
	    data = p;
	    //	    delete [] p;  // Noooooo!!!!
	}
	// data[theSize] = val;
	// ++theSize;
	data[theSize++] = val;
    }

    size_t size();
    void clear();
    void pop_back();

    // Square brackets?
    int operator[](size_t index) const;
    int& operator[](size_t index);
private:
    size_t theSize;
    size_t theCapacity;
    int* data;
};

void someFunction() {

    Vector v;
    cout << sizeof(v) << endl;


    vector<int> vi(25);
    
    int arr[25];
    cout << sizeof(arr) << endl;
}

int main() {

    Vector v;  // Not templated.  Our Vector class can only hold ints.
    v.push_back(17);
    v.push_back(42);
    v.push_back(6);
    v.push_back(28);

    Vector v2(v);
    Vector v3;
    v3 = v;

    for (size_t i = 0; i < v.size(); ++i) {
        cout << v[i] << endl;
    }
    cerr << "============\n";

    v[0] = 100;

    v.pop_back();

    v = 17;
}

