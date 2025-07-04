#include "../include/Reports.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>

ReportGenerator::ReportGenerator(const std::string& reportDirectory) : reportDir(reportDirectory) {
    createReportDirectory();
}

void ReportGenerator::createReportDirectory() {
    // Simple directory creation - in real implementation would use filesystem library
    std::cout << "📁 Reports will be saved to: " << reportDir << std::endl;
}

std::string ReportGenerator::generateTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y%m%d_%H%M%S");
    return ss.str();
}

std::string ReportGenerator::getCurrentDate() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void ReportGenerator::writeHeader(std::ofstream& file, const std::string& title) {
    file << "=" << std::string(50, '=') << "=\n";
    file << "  " << title << "\n";
    file << "  Generated on: " << getCurrentDate() << "\n";
    file << "=" << std::string(50, '=') << "=\n\n";
}

void ReportGenerator::writeFooter(std::ofstream& file) {
    file << "\n" << std::string(52, '-') << "\n";
    file << "Report generated by School Management System v2.0\n";
}

void ReportGenerator::generateStudentReport(const Student& student, const std::string& filename) {
    std::string fname = filename.empty() ? 
        reportDir + "student_" + std::to_string(student.getId()) + "_" + generateTimestamp() + ".txt" : 
        reportDir + filename;
    
    std::ofstream file(fname);
    if (file.is_open()) {
        writeHeader(file, "STUDENT REPORT");
        file << "Student Information:\n";
        file << "==================\n";
        file << "ID: " << student.getId() << "\n";
        file << "Name: " << student.getName() << "\n";
        file << "Age: " << student.getAge() << "\n";
        if (!student.getEmail().empty()) file << "Email: " << student.getEmail() << "\n";
        if (!student.getPhone().empty()) file << "Phone: " << student.getPhone() << "\n";
        
        file << "\nEnrolled Courses:\n";
        file << "================\n";
        const auto& courses = student.getEnrolledCourses();
        if (courses.empty()) {
            file << "No courses enrolled.\n";
        } else {
            for (const auto& courseWeakPtr : courses) {
                auto course = courseWeakPtr.lock();
                if (course) {
                    file << "- " << course->getName() << " (ID: " << course->getId() << ")\n";
                }
            }
        }
        
        writeFooter(file);
        file.close();
        std::cout << "✅ Student report generated: " << fname << std::endl;
    }
}

void ReportGenerator::generateAllStudentsReport(const std::vector<Student>& students, const std::string& filename) {
    std::string fname = filename.empty() ? 
        reportDir + "all_students_" + generateTimestamp() + ".txt" : 
        reportDir + filename;
    
    std::ofstream file(fname);
    if (file.is_open()) {
        writeHeader(file, "ALL STUDENTS REPORT");
        file << "Total Students: " << students.size() << "\n\n";
        
        for (const auto& student : students) {
            file << "Student ID: " << student.getId() << "\n";
            file << "Name: " << student.getName() << "\n";
            file << "Age: " << student.getAge() << "\n";
            if (!student.getEmail().empty()) file << "Email: " << student.getEmail() << "\n";
            file << std::string(30, '-') << "\n";
        }
        
        writeFooter(file);
        file.close();
        std::cout << "✅ All students report generated: " << fname << std::endl;
    }
}

// Statistics implementations
double Statistics::calculateAverageAge(const std::vector<Student>& students) {
    if (students.empty()) return 0.0;
    
    double totalAge = 0.0;
    for (const auto& student : students) {
        totalAge += student.getAge();
    }
    
    return totalAge / students.size();
}

int Statistics::getTotalStudents(const std::vector<Student>& students) {
    return students.size();
}

int Statistics::getTotalTeachers(const std::vector<Teacher>& teachers) {
    return teachers.size();
}

int Statistics::getTotalCourses(const std::vector<Course>& courses) {
    return courses.size();
}

std::string Statistics::generateSummaryReport(const std::vector<Student>& students,
                                           const std::vector<Teacher>& teachers,
                                           const std::vector<Course>& courses) {
    std::stringstream report;
    
    report << "📊 SYSTEM SUMMARY REPORT\n";
    report << "========================\n\n";
    report << "Total Students: " << getTotalStudents(students) << "\n";
    report << "Total Teachers: " << getTotalTeachers(teachers) << "\n";
    report << "Total Courses: " << getTotalCourses(courses) << "\n";
    
    if (!students.empty()) {
        report << "Average Student Age: " << std::fixed << std::setprecision(1) 
               << calculateAverageAge(students) << " years\n";
    }
    
    report << "\n📈 Quick Statistics:\n";
    report << "===================\n";
    if (!courses.empty()) {
        report << "Average Students per Course: " << (students.size() / courses.size()) << "\n";
    }
    
    return report.str();
}

// Placeholder implementations for other methods
void ReportGenerator::generateStudentAttendanceReport(const std::vector<Student>& students, const std::vector<Course>& courses, const std::string& filename) {
    std::cout << "📊 Student Attendance Report - Implementation in progress\n";
}

void ReportGenerator::generateTeacherReport(const Teacher& teacher, const std::string& filename) {
    std::cout << "👩‍🏫 Teacher Report - Implementation in progress\n";
}

void ReportGenerator::generateAllTeachersReport(const std::vector<Teacher>& teachers, const std::string& filename) {
    std::cout << "👥 All Teachers Report - Implementation in progress\n";
}

void ReportGenerator::generateTeacherWorkloadReport(const std::vector<Teacher>& teachers, const std::string& filename) {
    std::cout << "📈 Teacher Workload Report - Implementation in progress\n";
}

void ReportGenerator::generateCourseReport(const Course& course, const std::string& filename) {
    std::cout << "📚 Course Report - Implementation in progress\n";
}

void ReportGenerator::generateAllCoursesReport(const std::vector<Course>& courses, const std::string& filename) {
    std::cout << "📖 All Courses Report - Implementation in progress\n";
}

void ReportGenerator::generateCourseEnrollmentReport(const std::vector<Course>& courses, const std::string& filename) {
    std::cout << "📊 Course Enrollment Report - Implementation in progress\n";
}

void ReportGenerator::generateStatisticsReport(const std::vector<Student>& students, 
                            const std::vector<Teacher>& teachers,
                            const std::vector<Course>& courses,
                            const std::string& filename) {
    std::cout << "📊 Statistics Report - Implementation in progress\n";
}

void ReportGenerator::generateAttendanceStatistics(const std::vector<Course>& courses, const std::string& filename) {
    std::cout << "📅 Attendance Statistics - Implementation in progress\n";
}
