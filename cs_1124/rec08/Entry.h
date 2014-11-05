#ifndef POSITION_H
#define POSITION_H

#include <string>

namespace BrooklynPoly {
    class Position {
    public:
        Position(const std::string& aTitle, double aSalary)
        : title(aTitle), salary(aSalary) {}
        std::string getTitle() const {return title;}
        double getSalary() const {return salary;}
        void changeSalaryTo(double d) {salary = d;}
    private:
        std::string title;
        double salary;
    }; // class Position
}

#endif
