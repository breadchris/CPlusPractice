#include <iostream>
#include <string>
#include <vector>

using namespace std;

class TimeSlot {
public:
    TimeSlot(const string& _day, int _time) : day(_day), time(_time) {}

    void display() {
        cout << day << " " << time << endl;
    }
private:
    string day;
    int time;
};

class Student {
public:
    Student(const string& _name) : name(_name), grades(13, -1) {}

    string getName() {
        return name;
    }

    void setGrade(int grade, int week) {
        grades[week] = grade;
    }

    void display() {
        cout << "Student: " << name << "; Grades: ";
        for (int i = 0; i < grades.size(); i++) {
            cout << grades[i] << " ";
        }
        cout << "\n";
    }

private:
    string name;
    vector<int> grades;
};

class Section {
public:
    Section(const string& _section, const string& _day, int _time) : section(_section), timeslot(TimeSlot(_day, _time)) {}

    void addGrade(const string& studentName, int grade, int week) {
        for (int i = 0; i < students.size(); i++) {
            if (students[i]->getName() == studentName) {
                students[i]->setGrade(grade, week);
            }
        }
    }

    void addStudent(const string& name) {
        Student* student = new Student(name);
        students.push_back(student);
    }

    void display() {
        cout << "Section: " << section << "; ";
        timeslot.display();
    }

    void displayStudents() {
        for (int i = 0; i < students.size(); i++) {
            students[i]->display();
        }
    }

    void reset() {
        for (int i = 0; i < students.size(); i++) {
            delete students[i];
        }

        students.clear();
    }

private:
    string section;
    TimeSlot timeslot;
    vector<Student*> students;
};

class LabWorker {
public:
    LabWorker(const string& _name) : name(_name), section(nullptr) {}

    void addSection(Section& _section) {
        *section = _section;
    }

    void addGrade(const string& studentName, int grade, int week) {
        section->addGrade(studentName, grade, week);
    }

    void displayGrades() {
        cout << name << " has ";
        section->display();
    }

private:
    string name;
    Section* section;
};

int main() {

   // lab workers
   LabWorker moe( "Moe" );
   LabWorker jane( "Jane" );
   
   // sections and setup and testing
   Section secA2( "A2", "Tuesday", 16 );
   //secA2.loadStudentsFromFile( "A2.txt" );
   secA2.addStudent("John");
   secA2.addStudent("George");
   secA2.addStudent("Paul");
   secA2.addStudent("Ringo");

   cout << "\ntest A2\n";    // here the modeler-programmer checks that load worked
   secA2.display();          // YOU'll DO THIS LATER AS: cout << secA2 << endl;
   moe.addSection( secA2 );
   moe.displayGrades();       // here the modeler-programmer checks that adding the Section worked
  
   Section secB3( "B3", "Thursday", 11 );
   //secB3.loadStudentsFromFile( "B3.txt" );
   secB3.addStudent("Thorin");
   secB3.addStudent("Dwalin");
   secB3.addStudent("Balin");
   secB3.addStudent("Kili");
   secB3.addStudent("Fili");
   secB3.addStudent("Dori");
   secB3.addStudent("Nori");
   secB3.addStudent("Ori");
   secB3.addStudent("Oin");
   secB3.addStudent("Gloin");
   secB3.addStudent("Bifur");
   secB3.addStudent("Bofur");
   secB3.addStudent("Bombur");

   cout << "\ntest B3\n";    // here the modeler-programmer checks that load worked
   secB3.display();          // YOU'll DO THIS LATER AS: cout << secB3 << endl;
   jane.addSection( secB3 );
   jane.displayGrades();      // here the modeler-programmer checks that adding Instructor worked


   // setup is complete, now a real world scenario can be written in the code
   // [NOTE: the modeler-programmer is only modeling joe's actions for the rest of the program]

   // week one activities  
   cout << "\nModeling week: 1\n";
   moe.addGrade( "John", 7, 1 );  
   moe.addGrade( "Paul", 9, 1 );  
   moe.addGrade( "George", 7, 1 );  
   moe.addGrade( "Ringo", 7, 1 );  
   cout << "End of week one\n";
   moe.displayGrades();

   // week two activities
   cout << "\nModeling week: 2\n";
   moe.addGrade( "John", 5, 2 );  
   moe.addGrade( "Paul", 10, 2 );  
   moe.addGrade( "Ringo", 0, 2 );  
   cout << "End of week two\n";
   moe.displayGrades();

   //test that reset works  // NOTE: can we check that the heap data was dealt with?
   cout << "\ntesting reset()\n";
   secA2.reset();
   secA2.display();
   moe.displayGrades();

} // main
