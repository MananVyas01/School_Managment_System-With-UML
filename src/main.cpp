#include "include/Config.h"
#include "include/Student.h"
#include "include/Teacher.h"
#include "include/Course.h"
#include "include/Classroom.h"
#include "include/Reports.h"
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <limits>

class SchoolManagementSystem {
private:
    std::vector<std::shared_ptr<Student>> students;
    std::vector<std::shared_ptr<Teacher>> teachers;
    std::vector<std::shared_ptr<Course>> courses;
    std::vector<std::shared_ptr<Classroom>> classrooms;
    Config* config;
    ReportGenerator reportGenerator;

public:
    SchoolManagementSystem() {
        config = Config::getInstance();
        config->loadFromFile("config.txt");
        reportGenerator = ReportGenerator(config->get("reports_dir", "reports/"));
        loadAllData();
    }

    void run() {
        std::cout << "🎓 Welcome to School Management System v2.0\n";
        std::cout << "=============================================\n\n";
        
        int choice;
        do {
            showMainMenu();
            choice = getIntInput("Enter your choice: ");
            processChoice(choice);
        } while (choice != 0);
        
        saveAllData();
        std::cout << "\n👋 Thank you for using School Management System!\n";
    }

private:
    void showMainMenu() {
        std::cout << "\n📋 MAIN MENU\n";
        std::cout << "=============\n";
        std::cout << "1. 👨‍🎓 Student Management\n";
        std::cout << "2. 👩‍🏫 Teacher Management\n";
        std::cout << "3. 📚 Course Management\n";
        std::cout << "4. 🏫 Classroom Management\n";
        std::cout << "5. 📊 Reports & Statistics\n";
        std::cout << "6. ⚙️  System Settings\n";
        std::cout << "0. 🚪 Exit\n";
        std::cout << "=============\n";
    }

    void processChoice(int choice) {
        switch (choice) {
            case 1: studentMenu(); break;
            case 2: teacherMenu(); break;
            case 3: courseMenu(); break;
            case 4: classroomMenu(); break;
            case 5: reportsMenu(); break;
            case 6: settingsMenu(); break;
            case 0: break;
            default: std::cout << "❌ Invalid choice! Please try again.\n";
        }
    }

    void studentMenu() {
        int choice;
        do {
            std::cout << "\n👨‍🎓 STUDENT MANAGEMENT\n";
            std::cout << "=====================\n";
            std::cout << "1. Add New Student\n";
            std::cout << "2. View All Students\n";
            std::cout << "3. Search Student\n";
            std::cout << "4. Update Student\n";
            std::cout << "5. Delete Student\n";
            std::cout << "6. Enroll in Course\n";
            std::cout << "7. Record Attendance\n";
            std::cout << "0. Back to Main Menu\n";
            
            choice = getIntInput("Enter your choice: ");
            
            switch (choice) {
                case 1: addStudent(); break;
                case 2: viewAllStudents(); break;
                case 3: searchStudent(); break;
                case 4: updateStudent(); break;
                case 5: deleteStudent(); break;
                case 6: enrollStudentInCourse(); break;
                case 7: recordStudentAttendance(); break;
                case 0: break;
                default: std::cout << "❌ Invalid choice!\n";
            }
        } while (choice != 0);
    }

    void teacherMenu() {
        int choice;
        do {
            std::cout << "\n👩‍🏫 TEACHER MANAGEMENT\n";
            std::cout << "=====================\n";
            std::cout << "1. Add New Teacher\n";
            std::cout << "2. View All Teachers\n";
            std::cout << "3. Search Teacher\n";
            std::cout << "4. Update Teacher\n";
            std::cout << "5. Delete Teacher\n";
            std::cout << "6. Assign to Course\n";
            std::cout << "0. Back to Main Menu\n";
            
            choice = getIntInput("Enter your choice: ");
            
            switch (choice) {
                case 1: addTeacher(); break;
                case 2: viewAllTeachers(); break;
                case 3: searchTeacher(); break;
                case 4: updateTeacher(); break;
                case 5: deleteTeacher(); break;
                case 6: assignTeacherToCourse(); break;
                case 0: break;
                default: std::cout << "❌ Invalid choice!\n";
            }
        } while (choice != 0);
    }

    void courseMenu() {
        int choice;
        do {
            std::cout << "\n📚 COURSE MANAGEMENT\n";
            std::cout << "===================\n";
            std::cout << "1. Add New Course\n";
            std::cout << "2. View All Courses\n";
            std::cout << "3. Search Course\n";
            std::cout << "4. Update Course\n";
            std::cout << "5. Delete Course\n";
            std::cout << "6. View Course Details\n";
            std::cout << "0. Back to Main Menu\n";
            
            choice = getIntInput("Enter your choice: ");
            
            switch (choice) {
                case 1: addCourse(); break;
                case 2: viewAllCourses(); break;
                case 3: searchCourse(); break;
                case 4: updateCourse(); break;
                case 5: deleteCourse(); break;
                case 6: viewCourseDetails(); break;
                case 0: break;
                default: std::cout << "❌ Invalid choice!\n";
            }
        } while (choice != 0);
    }

