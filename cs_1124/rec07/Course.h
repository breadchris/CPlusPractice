#ifndef COURSE_H
#define COURSE_H

#include <vector>
#include <string>

namespace BrooklynPoly {
    class Student;

    class Course {
    private:
        std::string name;
        std::vector<Student*> students;
    public:
        Course (const std::string& _name);

        void dropStudents();

        void addStudent(Student* student);

        void dropStudent(Student* student);

        std::string getName();

        void display();
    };
}

#endif
