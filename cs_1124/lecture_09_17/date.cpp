
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

class Date {
public:
    Date(int m, int d, int y): month(m), day(d), year(y) { }
    void display(ostream& os = cout) const {
        os << month << "/" << day << "/" << year;
    }
private:
    int month, day, year;
};

struct Person {
public:
    Person(const string& aName, int age, int m, int d, int y)
        :name(aName), age(age), dob(m, d, y) { }
private:
    string name;
    int age;
    Date dob;
};

