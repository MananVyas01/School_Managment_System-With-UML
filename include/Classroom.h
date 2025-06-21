#ifndef CLASSROOM_H
#define CLASSROOM_H

#include <string>
#include <vector>
#include <memory>

class Course;

class Classroom {
private:
    int id;
    int capacity;
    std::string location;
    std::string building;
    std::vector<std::shared_ptr<Course>> scheduledCourses;

public:
    Classroom(int id, int capacity, const std::string& location = "", const std::string& building = "");
    
    // Getters
    int getId() const { return id; }
    int getCapacity() const { return capacity; }
    const std::string& getLocation() const { return location; }
    const std::string& getBuilding() const { return building; }
    int getScheduledCoursesCount() const { return scheduledCourses.size(); }
    const std::vector<std::shared_ptr<Course>>& getScheduledCourses() const { return scheduledCourses; }
    
    // Setters
    void setCapacity(int newCapacity) { capacity = newCapacity; }
    void setLocation(const std::string& newLocation) { location = newLocation; }
    void setBuilding(const std::string& newBuilding) { building = newBuilding; }
    
    // Course management
    bool scheduleCourse(std::shared_ptr<Course> course);
    bool unscheduleCourse(int courseId);
    bool isCourseScheduled(int courseId) const;
    bool isFull() const;
    
    // File operations
    void saveToFile(const std::string& filename) const;
    static std::vector<Classroom> loadFromFile(const std::string& filename);
    
    // Utility
    std::string toString() const;
    void displayInfo() const;
    void displaySchedule() const;
    
    // Validation
    static bool isValidId(int id);
    static bool isValidCapacity(int capacity);
};

#endif // CLASSROOM_H
