#ifndef SCHOOL_MANAGEMENT_SYSTEM_H
#define SCHOOL_MANAGEMENT_SYSTEM_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <regex>
#include <iomanip>
#include <limits>
#include <stdexcept>
#include <chrono>
#include <ctime>

// Forward declarations
class Course;
class Teacher;

// Utility class for input validation and handling
class InputValidator {
public:
    static bool isValidId(int id) {
        return id > 0 && id <= 999999;
    }
    
    static bool isValidAge(int age) {
        return age >= 16 && age <= 100;
    }
    
    static bool isValidEmail(const std::string& email) {
        if (email.empty()) return true; // Optional field
        std::regex emailPattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
        return std::regex_match(email, emailPattern);
    }
    
    static bool isValidPhone(const std::string& phone) {
        if (phone.empty()) return true; // Optional field
        std::regex phonePattern(R"(\+?[\d\s\-\(\)]{10,15})");
        return std::regex_match(phone, phonePattern);
    }
    
    static bool isValidName(const std::string& name) {
        return !name.empty() && name.length() <= 100 && 
               std::all_of(name.begin(), name.end(), [](char c) {
                   return std::isalnum(c) || std::isspace(c) || c == '.' || c == '-' || c == '\'';
               });
    }
    
    static std::string sanitizeString(const std::string& input) {
        std::string result = input;
        // Remove leading/trailing whitespace
        result.erase(0, result.find_first_not_of(" \t\n\r"));
        result.erase(result.find_last_not_of(" \t\n\r") + 1);
        // Replace multiple spaces with single space
        std::regex multipleSpaces("\\s+");
        result = std::regex_replace(result, multipleSpaces, " ");
        return result;
    }
};

// Safe input handling class
class SafeInput {
public:
    static int getInt(const std::string& prompt, int min = INT_MIN, int max = INT_MAX) {
        int value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value) {
                if (value >= min && value <= max) {
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    return value;
                } else {
                    std::cout << "❌ Value must be between " << min << " and " << max << ". Try again.\n";
                }
            } else {
                std::cout << "❌ Invalid input. Please enter a valid number.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }
    
    static std::string getString(const std::string& prompt, bool required = true, size_t maxLength = 100) {
        std::string value;
        while (true) {
            std::cout << prompt;
            std::getline(std::cin, value);
            value = InputValidator::sanitizeString(value);
            
            if (!required && value.empty()) {
                return value;
            }
            
            if (value.empty() && required) {
                std::cout << "❌ This field is required. Please enter a value.\n";
                continue;
            }
            
            if (value.length() > maxLength) {
                std::cout << "❌ Input too long. Maximum " << maxLength << " characters allowed.\n";
                continue;
            }
            
            return value;
        }
    }
    
    static bool getYesNo(const std::string& prompt) {
        std::string input;
        while (true) {
            std::cout << prompt << " (y/n): ";
            std::getline(std::cin, input);
            std::transform(input.begin(), input.end(), input.begin(), ::tolower);
            
            if (input == "y" || input == "yes") return true;
            if (input == "n" || input == "no") return false;
            
            std::cout << "❌ Please enter 'y' for yes or 'n' for no.\n";
        }
    }
    
    static void waitForEnter(const std::string& message = "Press Enter to continue...") {
        std::cout << message;
        std::cin.get();
    }
};

// Enhanced Student class
class Student {
private:
    int id;
    std::string name;
    int age;
    std::string email;
    std::string phone;
    std::string address;
    std::string enrollmentDate;
    std::vector<int> enrolledCourses;
    std::unordered_map<std::string, std::vector<bool>> attendance; // Course -> attendance records
    bool isActive;
    
public:
    Student(int id, const std::string& name, int age, const std::string& email = "", 
            const std::string& phone = "", const std::string& address = "")
        : id(id), name(name), age(age), email(email), phone(phone), address(address), isActive(true) {
        enrollmentDate = getCurrentDate();
    }
    
    // Getters
    int getId() const { return id; }
    const std::string& getName() const { return name; }
    int getAge() const { return age; }
    const std::string& getEmail() const { return email; }
    const std::string& getPhone() const { return phone; }
    const std::string& getAddress() const { return address; }
    const std::string& getEnrollmentDate() const { return enrollmentDate; }
    const std::vector<int>& getEnrolledCourses() const { return enrolledCourses; }
    bool getIsActive() const { return isActive; }
    
