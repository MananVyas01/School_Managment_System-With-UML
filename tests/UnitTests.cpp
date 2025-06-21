#include "../include/TestFramework.h"
#include "../include/Student.h"
#include "../include/Teacher.h"
#include "../include/Course.h"
#include "../include/Config.h"
#include <memory>

class StudentTests {
public:
    static void testStudentCreation() {
        Student student(1, "John Doe", 20, "john@example.com", "123-456-7890");
        
        ASSERT_EQ(1, student.getId());
        ASSERT_EQ("John Doe", student.getName());
        ASSERT_EQ(20, student.getAge());
        ASSERT_EQ("john@example.com", student.getEmail());
        ASSERT_EQ("123-456-7890", student.getPhone());
    }
    
    static void testStudentValidation() {
        ASSERT_TRUE(Student::isValidId(1));
        ASSERT_FALSE(Student::isValidId(-1));
        ASSERT_FALSE(Student::isValidId(0));
        
        ASSERT_TRUE(Student::isValidAge(18));
        ASSERT_TRUE(Student::isValidAge(25));
        ASSERT_FALSE(Student::isValidAge(-1));
        ASSERT_FALSE(Student::isValidAge(150));
        
        ASSERT_TRUE(Student::isValidEmail("test@example.com"));
        ASSERT_FALSE(Student::isValidEmail("invalid-email"));
        ASSERT_TRUE(Student::isValidEmail("")); // Empty email should be valid (optional)
    }
    
    static void testStudentSetters() {
        Student student(1, "John", 20);
        
        student.setName("Jane Doe");
        ASSERT_EQ("Jane Doe", student.getName());
        
        student.setAge(22);
        ASSERT_EQ(22, student.getAge());
        
        student.setEmail("jane@example.com");
        ASSERT_EQ("jane@example.com", student.getEmail());
    }
    
    static void testStudentAttendance() {
        Student student(1, "John", 20);
        
        student.markAttendance("Math", true);
        student.markAttendance("Math", false);
        student.markAttendance("Math", true);
        
        // Test attendance marking (basic functionality)
        ASSERT_TRUE(student.getAttendance("Math")); // Last recorded attendance
    }
};

class TeacherTests {
public:
    static void testTeacherCreation() {
        Teacher teacher(1, "Dr. Smith", "Mathematics", "smith@school.edu", "555-1234", "Math Department");
        
        ASSERT_EQ(1, teacher.getId());
        ASSERT_EQ("Dr. Smith", teacher.getName());
        ASSERT_EQ("Mathematics", teacher.getSubject());
        ASSERT_EQ("smith@school.edu", teacher.getEmail());
        ASSERT_EQ("555-1234", teacher.getPhone());
        ASSERT_EQ("Math Department", teacher.getDepartment());
    }
    
    static void testTeacherValidation() {
        ASSERT_TRUE(Teacher::isValidId(1));
        ASSERT_FALSE(Teacher::isValidId(-1));
        ASSERT_FALSE(Teacher::isValidId(0));
        
        ASSERT_TRUE(Teacher::isValidEmail("teacher@school.edu"));
        ASSERT_FALSE(Teacher::isValidEmail("invalid"));
    }
    
    static void testTeacherSetters() {
        Teacher teacher(1, "Dr. Smith", "Math");
        
        teacher.setName("Dr. Johnson");
        ASSERT_EQ("Dr. Johnson", teacher.getName());
        
        teacher.setSubject("Physics");
        ASSERT_EQ("Physics", teacher.getSubject());
        
        teacher.setDepartment("Science Department");
        ASSERT_EQ("Science Department", teacher.getDepartment());
    }
};

class CourseTests {
public:
    static void testCourseCreation() {
        Course course(1, "Mathematics 101", "Introduction to Mathematics", 3, 30);
        
        ASSERT_EQ(1, course.getId());
        ASSERT_EQ("Mathematics 101", course.getName());
        ASSERT_EQ("Introduction to Mathematics", course.getDescription());
        ASSERT_EQ(3, course.getCredits());
        ASSERT_EQ(30, course.getMaxStudents());
        ASSERT_EQ(0, course.getCurrentStudentsCount());
        ASSERT_FALSE(course.hasTeacher());
        ASSERT_FALSE(course.isFull());
    }
    
    static void testCourseValidation() {
        ASSERT_TRUE(Course::isValidId(1));
        ASSERT_FALSE(Course::isValidId(-1));
        ASSERT_FALSE(Course::isValidId(0));
        
        ASSERT_TRUE(Course::isValidCredits(1));
        ASSERT_TRUE(Course::isValidCredits(5));
        ASSERT_FALSE(Course::isValidCredits(0));
        ASSERT_FALSE(Course::isValidCredits(-1));
    }
    
