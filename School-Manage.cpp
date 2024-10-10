#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>

class Student {
public:
    int id;
    std::string name;
    int age;
    std::string course;
    std::unordered_map<std::string, bool> attendance; // Course name and presence

    Student(int id, std::string name, int age) : id(id), name(name), age(age) {}

    void saveToFile() {
        std::ofstream file("students.txt", std::ios::app);
        if (file.is_open()) {
            file << id << "," << name << "," << age << "," << course << "\n";
            file.close();
        }
    }

    static void loadFromFile(std::vector<Student>& students) {
        std::ifstream file("students.txt");
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string id, name, age, course;
            std::getline(ss, id, ',');
            std::getline(ss, name, ',');
            std::getline(ss, age, ',');
            std::getline(ss, course, ',');
            students.push_back(Student(std::stoi(id), name, std::stoi(age)));
            students.back().course = course;
        }
    }
};

class Teacher {
public:
    int id;
    std::string name;
    std::string subject;

    Teacher(int id, std::string name, std::string subject) : id(id), name(name), subject(subject) {}

    void saveToFile() {
        std::ofstream file("teachers.txt", std::ios::app);
        if (file.is_open()) {
            file << id << "," << name << "," << subject << "\n";
            file.close();
        }
    }

    static void loadFromFile(std::vector<Teacher>& teachers) {
        std::ifstream file("teachers.txt");
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string id, name, subject;
            std::getline(ss, id, ',');
            std::getline(ss, name, ',');
            std::getline(ss, subject, ',');
            teachers.push_back(Teacher(std::stoi(id), name, subject));
        }
    }
};

class Course {
public:
    int id;
    std::string name;
    std::vector<Student> students;
    Teacher *teacher;

    Course(int id, std::string name) : id(id), name(name), teacher(nullptr) {}

    void assignTeacher(Teacher *t) {
        teacher = t;
    }

    void addStudent(Student &s) {
        students.push_back(s);
    }

    void recordAttendance(int studentId, bool isPresent) {
        for (auto &student : students) {
            if (student.id == studentId) {
                student.attendance[name] = isPresent;
            }
        }
    }

    void saveToFile() {
        std::ofstream file("courses.txt", std::ios::app);
        if (file.is_open()) {
            file << id << "," << name << "\n";
            file.close();
        }
    }

    static void loadFromFile(std::vector<Course>& courses) {
        std::ifstream file("courses.txt");
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string id, name;
            std::getline(ss, id, ',');
            std::getline(ss, name, ',');
            courses.push_back(Course(std::stoi(id), name));
        }
    }
};

class Classroom {
public:
    int id;
    int capacity;
    std::vector<Course> courses;

    Classroom(int id, int capacity) : id(id), capacity(capacity) {}

    void addCourse(Course &c) {
        if (courses.size() < capacity) {
            courses.push_back(c);
        } else {
            std::cout << "Classroom capacity reached." << std::endl;
        }
    }

    void saveToFile() {
        std::ofstream file("classrooms.txt", std::ios::app);
        if (file.is_open()) {
            file << id << "," << capacity << "\n";
            file.close();
        }
    }

    static void loadFromFile(std::vector<Classroom>& classrooms) {
        std::ifstream file("classrooms.txt");
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string id, capacity;
            std::getline(ss, id, ',');
            std::getline(ss, capacity, ',');
            classrooms.push_back(Classroom(std::stoi(id), std::stoi(capacity)));
        }
    }
};

void showMenu() {
    std::cout << "1. Add Student\n2. Add Teacher\n3. Add Course\n4. Add Classroom\n5. Assign Teacher to Course\n6. Enroll Student in Course\n7. Record Attendance\n8. Exit\n";
}

int main() {
    std::vector<Student> students;
    std::vector<Teacher> teachers;
    std::vector<Course> courses;
    std::vector<Classroom> classrooms;

    // Load data from files
    Student::loadFromFile(students);
    Teacher::loadFromFile(teachers);
    Course::loadFromFile(courses);
    Classroom::loadFromFile(classrooms);

    int choice;
    do {
        showMenu();
        std::cin >> choice;
        switch (choice) {
            case 1: {
                int id, age;
                std::string name, course;
                std::cout << "Enter Student ID: ";
                std::cin >> id;
                std::cout << "Enter Name: ";
                std::cin >> name;
                std::cout << "Enter Age: ";
                std::cin >> age;
                std::cout << "Enter Course: ";
                std::cin >> course;
                students.push_back(Student(id, name, age));
                students.back().course = course;
                students.back().saveToFile();
                break;
            }
            case 2: {
                int id;
                std::string name, subject;
                std::cout << "Enter Teacher ID: ";
                std::cin >> id;
                std::cout << "Enter Name: ";
                std::cin >> name;
                std::cout << "Enter Subject: ";
                std::cin >> subject;
                teachers.push_back(Teacher(id, name, subject));
                teachers.back().saveToFile();
                break;
            }
            case 3: {
                int id;
                std::string name;
                                std::cout << "Enter Course ID: ";
                std::cin >> id;
                std::cout << "Enter Course Name: ";
                std::cin >> name;
                courses.push_back(Course(id, name));
                courses.back().saveToFile();
                break;
            }
            case 4: {
                int id, capacity;
                std::cout << "Enter Classroom ID: ";
                std::cin >> id;
                std::cout << "Enter Classroom Capacity: ";
                std::cin >> capacity;
                classrooms.push_back(Classroom(id, capacity));
                classrooms.back().saveToFile();
                break;
            }
            case 5: {
                int courseId, teacherId;
                std::cout << "Enter Course ID: ";
                std::cin >> courseId;
                std::cout << "Enter Teacher ID: ";
                std::cin >> teacherId;
                Teacher* teacher = nullptr;
                for (auto& t : teachers) {
                    if (t.id == teacherId) {
                        teacher = &t;
                        break;
                    }
                }
                if (teacher != nullptr) {
                    for (auto& c : courses) {
                        if (c.id == courseId) {
                            c.assignTeacher(teacher);
                            break;
                        }
                    }
                } else {
                    std::cout << "Teacher not found." << std::endl;
                }
                break;
            }
            case 6: {
                int courseId, studentId;
                std::cout << "Enter Course ID: ";
                std::cin >> courseId;
                std::cout << "Enter Student ID: ";
                std::cin >> studentId;
                Student* student = nullptr;
                for (auto& s : students) {
                    if (s.id == studentId) {
                        student = &s;
                        break;
                    }
                }
                if (student != nullptr) {
                    for (auto& c : courses) {
                        if (c.id == courseId) {
                            c.addStudent(*student);
                            break;
                        }
                    }
                } else {
                    std::cout << "Student not found." << std::endl;
                }
                break;
            }
            case 7: {
                int courseId, studentId;
                bool isPresent;
                std::cout << "Enter Course ID: ";
                std::cin >> courseId;
                std::cout << "Enter Student ID: ";
                std::cin >> studentId;
                std::cout << "Is the student present? (1 for yes, 0 for no): ";
                std::cin >> isPresent;
                for (auto& c : courses) {
                    if (c.id == courseId) {
                        c.recordAttendance(studentId, isPresent);
                        break;
                    }
                }
                break;
            }
        }
    } while (choice != 8);

    return 0;
}