    // Setters with validation
    bool setName(const std::string& newName) {
        if (InputValidator::isValidName(newName)) {
            name = InputValidator::sanitizeString(newName);
            return true;
        }
        return false;
    }
    
    bool setAge(int newAge) {
        if (InputValidator::isValidAge(newAge)) {
            age = newAge;
            return true;
        }
        return false;
    }
    
    bool setEmail(const std::string& newEmail) {
        if (InputValidator::isValidEmail(newEmail)) {
            email = InputValidator::sanitizeString(newEmail);
            return true;
        }
        return false;
    }
    
    bool setPhone(const std::string& newPhone) {
        if (InputValidator::isValidPhone(newPhone)) {
            phone = InputValidator::sanitizeString(newPhone);
            return true;
        }
        return false;
    }
    
    void setAddress(const std::string& newAddress) {
        address = InputValidator::sanitizeString(newAddress);
    }
    
    void setIsActive(bool active) { isActive = active; }
    
    // Course management
    bool enrollInCourse(int courseId) {
        if (std::find(enrolledCourses.begin(), enrolledCourses.end(), courseId) == enrolledCourses.end()) {
            enrolledCourses.push_back(courseId);
            return true;
        }
        return false;
    }
    
    bool unenrollFromCourse(int courseId) {
        auto it = std::find(enrolledCourses.begin(), enrolledCourses.end(), courseId);
        if (it != enrolledCourses.end()) {
            enrolledCourses.erase(it);
            return true;
        }
        return false;
    }
    
    bool isEnrolledIn(int courseId) const {
        return std::find(enrolledCourses.begin(), enrolledCourses.end(), courseId) != enrolledCourses.end();
    }
    
    // Attendance management
    void markAttendance(const std::string& courseName, bool present) {
        attendance[courseName].push_back(present);
    }
    
    double getAttendancePercentage(const std::string& courseName) const {
        auto it = attendance.find(courseName);
        if (it == attendance.end() || it->second.empty()) return 0.0;
        
        int present = std::count(it->second.begin(), it->second.end(), true);
        return (static_cast<double>(present) / it->second.size()) * 100.0;
    }
    
    // File operations with error handling
    bool saveToFile(const std::string& filename) const {
        std::ofstream file(filename, std::ios::app);
        if (!file.is_open()) {
            std::cerr << "❌ Error: Cannot open file " << filename << " for writing.\n";
            return false;
        }
        
        file << id << "|" << name << "|" << age << "|" << email << "|" 
             << phone << "|" << address << "|" << enrollmentDate << "|" 
             << (isActive ? "1" : "0") << "|";
        
        // Save enrolled courses
        for (size_t i = 0; i < enrolledCourses.size(); ++i) {
            file << enrolledCourses[i];
            if (i < enrolledCourses.size() - 1) file << ",";
        }
        file << "\n";
        
        file.close();
        return true;
    }
    
    static std::vector<Student> loadFromFile(const std::string& filename) {
        std::vector<Student> students;
        std::ifstream file(filename);
        
        if (!file.is_open()) {
            std::cout << "ℹ️  File " << filename << " not found. Starting with empty student list.\n";
            return students;
        }
        
        std::string line;
        int lineNumber = 0;
        
        while (std::getline(file, line)) {
            lineNumber++;
            if (line.empty()) continue;
            
            try {
                std::vector<std::string> parts = split(line, '|');
                if (parts.size() >= 8) {
                    Student student(std::stoi(parts[0]), parts[1], std::stoi(parts[2]), 
                                  parts[3], parts[4], parts[5]);
                    student.enrollmentDate = parts[6];
                    student.isActive = (parts[7] == "1");
                    
                    // Load enrolled courses
                    if (parts.size() > 8 && !parts[8].empty()) {
                        std::vector<std::string> courseIds = split(parts[8], ',');
                        for (const auto& courseId : courseIds) {
                            if (!courseId.empty()) {
                                student.enrolledCourses.push_back(std::stoi(courseId));
                            }
                        }
                    }
                    
                    students.push_back(student);
                }
            } catch (const std::exception& e) {
                std::cerr << "⚠️  Warning: Error parsing line " << lineNumber 
                         << " in " << filename << ": " << e.what() << "\n";
            }
        }
        
        file.close();
        std::cout << "✅ Loaded " << students.size() << " students from " << filename << "\n";
        return students;
    }
    
