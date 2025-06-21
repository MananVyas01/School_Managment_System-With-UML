#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

class Student;
class Teacher;

class Course {
private:
    int id;
    std::string name;
    std::string description;
    int credits;
    int maxStudents;
    std::vector<std::shared_ptr<Student>> enrolledStudents;
    std::shared_ptr<Teacher> assignedTeacher;
    std::unordered_map<int, std::unordered_map<std::string, bool>> attendanceRecords; // studentId -> date -> present

public:
    Course(int id, const std::string& name, const std::string& description = "", 
           int credits = 3, int maxStudents = 50);
    
    // Getters
    int getId() const { return id; }
    const std::string& getName() const { return name; }
    const std::string& getDescription() const { return description; }
    int getCredits() const { return credits; }
    int getMaxStudents() const { return maxStudents; }
    int getCurrentStudentsCount() const { return enrolledStudents.size(); }
    const std::vector<std::shared_ptr<Student>>& getEnrolledStudents() const { return enrolledStudents; }
    std::shared_ptr<Teacher> getAssignedTeacher() const { return assignedTeacher; }
    
    // Setters
    void setName(const std::string& newName) { name = newName; }
    void setDescription(const std::string& newDescription) { description = newDescription; }
    void setCredits(int newCredits) { credits = newCredits; }
    void setMaxStudents(int newMaxStudents) { maxStudents = newMaxStudents; }
    
    // Teacher management
    void assignTeacher(std::shared_ptr<Teacher> teacher);
    void unassignTeacher();
    bool hasTeacher() const { return assignedTeacher != nullptr; }
    
    // Student management
    bool enrollStudent(std::shared_ptr<Student> student);
    bool unenrollStudent(int studentId);
    bool isStudentEnrolled(int studentId) const;
    bool isFull() const { return enrolledStudents.size() >= maxStudents; }
    
    // Attendance management
    void recordAttendance(int studentId, const std::string& date, bool isPresent);
    bool getAttendance(int studentId, const std::string& date) const;
    double getStudentAttendancePercentage(int studentId) const;
    std::unordered_map<int, double> getAllStudentAttendancePercentages() const;
    
    // File operations
    void saveToFile(const std::string& filename) const;
    static std::vector<Course> loadFromFile(const std::string& filename);
    
    // Utility
    std::string toString() const;
    void displayInfo() const;
    void displayStudentList() const;
    void displayAttendanceReport() const;
    
    // Validation
    static bool isValidId(int id);
    static bool isValidCredits(int credits);
};

#endif // COURSE_H
