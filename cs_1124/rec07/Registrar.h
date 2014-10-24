#ifndef REGISTRAR_H
#define REGISTRAR_H

#include <iostream>
#include <vector>
#include <string>

namespace BrooklynPoly {
    class Student;
    class Course;

    class Registrar {
    private:
        std::vector<Student*> students;
        std::vector<Course*> courses;
    public:
        Registrar ();

        void purge();

        void addCourse(const std::string& courseName);

        void cancelCourse(const std::string& courseName);

        void addStudent(const std::string& studentName);

        void enrollStudentInCourse(const std::string& studentName, const std::string& courseName);

        std::vector<Student*> getStudents() const;
        
        std::vector<Course*> getCourses() const;

        friend std::ostream& operator << (std::ostream& os, Registrar& registrar);
    };
}

#endif

