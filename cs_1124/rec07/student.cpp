#include <iostream>

#include "Registrar.h"
#include "Course.h"
#include "Student.h"

using namespace std;

namespace BrooklynPoly {
	class Course;

	Student::Student(const string& _name) : name(_name) {}

	void Student::addCourse(Course* course) {
		if (course != nullptr) {
			courses.push_back(course);
		} else {
			cerr << "Student given null course to join" << endl;
		}
	}

	void Student::dropCourse(Course* course) {
		if (course != nullptr) {
			for (size_t i = 0; i < courses.size(); i++) {
				if (courses[i] == course) {
					courses.erase(courses.begin() + i);
					break;
				}
			}
		} else {
			cerr << "Student given null course to drop" << endl;
		}
	}

    string Student::getName() {
        return name;
    }

	void Student::display() {
		cout << "Student: " << name << endl;
	}
}
