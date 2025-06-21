#ifndef REPORTS_H
#define REPORTS_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "Student.h"
#include "Teacher.h"
#include "Course.h"
#include "Classroom.h"

class ReportGenerator {
private:
    std::string reportDir;
    
public:
    ReportGenerator(const std::string& reportDirectory = "reports/");
    
    // Student Reports
    void generateStudentReport(const Student& student, const std::string& filename = "");
    void generateAllStudentsReport(const std::vector<Student>& students, const std::string& filename = "");
    void generateStudentAttendanceReport(const std::vector<Student>& students, const std::vector<Course>& courses, const std::string& filename = "");
    
    // Teacher Reports
    void generateTeacherReport(const Teacher& teacher, const std::string& filename = "");
    void generateAllTeachersReport(const std::vector<Teacher>& teachers, const std::string& filename = "");
    void generateTeacherWorkloadReport(const std::vector<Teacher>& teachers, const std::string& filename = "");
    
    // Course Reports
    void generateCourseReport(const Course& course, const std::string& filename = "");
    void generateAllCoursesReport(const std::vector<Course>& courses, const std::string& filename = "");
    void generateCourseEnrollmentReport(const std::vector<Course>& courses, const std::string& filename = "");
    
    // Statistical Reports
    void generateStatisticsReport(const std::vector<Student>& students, 
                                const std::vector<Teacher>& teachers,
                                const std::vector<Course>& courses,
                                const std::string& filename = "");
    
    // Attendance Reports
    void generateAttendanceStatistics(const std::vector<Course>& courses, const std::string& filename = "");
    
    // Utility functions
    void createReportDirectory();
    std::string generateTimestamp();
    
private:
    void writeHeader(std::ofstream& file, const std::string& title);
    void writeFooter(std::ofstream& file);
    std::string getCurrentDate();
};

class Statistics {
public:
    // Student Statistics
    static double calculateAverageAge(const std::vector<Student>& students);
    static std::unordered_map<std::string, int> getCourseEnrollmentCount(const std::vector<Course>& courses);
    static int getTotalStudents(const std::vector<Student>& students);
    
    // Teacher Statistics
    static double calculateAverageTeacherWorkload(const std::vector<Teacher>& teachers);
    static std::unordered_map<std::string, int> getSubjectDistribution(const std::vector<Teacher>& teachers);
    static int getTotalTeachers(const std::vector<Teacher>& teachers);
    
    // Course Statistics
    static double calculateAverageEnrollment(const std::vector<Course>& courses);
    static Course getMostPopularCourse(const std::vector<Course>& courses);
    static Course getLeastPopularCourse(const std::vector<Course>& courses);
    static int getTotalCourses(const std::vector<Course>& courses);
    
    // Attendance Statistics
    static double calculateOverallAttendanceRate(const std::vector<Course>& courses);
    static std::unordered_map<int, double> getStudentAttendanceRates(const std::vector<Course>& courses);
    static std::unordered_map<int, double> getCourseAttendanceRates(const std::vector<Course>& courses);
    
    // General Statistics
    static std::string generateSummaryReport(const std::vector<Student>& students,
                                           const std::vector<Teacher>& teachers,
                                           const std::vector<Course>& courses);
};

#endif // REPORTS_H
