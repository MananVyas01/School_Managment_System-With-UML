#include "../include/Course.h"
#include "../include/Student.h"
#include "../include/Teacher.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

Course::Course(int id, const std::string& name, const std::string& description, 
               int credits, int maxStudents)
    : id(id), name(name), description(description), credits(credits), maxStudents(maxStudents) {}

void Course::assignTeacher(std::shared_ptr<Teacher> teacher) {
    assignedTeacher = teacher;
}

void Course::unassignTeacher() {
    assignedTeacher = nullptr;
}

bool Course::enrollStudent(std::shared_ptr<Student> student) {
    if (!student || isStudentEnrolled(student->getId()) || isFull()) {
        return false;
    }
    
    enrolledStudents.push_back(student);
    return true;
}

bool Course::unenrollStudent(int studentId) {
    auto it = std::find_if(enrolledStudents.begin(), enrolledStudents.end(),
        [studentId](const std::weak_ptr<Student>& student) {
            auto sharedStudent = student.lock();
            return sharedStudent && sharedStudent->getId() == studentId;
        });
    
    if (it != enrolledStudents.end()) {
        enrolledStudents.erase(it);
        return true;
    }
    return false;
}

bool Course::isStudentEnrolled(int studentId) const {
    return std::any_of(enrolledStudents.begin(), enrolledStudents.end(),
        [studentId](const std::weak_ptr<Student>& student) {
            auto sharedStudent = student.lock();
            return sharedStudent && sharedStudent->getId() == studentId;
        });
}

void Course::recordAttendance(int studentId, const std::string& date, bool isPresent) {
    attendanceRecords[studentId][date] = isPresent;
}

bool Course::getAttendance(int studentId, const std::string& date) const {
    auto studentIt = attendanceRecords.find(studentId);
    if (studentIt != attendanceRecords.end()) {
        auto dateIt = studentIt->second.find(date);
        if (dateIt != studentIt->second.end()) {
            return dateIt->second;
        }
    }
    return false;
}

double Course::getStudentAttendancePercentage(int studentId) const {
    auto studentIt = attendanceRecords.find(studentId);
    if (studentIt == attendanceRecords.end() || studentIt->second.empty()) {
        return 0.0;
    }
    
    int totalDays = studentIt->second.size();
    int presentDays = 0;
    
    for (const auto& record : studentIt->second) {
        if (record.second) presentDays++;
    }
    
    return (static_cast<double>(presentDays) / totalDays) * 100.0;
}

std::unordered_map<int, double> Course::getAllStudentAttendancePercentages() const {
    std::unordered_map<int, double> percentages;
    
    for (const auto& studentRecord : attendanceRecords) {
        percentages[studentRecord.first] = getStudentAttendancePercentage(studentRecord.first);
    }
    
    return percentages;
}

void Course::saveToFile(const std::string& filename) const {
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        file << id << "," << name << "," << description << "," 
             << credits << "," << maxStudents << "\n";
        file.close();
    }
}

std::vector<Course> Course::loadFromFile(const std::string& filename) {
    std::vector<Course> courses;
    std::ifstream file(filename);
    std::string line;
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string id, name, description, credits, maxStudents;
        
        std::getline(ss, id, ',');
        std::getline(ss, name, ',');
        std::getline(ss, description, ',');
        std::getline(ss, credits, ',');
        std::getline(ss, maxStudents, ',');
        
        if (!id.empty() && !name.empty()) {
            int courseCredits = credits.empty() ? 3 : std::stoi(credits);
            int courseMaxStudents = maxStudents.empty() ? 50 : std::stoi(maxStudents);
            courses.emplace_back(std::stoi(id), name, description, courseCredits, courseMaxStudents);
        }
    }
    
    return courses;
}

std::string Course::toString() const {
    return "Course[ID: " + std::to_string(id) + ", Name: " + name + 
           ", Credits: " + std::to_string(credits) + ", Enrolled: " + 
           std::to_string(getCurrentStudentsCount()) + "/" + std::to_string(maxStudents) + "]";
}

void Course::displayInfo() const {
    std::cout << "📚 " << toString() << std::endl;
    if (!description.empty()) std::cout << "   📄 Description: " << description << std::endl;
    if (hasTeacher()) {
        std::cout << "   👩‍🏫 Teacher: " << assignedTeacher->getName() << std::endl;
    }
}

void Course::displayStudentList() const {
    std::cout << "📋 Students enrolled in " << name << ":\n";
    for (const auto& studentWeakPtr : enrolledStudents) {
        auto student = studentWeakPtr.lock();
        if (student) {
            std::cout << "  • " << student->getName() << " (ID: " << student->getId() << ")\n";
        }
    }
}

void Course::displayAttendanceReport() const {
    std::cout << "📊 Attendance Report for " << name << ":\n";
    for (const auto& record : attendanceRecords) {
        std::cout << "  Student ID " << record.first << ": " 
                  << getStudentAttendancePercentage(record.first) << "%\n";
    }
}

bool Course::isValidId(int id) {
    return id > 0;
}

bool Course::isValidCredits(int credits) {
    return credits > 0 && credits <= 10;
}
