#include "../include/Teacher.h"
#include "../include/Course.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <regex>

Teacher::Teacher(int id, const std::string& name, const std::string& subject,
                const std::string& email, const std::string& phone,
                const std::string& department)
    : id(id), name(name), subject(subject), email(email), phone(phone), department(department) {}

void Teacher::assignToCourse(std::shared_ptr<Course> course) {
    if (course && !isAssignedTo(course->getId())) {
        assignedCourses.push_back(course);
    }
}

void Teacher::unassignFromCourse(int courseId) {
    assignedCourses.erase(
        std::remove_if(assignedCourses.begin(), assignedCourses.end(),
            [courseId](const std::weak_ptr<Course>& course) {
                auto sharedCourse = course.lock();
                return !sharedCourse || sharedCourse->getId() == courseId;
            }),
        assignedCourses.end()
    );
}

bool Teacher::isAssignedTo(int courseId) const {
    return std::any_of(assignedCourses.begin(), assignedCourses.end(),
        [courseId](const std::weak_ptr<Course>& course) {
            auto sharedCourse = course.lock();
            return sharedCourse && sharedCourse->getId() == courseId;
        });
}

void Teacher::saveToFile(const std::string& filename) const {
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        file << id << "," << name << "," << subject << "," << email << "," 
             << phone << "," << department << "\n";
        file.close();
    }
}

std::vector<Teacher> Teacher::loadFromFile(const std::string& filename) {
    std::vector<Teacher> teachers;
    std::ifstream file(filename);
    std::string line;
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string id, name, subject, email, phone, department;
        
        std::getline(ss, id, ',');
        std::getline(ss, name, ',');
        std::getline(ss, subject, ',');
        std::getline(ss, email, ',');
        std::getline(ss, phone, ',');
        std::getline(ss, department, ',');
        
        if (!id.empty() && !name.empty() && !subject.empty()) {
            teachers.emplace_back(std::stoi(id), name, subject, email, phone, department);
        }
    }
    
    return teachers;
}

std::string Teacher::toString() const {
    return "Teacher[ID: " + std::to_string(id) + ", Name: " + name + 
           ", Subject: " + subject + "]";
}

void Teacher::displayInfo() const {
    std::cout << "👩‍🏫 " << toString() << std::endl;
    if (!department.empty()) std::cout << "   🏢 Department: " << department << std::endl;
    if (!email.empty()) std::cout << "   📧 Email: " << email << std::endl;
    if (!phone.empty()) std::cout << "   📞 Phone: " << phone << std::endl;
}

bool Teacher::isValidId(int id) {
    return id > 0;
}

bool Teacher::isValidEmail(const std::string& email) {
    if (email.empty()) return true; // Email is optional
    std::regex emailPattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    return std::regex_match(email, emailPattern);
}