    static void testCourseTeacherAssignment() {
        Course course(1, "Math 101");
        auto teacher = std::make_shared<Teacher>(1, "Dr. Smith", "Math");
        
        ASSERT_FALSE(course.hasTeacher());
        
        course.assignTeacher(teacher);
        ASSERT_TRUE(course.hasTeacher());
        ASSERT_TRUE(course.getAssignedTeacher() == teacher);
        // Test unassignment ASSET_EQ(Teacher, course.getAssignedTeacher()); is not valid, use pointer comparison
        // Unassigning teacher
        course.unassignTeacher();
        ASSERT_FALSE(course.hasTeacher());
    }
    
    static void testCourseStudentEnrollment() {
        Course course(1, "Math 101", "", 3, 2); // Max 2 students
        auto student1 = std::make_shared<Student>(1, "John", 20);
        auto student2 = std::make_shared<Student>(2, "Jane", 21);
        auto student3 = std::make_shared<Student>(3, "Bob", 19);
        
        ASSERT_EQ(0, course.getCurrentStudentsCount());
        ASSERT_FALSE(course.isFull());
        
        ASSERT_TRUE(course.enrollStudent(student1));
        ASSERT_EQ(1, course.getCurrentStudentsCount());
        ASSERT_TRUE(course.isStudentEnrolled(1));
        
        ASSERT_TRUE(course.enrollStudent(student2));
        ASSERT_EQ(2, course.getCurrentStudentsCount());
        ASSERT_TRUE(course.isFull());
        
        // Should fail - course is full
        ASSERT_FALSE(course.enrollStudent(student3));
        ASSERT_EQ(2, course.getCurrentStudentsCount());
        
        // Test unenrollment
        ASSERT_TRUE(course.unenrollStudent(1));
        ASSERT_EQ(1, course.getCurrentStudentsCount());
        ASSERT_FALSE(course.isStudentEnrolled(1));
        ASSERT_FALSE(course.isFull());
    }
};

class ConfigTests {
public:
    static void testConfigSingleton() {
        Config* config1 = Config::getInstance();
        Config* config2 = Config::getInstance();
        
        ASSERT_TRUE(config1 == config2); // Should be same instance
        // Test loading from default config file
        // Assuming default config file is "config.txt"
        // This should be replaced with the actual path to your config file
    }
    
    static void testConfigDefaults() {
        Config* config = Config::getInstance();
        
        ASSERT_EQ("data/students.txt", config->get("students_file"));
        ASSERT_EQ("data/teachers.txt", config->get("teachers_file"));
        ASSERT_EQ("50", config->get("max_students_per_course"));
        ASSERT_EQ("true", config->get("backup_enabled"));
    }
    
    static void testConfigSetGet() {
        Config* config = Config::getInstance();
        
        config->set("test_key", "test_value");
        ASSERT_EQ("test_value", config->get("test_key"));
        
        ASSERT_EQ("default", config->get("nonexistent_key", "default"));
    }
};

void runAllTests() {
    TestFramework framework;
    
    // Student Tests
    framework.addTest("Student Creation", StudentTests::testStudentCreation);
    framework.addTest("Student Validation", StudentTests::testStudentValidation);
    framework.addTest("Student Setters", StudentTests::testStudentSetters);
    framework.addTest("Student Attendance", StudentTests::testStudentAttendance);
    
    // Teacher Tests
    framework.addTest("Teacher Creation", TeacherTests::testTeacherCreation);
    framework.addTest("Teacher Validation", TeacherTests::testTeacherValidation);
    framework.addTest("Teacher Setters", TeacherTests::testTeacherSetters);
    
    // Course Tests
    framework.addTest("Course Creation", CourseTests::testCourseCreation);
    framework.addTest("Course Validation", CourseTests::testCourseValidation);
    framework.addTest("Course Teacher Assignment", CourseTests::testCourseTeacherAssignment);
    framework.addTest("Course Student Enrollment", CourseTests::testCourseStudentEnrollment);
    
    // Config Tests
    framework.addTest("Config Singleton", ConfigTests::testConfigSingleton);
    framework.addTest("Config Defaults", ConfigTests::testConfigDefaults);
    framework.addTest("Config Set/Get", ConfigTests::testConfigSetGet);
    
    framework.runAllTests();
    framework.printResults();
}
