#ifndef STUDENT_H
#define STUDENT_H

#include <vector>
#include <string>

namespace BrooklynPoly {
	class Course;

	class Student {
	private:
		std::string name;
		std::vector<Course*> courses;
	public:
	    Student (const std::string& _name);

	    void addCourse(Course* course);

	    void dropCourse(Course* course);

        std::string getName();

	    void display();
	};
}

#endif
