/*
 * 1.c
 * Christopher Thompson
 * First program in CS 1124
 */

#include <iostream>
#include <fstream> // File stream library

using namespace std;

int main() {
    cout << "asdf" << endl; // << is an output operator

    int x = 17;             // staticlly typed variables

    if (x == 42) {
        cout << "X is 42";
    } else if (x ==17) {
        cout << "X is 17";
    }

    int i = 20;
    while (i > 0) {
        cout << "i:" << i << endl;
        --i;
    }

    for (int j = 0; j < 10; j++) {
        cout << "j:" << j << endl;
    }

    int k = 10;
    do {
        cout << "k:" << k << endl;
        --k;
    } while (k > 0);

    ifstream ifs;
    ifs.open("numbers");
    ifs >> x;
    cout << "x:" << x << endl;
    ifs.close();

    cout << "===========" << endl;
    ifstream ifs2("numbers"); // Inline constructor instantiation
    string s1;
    while(ifs2 >> s1) {
        cout << "s:" << s1 << endl;
    }
    ifs2.close();

    cout << "===========" << endl;
    ifstream ifs3("numbers"); // Inline constructor instantiation
    string s2;
    while(getline(ifs3, s2)) {
        cout << "s:" << s2 << endl;
    }
    ifs3.close();

    cout << "===========" << endl;
    ifstream ifs4("numbers"); // Inline constructor instantiation
    char s3;
    while(ifs4 >> noskipws >> s3) {
        cout << "s:" << s3 << endl;
    }
    ifs4.close();

    return 0;
}
