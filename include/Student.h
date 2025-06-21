#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

class Course; // Forward declaration

class Student {
private:
    int id;
    std::string name;
    int age;
    std::string email;
    std::string phone;
    std::vector<std::shared_ptr<Course>> enrolledCourses;
    std::unordered_map<std::string, bool> attendance; // Course name and presence

public:
    Student(int id, const std::string& name, int age, 
            const std::string& email = "", const std::string& phone = "");
    
    // Getters
    int getId() const { return id; }
    const std::string& getName() const { return name; }
    int getAge() const { return age; }
    const std::string& getEmail() const { return email; }
    const std::string& getPhone() const { return phone; }
    const std::vector<std::shared_ptr<Course>>& getEnrolledCourses() const { return enrolledCourses; }
    
    // Setters
    void setName(const std::string& newName) { name = newName; }
    void setAge(int newAge) { age = newAge; }
    void setEmail(const std::string& newEmail) { email = newEmail; }
    void setPhone(const std::string& newPhone) { phone = newPhone; }
    
    // Course management
    void enrollInCourse(std::shared_ptr<Course> course);
    void unenrollFromCourse(int courseId);
    bool isEnrolledIn(int courseId) const;
    
    // Attendance
    void markAttendance(const std::string& courseName, bool isPresent);
    bool getAttendance(const std::string& courseName) const;
    double getAttendancePercentage(const std::string& courseName) const;
    
    // File operations
    void saveToFile(const std::string& filename) const;
    static std::vector<Student> loadFromFile(const std::string& filename);
    
    // Utility
    std::string toString() const;
    void displayInfo() const;
    
    // Validation
    static bool isValidId(int id);
    static bool isValidAge(int age);
    static bool isValidEmail(const std::string& email);
};

#endif // STUDENT_H