    // Display methods
    void displayInfo() const {
        std::cout << "👨‍🎓 Student Information:\n";
        std::cout << "   ID: " << id << "\n";
        std::cout << "   Name: " << name << "\n";
        std::cout << "   Age: " << age << "\n";
        std::cout << "   Email: " << (email.empty() ? "Not provided" : email) << "\n";
        std::cout << "   Phone: " << (phone.empty() ? "Not provided" : phone) << "\n";
        std::cout << "   Address: " << (address.empty() ? "Not provided" : address) << "\n";
        std::cout << "   Enrollment Date: " << enrollmentDate << "\n";
        std::cout << "   Status: " << (isActive ? "Active" : "Inactive") << "\n";
        std::cout << "   Enrolled Courses: " << enrolledCourses.size() << "\n";
    }
    
    void displaySummary() const {
        std::cout << std::setw(5) << id << " | " 
                  << std::setw(20) << name.substr(0, 20) << " | "
                  << std::setw(3) << age << " | "
                  << std::setw(25) << email.substr(0, 25) << " | "
                  << std::setw(8) << (isActive ? "Active" : "Inactive") << "\n";
    }
    
private:
    static std::string getCurrentDate() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d");
        return ss.str();
    }
    
    static std::vector<std::string> split(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;
        while (std::getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }
};

// Enhanced Teacher class
class Teacher {
private:
    int id;
    std::string name;
    std::string subject;
    std::string email;
    std::string phone;
    std::string department;
    std::string hireDate;
    std::vector<int> assignedCourses;
    bool isActive;
    double salary;
    
public:
    Teacher(int id, const std::string& name, const std::string& subject,
            const std::string& email = "", const std::string& phone = "",
            const std::string& department = "", double salary = 0.0)
        : id(id), name(name), subject(subject), email(email), phone(phone), 
          department(department), isActive(true), salary(salary) {
        hireDate = getCurrentDate();
    }
    
    // Getters
    int getId() const { return id; }
    const std::string& getName() const { return name; }
    const std::string& getSubject() const { return subject; }
    const std::string& getEmail() const { return email; }
    const std::string& getPhone() const { return phone; }
    const std::string& getDepartment() const { return department; }
    const std::string& getHireDate() const { return hireDate; }
    const std::vector<int>& getAssignedCourses() const { return assignedCourses; }
    bool getIsActive() const { return isActive; }
    double getSalary() const { return salary; }
    
    // Setters with validation
    bool setName(const std::string& newName) {
        if (InputValidator::isValidName(newName)) {
            name = InputValidator::sanitizeString(newName);
            return true;
        }
        return false;
    }
    
    bool setSubject(const std::string& newSubject) {
        if (!newSubject.empty() && newSubject.length() <= 100) {
            subject = InputValidator::sanitizeString(newSubject);
            return true;
        }
        return false;
    }
    
    bool setEmail(const std::string& newEmail) {
        if (InputValidator::isValidEmail(newEmail)) {
            email = InputValidator::sanitizeString(newEmail);
            return true;
        }
        return false;
    }
    
    bool setPhone(const std::string& newPhone) {
        if (InputValidator::isValidPhone(newPhone)) {
            phone = InputValidator::sanitizeString(newPhone);
            return true;
        }
        return false;
    }
    
    void setDepartment(const std::string& newDepartment) {
        department = InputValidator::sanitizeString(newDepartment);
    }
    
    void setIsActive(bool active) { isActive = active; }
    
    bool setSalary(double newSalary) {
        if (newSalary >= 0) {
            salary = newSalary;
            return true;
        }
        return false;
    }
    
    // Course management
    bool assignToCourse(int courseId) {
        if (std::find(assignedCourses.begin(), assignedCourses.end(), courseId) == assignedCourses.end()) {
            assignedCourses.push_back(courseId);
            return true;
        }
        return false;
    }
    
