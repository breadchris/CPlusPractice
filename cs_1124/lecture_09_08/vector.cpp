/*
 * Christopher Thompson
 * Collections
 */

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

int print_vector(vector<int> v);

void addOne(int someInt) {
}

int addOneB(int someInt) {
    return someInt++;
}

void addOneRef(int& someInt) {
    someInt++;
}

int main() {
    vector<int> v; // Dynamically allocates memory for elements of collection

    v.push_back(17);
    v.push_back(42);
    v.push_back(5);

    print_vector(v);

    ifstream ifs("numbers");
    if (!ifs) {
        cerr << "Unable to open numbers\n";
        exit(1);
    }

    vector<int> ifs_vector;
    int number;

    while (ifs >> number) {
        ifs_vector.push_back(number);
    }

    print_vector(ifs_vector);

    ifs.close();

    return 0;
}

int print_vector(vector<int> v) {
    for (int i = 0; i < v.size(); i++) {
        cout << v[i] << endl;
    }

    return 0;
}

