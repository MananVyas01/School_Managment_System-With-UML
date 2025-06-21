#include "../include/Classroom.h"
#include "../include/Course.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

Classroom::Classroom(int id, int capacity, const std::string& location, const std::string& building)
    : id(id), capacity(capacity), location(location), building(building) {}

bool Classroom::scheduleCourse(std::shared_ptr<Course> course) {
    if (!course || isCourseScheduled(course->getId()) || isFull()) {
        return false;
    }
    
    scheduledCourses.push_back(course);
    return true;
}

bool Classroom::unscheduleCourse(int courseId) {
    auto it = std::find_if(scheduledCourses.begin(), scheduledCourses.end(),
        [courseId](const std::shared_ptr<Course>& course) {
            return course && course->getId() == courseId;
        });
    
    if (it != scheduledCourses.end()) {
        scheduledCourses.erase(it);
        return true;
    }
    return false;
}

bool Classroom::isCourseScheduled(int courseId) const {
    return std::any_of(scheduledCourses.begin(), scheduledCourses.end(),
        [courseId](const std::shared_ptr<Course>& course) {
            return course && course->getId() == courseId;
        });
}

bool Classroom::isFull() const {
    return scheduledCourses.size() >= static_cast<size_t>(capacity);
}

void Classroom::saveToFile(const std::string& filename) const {
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        file << id << "," << capacity << "," << location << "," << building << "\n";
        file.close();
    }
}

std::vector<Classroom> Classroom::loadFromFile(const std::string& filename) {
    std::vector<Classroom> classrooms;
    std::ifstream file(filename);
    std::string line;
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string id, capacity, location, building;
        
        std::getline(ss, id, ',');
        std::getline(ss, capacity, ',');
        std::getline(ss, location, ',');
        std::getline(ss, building, ',');
        
        if (!id.empty() && !capacity.empty()) {
            classrooms.emplace_back(std::stoi(id), std::stoi(capacity), location, building);
        }
    }
    
    return classrooms;
}

std::string Classroom::toString() const {
    return "Classroom[ID: " + std::to_string(id) + ", Capacity: " + 
           std::to_string(capacity) + ", Location: " + location + "]";
}

void Classroom::displayInfo() const {
    std::cout << "🏫 " << toString() << std::endl;
    if (!building.empty()) std::cout << "   🏢 Building: " << building << std::endl;
    std::cout << "   📅 Scheduled Courses: " << scheduledCourses.size() << "/" << capacity << std::endl;
}

void Classroom::displaySchedule() const {
    std::cout << "📅 Schedule for " << toString() << ":\n";
    for (const auto& course : scheduledCourses) {
        if (course) {
            std::cout << "  • " << course->getName() << " (ID: " << course->getId() << ")\n";
        }
    }
}

bool Classroom::isValidId(int id) {
    return id > 0;
}

bool Classroom::isValidCapacity(int capacity) {
    return capacity > 0 && capacity <= 1000;
}
