#include "../include/Student.h"
#include "../include/Course.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <regex>

Student::Student(int id, const std::string& name, int age, 
                const std::string& email, const std::string& phone) 
    : id(id), name(name), age(age), email(email), phone(phone) {}

void Student::enrollInCourse(std::shared_ptr<Course> course) {
    if (course && !isEnrolledIn(course->getId())) {
        enrolledCourses.push_back(course);
    }
}

void Student::unenrollFromCourse(int courseId) {
    enrolledCourses.erase(
        std::remove_if(enrolledCourses.begin(), enrolledCourses.end(),
            [courseId](const std::weak_ptr<Course>& course) {
                auto sharedCourse = course.lock();
                return !sharedCourse || sharedCourse->getId() == courseId;
            }),
        enrolledCourses.end()
    );
}

bool Student::isEnrolledIn(int courseId) const {
    return std::any_of(enrolledCourses.begin(), enrolledCourses.end(),
        [courseId](const std::weak_ptr<Course>& course) {
            auto sharedCourse = course.lock();
            return sharedCourse && sharedCourse->getId() == courseId;
        });
}

void Student::markAttendance(const std::string& courseName, bool isPresent) {
    attendance[courseName] = isPresent;
}

bool Student::getAttendance(const std::string& courseName) const {
    auto it = attendance.find(courseName);
    return it != attendance.end() ? it->second : false;
}

double Student::getAttendancePercentage(const std::string& courseName) const {
    // Simplified implementation - in real system would track multiple attendance records
    return getAttendance(courseName) ? 100.0 : 0.0;
}

void Student::saveToFile(const std::string& filename) const {
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        file << id << "," << name << "," << age << "," << email << "," << phone << "\n";
        file.close();
    }
}

std::vector<Student> Student::loadFromFile(const std::string& filename) {
    std::vector<Student> students;
    std::ifstream file(filename);
    std::string line;
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string id, name, age, email, phone;
        
        std::getline(ss, id, ',');
        std::getline(ss, name, ',');
        std::getline(ss, age, ',');
        std::getline(ss, email, ',');
        std::getline(ss, phone, ',');
        
        if (!id.empty() && !name.empty() && !age.empty()) {
            students.emplace_back(std::stoi(id), name, std::stoi(age), email, phone);
        }
    }
    
    return students;
}

std::string Student::toString() const {
    return "Student[ID: " + std::to_string(id) + ", Name: " + name + 
           ", Age: " + std::to_string(age) + "]";
}

void Student::displayInfo() const {
    std::cout << "📚 " << toString() << std::endl;
    if (!email.empty()) std::cout << "   📧 Email: " << email << std::endl;
    if (!phone.empty()) std::cout << "   📞 Phone: " << phone << std::endl;
}

bool Student::isValidId(int id) {
    return id > 0;
}

bool Student::isValidAge(int age) {
    return age >= 16 && age <= 100;
}

bool Student::isValidEmail(const std::string& email) {
    if (email.empty()) return true; // Email is optional
    std::regex emailPattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    return std::regex_match(email, emailPattern);
}
