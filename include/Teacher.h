#ifndef TEACHER_H
#define TEACHER_H

#include <string>
#include <vector>
#include <memory>

class Course; // Forward declaration

class Teacher {
private:
    int id;
    std::string name;
    std::string subject;
    std::string email;
    std::string phone;
    std::string department;
    std::vector<std::weak_ptr<Course>> assignedCourses;

public:
    Teacher(int id, const std::string& name, const std::string& subject,
            const std::string& email = "", const std::string& phone = "",
            const std::string& department = "");
    
    // Getters
    int getId() const { return id; }
    const std::string& getName() const { return name; }
    const std::string& getSubject() const { return subject; }
    const std::string& getEmail() const { return email; }
    const std::string& getPhone() const { return phone; }
    const std::string& getDepartment() const { return department; }
    const std::vector<std::weak_ptr<Course>>& getAssignedCourses() const { return assignedCourses; }
    
    // Setters
    void setName(const std::string& newName) { name = newName; }
    void setSubject(const std::string& newSubject) { subject = newSubject; }
    void setEmail(const std::string& newEmail) { email = newEmail; }
    void setPhone(const std::string& newPhone) { phone = newPhone; }
    void setDepartment(const std::string& newDepartment) { department = newDepartment; }
    
    // Course management
    void assignToCourse(std::shared_ptr<Course> course);
    void unassignFromCourse(int courseId);
    bool isAssignedTo(int courseId) const;
    int getAssignedCoursesCount() const { return assignedCourses.size(); }
    
    // File operations
    void saveToFile(const std::string& filename) const;
    static std::vector<Teacher> loadFromFile(const std::string& filename);
    
    // Utility
    std::string toString() const;
    void displayInfo() const;
    
    // Validation
    static bool isValidId(int id);
    static bool isValidEmail(const std::string& email);
};

#endif // TEACHER_H
