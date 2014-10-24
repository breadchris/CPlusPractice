#include <iostream>
#include <vector>

#include "Registrar.h"
#include "Course.h"
#include "Student.h"

using namespace std;

namespace BrooklynPoly {
	class Student;
	class Course;

	Registrar::Registrar() {}

	ostream& operator << (ostream& os, Registrar& registrar) {
        cout << "Students: " << endl;

        vector<Student*> students = registrar.getStudents();

        if (students.size() == 0) {
            cout << "none" << endl;
        }

        for (Student* s : students) {
            s->display();
        }

        cout << "\nCourses: " << endl;

        vector<Course*> courses = registrar.getCourses();

        if (courses.size() == 0) {
            cout << "none" << endl;
        }

        for (Course* c : courses) {
            c->display();
        }

        return os;
	}

	void Registrar::purge() {
        for (Student* s : students) {
            if (s != nullptr) {
                delete s;
            }
        }

        for (Course* c : courses) {
            if (c != nullptr) {
                delete c;
            }
        }

        students.clear();
        courses.clear();
	}

	void Registrar::addCourse(const std::string& courseName) {
        Course* c = new Course(courseName);
        courses.push_back(c);
	}

	void Registrar::cancelCourse(const std::string& courseName) {
        Course* course = nullptr;

        for (size_t i = 0; i < courses.size(); i++) {
            if (courses[i]->getName() == courseName) {
                course = courses[i];
            }
        }

        if (course != nullptr) {
            course->dropStudents();
        } else {
            cerr << "Unable to find course when cancelling course" << endl;
        }
	}

	void Registrar::addStudent(const std::string& studentName) {
        Student* s = new Student(studentName);
        students.push_back(s);
	}

	void Registrar::enrollStudentInCourse(const std::string& studentName, const std::string& courseName) {
        Student* student = nullptr;
        Course* course = nullptr;

        for (size_t i = 0; i < students.size(); i++) {
            if (students[i]->getName() == studentName) {
                student = students[i];
            }
        }

        for (size_t i = 0; i < courses.size(); i++) {
            if (courses[i]->getName() == courseName) {
                course = courses[i];
            }
        }

        if (student != nullptr && course != nullptr) {
            student->addCourse(course);
            course->addStudent(student);
        } else {
            cerr << "Unable to find student or course when enrolling student" << endl;
        }
	}

	vector<Student*> Registrar::getStudents() const {
        return this->students;
	}

	vector<Course*> Registrar::getCourses() const {
        return this->courses;
	}
}

