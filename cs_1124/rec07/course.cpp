#include <iostream>

#include "Registrar.h"
#include "Course.h"
#include "Student.h"

using namespace std;

namespace BrooklynPoly {
	class Student;
	class Course;

	Course::Course(const string& _name) : name(_name) {}

	void Course::dropStudents() {
		for (size_t i = 0; i < students.size(); i++) {
			students[i]->dropCourse(this);
		}
		students.clear();
	}

	void Course::addStudent(Student* student) {
		if (student != nullptr) {
			students.push_back(student);
		} else {
			cerr << "Course given null student to add" << endl;
		}
	}

	void Course::dropStudent(Student* student) {
		if (student != nullptr) {
			for (size_t i = 0; i < students.size(); i++) {
				if (students[i] == student) {
					students.erase(students.begin() + i);
					break;
				}
			}
		} else {
			cerr << "Course given null student to drop" << endl;
		}
	}

    string Course::getName() {
        return name;
    }

	void Course::display() {
		cout << "Course: " << name << endl;
	}
}