    void reportsMenu() {
        int choice;
        do {
            std::cout << "\n📊 REPORTS & STATISTICS\n";
            std::cout << "======================\n";
            std::cout << "1. Student Reports\n";
            std::cout << "2. Teacher Reports\n";
            std::cout << "3. Course Reports\n";
            std::cout << "4. Attendance Reports\n";
            std::cout << "5. System Statistics\n";
            std::cout << "6. Generate All Reports\n";
            std::cout << "0. Back to Main Menu\n";
            
            choice = getIntInput("Enter your choice: ");
            
            switch (choice) {
                case 1: generateStudentReports(); break;
                case 2: generateTeacherReports(); break;
                case 3: generateCourseReports(); break;
                case 4: generateAttendanceReports(); break;
                case 5: showSystemStatistics(); break;
                case 6: generateAllReports(); break;
                case 0: break;
                default: std::cout << "❌ Invalid choice!\n";
            }
        } while (choice != 0);
    }

    // Utility methods
    int getIntInput(const std::string& prompt) {
        int value;
        std::cout << prompt;
        while (!(std::cin >> value)) {
            std::cout << "❌ Invalid input! Please enter a number: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cin.ignore(); // Clear the buffer
        return value;
    }

    std::string getStringInput(const std::string& prompt) {
        std::string value;
        std::cout << prompt;
        std::getline(std::cin, value);
        return value;
    }

    // Placeholder implementations (to be completed)
    void addStudent() {
        std::cout << "🔨 Add Student functionality - To be implemented\n";
    }

    void viewAllStudents() {
        std::cout << "📋 View All Students:\n";
        if (students.empty()) {
            std::cout << "No students found.\n";
            return;
        }
        for (const auto& student : students) {
            if (student) {
                student->displayInfo();
                std::cout << "-------------------\n";
            }
        }
    }

    void loadAllData() {
        std::cout << "📂 Loading system data...\n";
        // Implementation will be added when class implementations are complete
    }

    void saveAllData() {
        std::cout << "💾 Saving system data...\n";
        // Implementation will be added when class implementations are complete
    }

    // Placeholder methods for other functionalities
    void searchStudent() { std::cout << "🔨 Search Student - To be implemented\n"; }
    void updateStudent() { std::cout << "🔨 Update Student - To be implemented\n"; }
    void deleteStudent() { std::cout << "🔨 Delete Student - To be implemented\n"; }
    void enrollStudentInCourse() { std::cout << "🔨 Enroll Student - To be implemented\n"; }
    void recordStudentAttendance() { std::cout << "🔨 Record Attendance - To be implemented\n"; }
    
    void addTeacher() { std::cout << "🔨 Add Teacher - To be implemented\n"; }
    void viewAllTeachers() { std::cout << "🔨 View Teachers - To be implemented\n"; }
    void searchTeacher() { std::cout << "🔨 Search Teacher - To be implemented\n"; }
    void updateTeacher() { std::cout << "🔨 Update Teacher - To be implemented\n"; }
    void deleteTeacher() { std::cout << "🔨 Delete Teacher - To be implemented\n"; }
    void assignTeacherToCourse() { std::cout << "🔨 Assign Teacher - To be implemented\n"; }
    
    void addCourse() { std::cout << "🔨 Add Course - To be implemented\n"; }
    void viewAllCourses() { std::cout << "🔨 View Courses - To be implemented\n"; }
    void searchCourse() { std::cout << "🔨 Search Course - To be implemented\n"; }
    void updateCourse() { std::cout << "🔨 Update Course - To be implemented\n"; }
    void deleteCourse() { std::cout << "🔨 Delete Course - To be implemented\n"; }
    void viewCourseDetails() { std::cout << "🔨 Course Details - To be implemented\n"; }
    
    void classroomMenu() { std::cout << "🔨 Classroom Management - To be implemented\n"; }
    void settingsMenu() { std::cout << "🔨 System Settings - To be implemented\n"; }
    
    void generateStudentReports() { std::cout << "🔨 Student Reports - To be implemented\n"; }
    void generateTeacherReports() { std::cout << "🔨 Teacher Reports - To be implemented\n"; }
    void generateCourseReports() { std::cout << "🔨 Course Reports - To be implemented\n"; }
    void generateAttendanceReports() { std::cout << "🔨 Attendance Reports - To be implemented\n"; }
    void showSystemStatistics() { std::cout << "🔨 System Statistics - To be implemented\n"; }
    void generateAllReports() { std::cout << "🔨 Generate All Reports - To be implemented\n"; }
};

int main() {
    try {
        SchoolManagementSystem sms;
        sms.run();
    } catch (const std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