    bool unassignFromCourse(int courseId) {
        auto it = std::find(assignedCourses.begin(), assignedCourses.end(), courseId);
        if (it != assignedCourses.end()) {
            assignedCourses.erase(it);
            return true;
        }
        return false;
    }
    
    bool isAssignedTo(int courseId) const {
        return std::find(assignedCourses.begin(), assignedCourses.end(), courseId) != assignedCourses.end();
    }
    
    // File operations
    bool saveToFile(const std::string& filename) const {
        std::ofstream file(filename, std::ios::app);
        if (!file.is_open()) {
            std::cerr << "❌ Error: Cannot open file " << filename << " for writing.\n";
            return false;
        }
        
        file << id << "|" << name << "|" << subject << "|" << email << "|"
             << phone << "|" << department << "|" << hireDate << "|"
             << (isActive ? "1" : "0") << "|" << salary << "|";
        
        // Save assigned courses
        for (size_t i = 0; i < assignedCourses.size(); ++i) {
            file << assignedCourses[i];
            if (i < assignedCourses.size() - 1) file << ",";
        }
        file << "\n";
        
        file.close();
        return true;
    }
    
    static std::vector<Teacher> loadFromFile(const std::string& filename) {
        std::vector<Teacher> teachers;
        std::ifstream file(filename);
        
        if (!file.is_open()) {
            std::cout << "ℹ️  File " << filename << " not found. Starting with empty teacher list.\n";
            return teachers;
        }
        
        std::string line;
        int lineNumber = 0;
        
        while (std::getline(file, line)) {
            lineNumber++;
            if (line.empty()) continue;
            
            try {
                std::vector<std::string> parts = split(line, '|');
                if (parts.size() >= 9) {
                    Teacher teacher(std::stoi(parts[0]), parts[1], parts[2],
                                  parts[3], parts[4], parts[5], std::stod(parts[8]));
                    teacher.hireDate = parts[6];
                    teacher.isActive = (parts[7] == "1");
                    
                    // Load assigned courses
                    if (parts.size() > 9 && !parts[9].empty()) {
                        std::vector<std::string> courseIds = split(parts[9], ',');
                        for (const auto& courseId : courseIds) {
                            if (!courseId.empty()) {
                                teacher.assignedCourses.push_back(std::stoi(courseId));
                            }
                        }
                    }
                    
                    teachers.push_back(teacher);
                }
            } catch (const std::exception& e) {
                std::cerr << "⚠️  Warning: Error parsing line " << lineNumber 
                         << " in " << filename << ": " << e.what() << "\n";
            }
        }
        
        file.close();
        std::cout << "✅ Loaded " << teachers.size() << " teachers from " << filename << "\n";
        return teachers;
    }
    
    // Display methods
    void displayInfo() const {
        std::cout << "👩‍🏫 Teacher Information:\n";
        std::cout << "   ID: " << id << "\n";
        std::cout << "   Name: " << name << "\n";
        std::cout << "   Subject: " << subject << "\n";
        std::cout << "   Email: " << (email.empty() ? "Not provided" : email) << "\n";
        std::cout << "   Phone: " << (phone.empty() ? "Not provided" : phone) << "\n";
        std::cout << "   Department: " << (department.empty() ? "Not assigned" : department) << "\n";
        std::cout << "   Hire Date: " << hireDate << "\n";
        std::cout << "   Status: " << (isActive ? "Active" : "Inactive") << "\n";
        std::cout << "   Salary: $" << std::fixed << std::setprecision(2) << salary << "\n";
        std::cout << "   Assigned Courses: " << assignedCourses.size() << "\n";
    }
    
    void displaySummary() const {
        std::cout << std::setw(5) << id << " | " 
                  << std::setw(20) << name.substr(0, 20) << " | "
                  << std::setw(15) << subject.substr(0, 15) << " | "
                  << std::setw(20) << department.substr(0, 20) << " | "
                  << std::setw(8) << (isActive ? "Active" : "Inactive") << "\n";
    }
    
private:
    static std::string getCurrentDate() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d");
        return ss.str();
    }
    
    static std::vector<std::string> split(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;
        while (std::getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }
};

#endif // SCHOOL_MANAGEMENT_SYSTEM_H
