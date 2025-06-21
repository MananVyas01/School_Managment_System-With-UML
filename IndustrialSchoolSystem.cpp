#include "IndustrialSchoolSystem.h"
#include <algorithm>
#include <cstdlib>

// Enhanced Course class
class Course {
private:
    int id;
    std::string name;
    std::string description;
    int credits;
    int maxStudents;
    std::vector<int> enrolledStudents;
    int assignedTeacherId;
    std::string startDate;
    std::string endDate;
    bool isActive;
    double fee;
    
public:
    Course(int id, const std::string& name, const std::string& description = "",
           int credits = 3, int maxStudents = 30, double fee = 0.0)
        : id(id), name(name), description(description), credits(credits), 
          maxStudents(maxStudents), assignedTeacherId(-1), isActive(true), fee(fee) {
        startDate = getCurrentDate();
        endDate = ""; // Will be set when course ends
    }
    
    // Getters
    int getId() const { return id; }
    const std::string& getName() const { return name; }
    const std::string& getDescription() const { return description; }
    int getCredits() const { return credits; }
    int getMaxStudents() const { return maxStudents; }
    int getCurrentEnrollment() const { return enrolledStudents.size(); }
    int getAssignedTeacherId() const { return assignedTeacherId; }
    const std::string& getStartDate() const { return startDate; }
    const std::string& getEndDate() const { return endDate; }
    bool getIsActive() const { return isActive; }
    double getFee() const { return fee; }
    const std::vector<int>& getEnrolledStudents() const { return enrolledStudents; }
    
    // Setters with validation
    bool setName(const std::string& newName) {
        if (InputValidator::isValidName(newName)) {
            name = InputValidator::sanitizeString(newName);
            return true;
        }
        return false;
    }
    
    bool setDescription(const std::string& newDescription) {
        description = InputValidator::sanitizeString(newDescription);
        return true;
    }
    
    bool setCredits(int newCredits) {
        if (newCredits > 0 && newCredits <= 10) {
            credits = newCredits;
            return true;
        }
        return false;
    }
    
    bool setMaxStudents(int newMaxStudents) {
        if (newMaxStudents > 0 && newMaxStudents <= 500) {
            maxStudents = newMaxStudents;
            return true;
        }
        return false;
    }
    
    void setAssignedTeacherId(int teacherId) { assignedTeacherId = teacherId; }
    void setIsActive(bool active) { isActive = active; }
    void setEndDate(const std::string& date) { endDate = date; }
    
    bool setFee(double newFee) {
        if (newFee >= 0) {
            fee = newFee;
            return true;
        }
        return false;
    }
    
    // Student management
    bool enrollStudent(int studentId) {
        if (isActive && getCurrentEnrollment() < maxStudents && 
            std::find(enrolledStudents.begin(), enrolledStudents.end(), studentId) == enrolledStudents.end()) {
            enrolledStudents.push_back(studentId);
            return true;
        }
        return false;
    }
    
    bool unenrollStudent(int studentId) {
        auto it = std::find(enrolledStudents.begin(), enrolledStudents.end(), studentId);
        if (it != enrolledStudents.end()) {
            enrolledStudents.erase(it);
            return true;
        }
        return false;
    }
    
    bool isStudentEnrolled(int studentId) const {
        return std::find(enrolledStudents.begin(), enrolledStudents.end(), studentId) != enrolledStudents.end();
    }
    
    bool isFull() const { return getCurrentEnrollment() >= maxStudents; }
    bool hasTeacher() const { return assignedTeacherId != -1; }
    
    // File operations
    bool saveToFile(const std::string& filename) const {
        std::ofstream file(filename, std::ios::app);
        if (!file.is_open()) {
            std::cerr << "❌ Error: Cannot open file " << filename << " for writing.\n";
            return false;
        }
        
        file << id << "|" << name << "|" << description << "|" << credits << "|"
             << maxStudents << "|" << assignedTeacherId << "|" << startDate << "|"
             << endDate << "|" << (isActive ? "1" : "0") << "|" << fee << "|";
        
        // Save enrolled students
        for (size_t i = 0; i < enrolledStudents.size(); ++i) {
            file << enrolledStudents[i];
            if (i < enrolledStudents.size() - 1) file << ",";
        }
        file << "\n";
        
        file.close();
        return true;
    }
    
    static std::vector<Course> loadFromFile(const std::string& filename) {
        std::vector<Course> courses;
        std::ifstream file(filename);
        
        if (!file.is_open()) {
            std::cout << "ℹ️  File " << filename << " not found. Starting with empty course list.\n";
            return courses;
        }
        
        std::string line;
        int lineNumber = 0;
        
        while (std::getline(file, line)) {
            lineNumber++;
            if (line.empty()) continue;
            
            try {
                std::vector<std::string> parts = split(line, '|');
                if (parts.size() >= 10) {
                    Course course(std::stoi(parts[0]), parts[1], parts[2],
                                std::stoi(parts[3]), std::stoi(parts[4]), std::stod(parts[9]));
                    course.assignedTeacherId = std::stoi(parts[5]);
                    course.startDate = parts[6];
                    course.endDate = parts[7];
                    course.isActive = (parts[8] == "1");
                    
                    // Load enrolled students
                    if (parts.size() > 10 && !parts[10].empty()) {
                        std::vector<std::string> studentIds = split(parts[10], ',');
                        for (const auto& studentId : studentIds) {
                            if (!studentId.empty()) {
                                course.enrolledStudents.push_back(std::stoi(studentId));
                            }
                        }
                    }
                    
                    courses.push_back(course);
                }
            } catch (const std::exception& e) {
                std::cerr << "⚠️  Warning: Error parsing line " << lineNumber 
                         << " in " << filename << ": " << e.what() << "\n";
            }
        }
        
        file.close();
        std::cout << "✅ Loaded " << courses.size() << " courses from " << filename << "\n";
        return courses;
    }
    
    // Display methods
    void displayInfo() const {
        std::cout << "📚 Course Information:\n";
        std::cout << "   ID: " << id << "\n";
        std::cout << "   Name: " << name << "\n";
        std::cout << "   Description: " << (description.empty() ? "Not provided" : description) << "\n";
        std::cout << "   Credits: " << credits << "\n";
        std::cout << "   Enrollment: " << getCurrentEnrollment() << "/" << maxStudents << "\n";
        std::cout << "   Teacher ID: " << (hasTeacher() ? std::to_string(assignedTeacherId) : "Not assigned") << "\n";
        std::cout << "   Start Date: " << startDate << "\n";
        std::cout << "   End Date: " << (endDate.empty() ? "Ongoing" : endDate) << "\n";
        std::cout << "   Status: " << (isActive ? "Active" : "Inactive") << "\n";
        std::cout << "   Fee: $" << std::fixed << std::setprecision(2) << fee << "\n";
    }
    
    void displaySummary() const {
        std::cout << std::setw(5) << id << " | " 
                  << std::setw(25) << name.substr(0, 25) << " | "
                  << std::setw(3) << credits << " | "
                  << std::setw(8) << (std::to_string(getCurrentEnrollment()) + "/" + std::to_string(maxStudents)) << " | "
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

// Enhanced Classroom class
class Classroom {
private:
    int id;
    std::string location;
    int capacity;
    std::string building;
    std::vector<int> scheduledCourses;
    bool isAvailable;
    std::string equipment;
    
public:
    Classroom(int id, const std::string& location, int capacity, 
              const std::string& building = "", const std::string& equipment = "")
        : id(id), location(location), capacity(capacity), building(building), 
          equipment(equipment), isAvailable(true) {}
    
    // Getters
    int getId() const { return id; }
    const std::string& getLocation() const { return location; }
    int getCapacity() const { return capacity; }
    const std::string& getBuilding() const { return building; }
    const std::vector<int>& getScheduledCourses() const { return scheduledCourses; }
    bool getIsAvailable() const { return isAvailable; }
    const std::string& getEquipment() const { return equipment; }
    
    // Setters
    void setLocation(const std::string& newLocation) { 
        location = InputValidator::sanitizeString(newLocation); 
    }
    
    bool setCapacity(int newCapacity) {
        if (newCapacity > 0 && newCapacity <= 1000) {
            capacity = newCapacity;
            return true;
        }
        return false;
    }
    
    void setBuilding(const std::string& newBuilding) { 
        building = InputValidator::sanitizeString(newBuilding); 
    }
    
    void setIsAvailable(bool available) { isAvailable = available; }
    void setEquipment(const std::string& newEquipment) { 
        equipment = InputValidator::sanitizeString(newEquipment); 
    }
    
    // Course scheduling
    bool scheduleCourse(int courseId) {
        if (isAvailable && std::find(scheduledCourses.begin(), scheduledCourses.end(), courseId) == scheduledCourses.end()) {
            scheduledCourses.push_back(courseId);
            return true;
        }
        return false;
    }
    
    bool unscheduleCourse(int courseId) {
        auto it = std::find(scheduledCourses.begin(), scheduledCourses.end(), courseId);
        if (it != scheduledCourses.end()) {
            scheduledCourses.erase(it);
            return true;
        }
        return false;
    }
    
    bool isCourseScheduled(int courseId) const {
        return std::find(scheduledCourses.begin(), scheduledCourses.end(), courseId) != scheduledCourses.end();
    }
    
    // File operations
    bool saveToFile(const std::string& filename) const {
        std::ofstream file(filename, std::ios::app);
        if (!file.is_open()) {
            std::cerr << "❌ Error: Cannot open file " << filename << " for writing.\n";
            return false;
        }
        
        file << id << "|" << location << "|" << capacity << "|" << building << "|"
             << (isAvailable ? "1" : "0") << "|" << equipment << "|";
        
        // Save scheduled courses
        for (size_t i = 0; i < scheduledCourses.size(); ++i) {
            file << scheduledCourses[i];
            if (i < scheduledCourses.size() - 1) file << ",";
        }
        file << "\n";
        
        file.close();
        return true;
    }
    
    static std::vector<Classroom> loadFromFile(const std::string& filename) {
        std::vector<Classroom> classrooms;
        std::ifstream file(filename);
        
        if (!file.is_open()) {
            std::cout << "ℹ️  File " << filename << " not found. Starting with empty classroom list.\n";
            return classrooms;
        }
        
        std::string line;
        int lineNumber = 0;
        
        while (std::getline(file, line)) {
            lineNumber++;
            if (line.empty()) continue;
            
            try {
                std::vector<std::string> parts = split(line, '|');
                if (parts.size() >= 6) {
                    Classroom classroom(std::stoi(parts[0]), parts[1], std::stoi(parts[2]), parts[3], parts[5]);
                    classroom.isAvailable = (parts[4] == "1");
                    
                    // Load scheduled courses
                    if (parts.size() > 6 && !parts[6].empty()) {
                        std::vector<std::string> courseIds = split(parts[6], ',');
                        for (const auto& courseId : courseIds) {
                            if (!courseId.empty()) {
                                classroom.scheduledCourses.push_back(std::stoi(courseId));
                            }
                        }
                    }
                    
                    classrooms.push_back(classroom);
                }
            } catch (const std::exception& e) {
                std::cerr << "⚠️  Warning: Error parsing line " << lineNumber 
                         << " in " << filename << ": " << e.what() << "\n";
            }
        }
        
        file.close();
        std::cout << "✅ Loaded " << classrooms.size() << " classrooms from " << filename << "\n";
        return classrooms;
    }
    
    // Display methods
    void displayInfo() const {
        std::cout << "🏫 Classroom Information:\n";
        std::cout << "   ID: " << id << "\n";
        std::cout << "   Location: " << location << "\n";
        std::cout << "   Capacity: " << capacity << "\n";
        std::cout << "   Building: " << (building.empty() ? "Not specified" : building) << "\n";
        std::cout << "   Status: " << (isAvailable ? "Available" : "Unavailable") << "\n";
        std::cout << "   Equipment: " << (equipment.empty() ? "None specified" : equipment) << "\n";
        std::cout << "   Scheduled Courses: " << scheduledCourses.size() << "\n";
    }
    
    void displaySummary() const {
        std::cout << std::setw(5) << id << " | " 
                  << std::setw(15) << location.substr(0, 15) << " | "
                  << std::setw(8) << capacity << " | "
                  << std::setw(15) << building.substr(0, 15) << " | "
                  << std::setw(10) << (isAvailable ? "Available" : "Unavailable") << "\n";
    }
    
private:
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

// Main School Management System class
class IndustrialSchoolManagementSystem {
private:
    std::vector<Student> students;
    std::vector<Teacher> teachers;
    std::vector<Course> courses;
    std::vector<Classroom> classrooms;
    
    const std::string STUDENTS_FILE = "industrial_students.txt";
    const std::string TEACHERS_FILE = "industrial_teachers.txt";
    const std::string COURSES_FILE = "industrial_courses.txt";
    const std::string CLASSROOMS_FILE = "industrial_classrooms.txt";
    const std::string BACKUP_DIR = "backups/";
    
public:
    IndustrialSchoolManagementSystem() {
        loadAllData();
        std::cout << "🎓 Industrial School Management System v3.0 Initialized\n";
        std::cout << "======================================================\n";
        displaySystemStatus();
    }
    
    ~IndustrialSchoolManagementSystem() {
        saveAllData();
        createBackup();
    }
    
    void run() {
        int choice;
        do {
            showMainMenu();
            choice = SafeInput::getInt("Enter your choice (0-9): ", 0, 9);
            
            try {
                processChoice(choice);
            } catch (const std::exception& e) {
                std::cerr << "❌ Error: " << e.what() << "\n";
                SafeInput::waitForEnter("Press Enter to continue...");
            }
            
        } while (choice != 0);
        
        std::cout << "\n👋 Thank you for using Industrial School Management System!\n";
        std::cout << "All data has been saved automatically.\n";
    }
    
private:
    void showMainMenu() {
        clearScreen();
        std::cout << "🎓 INDUSTRIAL SCHOOL MANAGEMENT SYSTEM v3.0\n";
        std::cout << "============================================\n\n";
        
        displaySystemStatus();
        
        std::cout << "\n📋 MAIN MENU:\n";
        std::cout << "==============\n";
        std::cout << "1. 👨‍🎓 Student Management\n";
        std::cout << "2. 👩‍🏫 Teacher Management\n";
        std::cout << "3. 📚 Course Management\n";
        std::cout << "4. 🏫 Classroom Management\n";
        std::cout << "5. 📊 Reports & Analytics\n";
        std::cout << "6. 🔄 Data Management\n";
        std::cout << "7. 🎯 Quick Actions\n";
        std::cout << "8. ⚙️  System Settings\n";
        std::cout << "9. ℹ️  Help & About\n";
        std::cout << "0. 🚪 Exit System\n";
        std::cout << "==============\n";
    }
    
    void processChoice(int choice) {
        switch (choice) {
            case 1: studentManagement(); break;
            case 2: teacherManagement(); break;
            case 3: courseManagement(); break;
            case 4: classroomManagement(); break;
            case 5: reportsAndAnalytics(); break;
            case 6: dataManagement(); break;
            case 7: quickActions(); break;
            case 8: systemSettings(); break;
            case 9: helpAndAbout(); break;
            case 0: break;
            default: 
                std::cout << "❌ Invalid choice. Please try again.\n";
                SafeInput::waitForEnter();
        }
    }
    
    void studentManagement() {
        int choice;
        do {
            clearScreen();
            std::cout << "👨‍🎓 STUDENT MANAGEMENT\n";
            std::cout << "=======================\n\n";
            
            std::cout << "Current Students: " << students.size() << "\n";
            std::cout << "Active Students: " << countActiveStudents() << "\n\n";
            
            std::cout << "1. ➕ Add New Student\n";
            std::cout << "2. 📋 View All Students\n";
            std::cout << "3. 🔍 Search Student\n";
            std::cout << "4. ✏️  Edit Student\n";
            std::cout << "5. ❌ Deactivate Student\n";
            std::cout << "6. ✅ Reactivate Student\n";
            std::cout << "7. 📚 Enroll in Course\n";
            std::cout << "8. 📤 Unenroll from Course\n";
            std::cout << "9. 📊 Student Report\n";
            std::cout << "0. ⬅️  Back to Main Menu\n";
            
            choice = SafeInput::getInt("Enter your choice (0-9): ", 0, 9);
            
            switch (choice) {
                case 1: addStudent(); break;
                case 2: viewAllStudents(); break;
                case 3: searchStudent(); break;
                case 4: editStudent(); break;
                case 5: deactivateStudent(); break;
                case 6: reactivateStudent(); break;
                case 7: enrollStudentInCourse(); break;
                case 8: unenrollStudentFromCourse(); break;
                case 9: generateStudentReport(); break;
                case 0: break;
                default: 
                    std::cout << "❌ Invalid choice.\n";
                    SafeInput::waitForEnter();
            }
        } while (choice != 0);
    }
    
    void addStudent() {
        clearScreen();
        std::cout << "➕ ADD NEW STUDENT\n";
        std::cout << "==================\n\n";
        
        // Get and validate student ID
        int id;
        while (true) {
            id = SafeInput::getInt("Enter Student ID (1-999999): ", 1, 999999);
            if (findStudentById(id) == nullptr) {
                break;
            }
            std::cout << "❌ Student ID " << id << " already exists. Please choose a different ID.\n";
        }
        
        // Get student details with validation
        std::string name = SafeInput::getString("Enter Student Name: ");
        if (!InputValidator::isValidName(name)) {
            std::cout << "❌ Invalid name format.\n";
            SafeInput::waitForEnter();
            return;
        }
        
        int age = SafeInput::getInt("Enter Student Age (16-100): ", 16, 100);
        
        std::string email = SafeInput::getString("Enter Email (optional): ", false);
        if (!email.empty() && !InputValidator::isValidEmail(email)) {
            std::cout << "❌ Invalid email format.\n";
            SafeInput::waitForEnter();
            return;
        }
        
        std::string phone = SafeInput::getString("Enter Phone (optional): ", false);
        if (!phone.empty() && !InputValidator::isValidPhone(phone)) {
            std::cout << "❌ Invalid phone format.\n";
            SafeInput::waitForEnter();
            return;
        }
        
        std::string address = SafeInput::getString("Enter Address (optional): ", false);
        
        // Create and add student
        Student newStudent(id, name, age, email, phone, address);
        students.push_back(newStudent);
        
        // Save immediately
        if (newStudent.saveToFile(STUDENTS_FILE)) {
            std::cout << "\n✅ Student added successfully!\n";
            newStudent.displayInfo();
        } else {
            std::cout << "\n❌ Error saving student to file.\n";
        }
        
        SafeInput::waitForEnter();
    }
    
    void viewAllStudents() {
        clearScreen();
        std::cout << "📋 ALL STUDENTS\n";
        std::cout << "===============\n\n";
        
        if (students.empty()) {
            std::cout << "No students found.\n";
            SafeInput::waitForEnter();
            return;
        }
        
        // Display header
        std::cout << std::setw(5) << "ID" << " | " 
                  << std::setw(20) << "Name" << " | "
                  << std::setw(3) << "Age" << " | "
                  << std::setw(25) << "Email" << " | "
                  << std::setw(8) << "Status" << "\n";
        std::cout << std::string(70, '-') << "\n";
        
        // Display students
        for (const auto& student : students) {
            student.displaySummary();
        }
        
        std::cout << "\nTotal Students: " << students.size() << "\n";
        std::cout << "Active Students: " << countActiveStudents() << "\n";
        
        SafeInput::waitForEnter();
    }
    
    void searchStudent() {
        clearScreen();
        std::cout << "🔍 SEARCH STUDENT\n";
        std::cout << "=================\n\n";
        
        std::cout << "Search by:\n";
        std::cout << "1. Student ID\n";
        std::cout << "2. Student Name\n";
        
        int searchType = SafeInput::getInt("Enter search type (1-2): ", 1, 2);
        
        if (searchType == 1) {
            int id = SafeInput::getInt("Enter Student ID: ");
            Student* student = findStudentById(id);
            if (student) {
                std::cout << "\n✅ Student found:\n";
                student->displayInfo();
            } else {
                std::cout << "\n❌ Student with ID " << id << " not found.\n";
            }
        } else {
            std::string name = SafeInput::getString("Enter Student Name (or part of name): ");
            std::vector<Student*> foundStudents = searchStudentsByName(name);
            
            if (foundStudents.empty()) {
                std::cout << "\n❌ No students found with name containing '" << name << "'.\n";
            } else {
                std::cout << "\n✅ Found " << foundStudents.size() << " student(s):\n\n";
                for (Student* student : foundStudents) {
                    student->displayInfo();
                    std::cout << std::string(50, '-') << "\n";
                }
            }
        }
        
        SafeInput::waitForEnter();
    }
    
    // Additional helper methods
    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }
    
    void displaySystemStatus() {
        std::cout << "📊 System Status: ";
        std::cout << students.size() << " Students | ";
        std::cout << teachers.size() << " Teachers | ";
        std::cout << courses.size() << " Courses | ";
        std::cout << classrooms.size() << " Classrooms\n";
    }
    
    int countActiveStudents() {
        return std::count_if(students.begin(), students.end(),
            [](const Student& s) { return s.getIsActive(); });
    }
    
    int countActiveTeachers() {
        return std::count_if(teachers.begin(), teachers.end(),
            [](const Teacher& t) { return t.getIsActive(); });
    }
    
    Student* findStudentById(int id) {
        auto it = std::find_if(students.begin(), students.end(),
            [id](const Student& s) { return s.getId() == id; });
        return (it != students.end()) ? &(*it) : nullptr;
    }
    
    Teacher* findTeacherById(int id) {
        auto it = std::find_if(teachers.begin(), teachers.end(),
            [id](const Teacher& t) { return t.getId() == id; });
        return (it != teachers.end()) ? &(*it) : nullptr;
    }
    
    Course* findCourseById(int id) {
        auto it = std::find_if(courses.begin(), courses.end(),
            [id](const Course& c) { return c.getId() == id; });
        return (it != courses.end()) ? &(*it) : nullptr;
    }
    
    std::vector<Student*> searchStudentsByName(const std::string& name) {
        std::vector<Student*> results;
        std::string lowerName = name;
        std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
        
        for (auto& student : students) {
            std::string studentName = student.getName();
            std::transform(studentName.begin(), studentName.end(), studentName.begin(), ::tolower);
            if (studentName.find(lowerName) != std::string::npos) {
                results.push_back(&student);
            }
        }
        
        return results;
    }
    
    void loadAllData() {
        std::cout << "📂 Loading system data...\n";
        students = Student::loadFromFile(STUDENTS_FILE);
        teachers = Teacher::loadFromFile(TEACHERS_FILE);
        courses = Course::loadFromFile(COURSES_FILE);
        classrooms = Classroom::loadFromFile(CLASSROOMS_FILE);
        std::cout << "✅ Data loading completed.\n\n";
    }
    
    void saveAllData() {
        std::cout << "💾 Saving all data...\n";
        
        // Clear files and save all data
        clearFile(STUDENTS_FILE);
        clearFile(TEACHERS_FILE);
        clearFile(COURSES_FILE);
        clearFile(CLASSROOMS_FILE);
        
        for (const auto& student : students) {
            student.saveToFile(STUDENTS_FILE);
        }
        
        for (const auto& teacher : teachers) {
            teacher.saveToFile(TEACHERS_FILE);
        }
        
        for (const auto& course : courses) {
            course.saveToFile(COURSES_FILE);
        }
        
        for (const auto& classroom : classrooms) {
            classroom.saveToFile(CLASSROOMS_FILE);
        }
        
        std::cout << "✅ All data saved successfully.\n";
    }
    
    void clearFile(const std::string& filename) {
        std::ofstream file(filename, std::ios::trunc);
        file.close();
    }
    
    void createBackup() {
        // Create backup directory if it doesn't exist
        #ifdef _WIN32
            system(("mkdir " + BACKUP_DIR + " 2>nul").c_str());
        #else
            system(("mkdir -p " + BACKUP_DIR).c_str());
        #endif
        
        // Copy files to backup directory with timestamp
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y%m%d_%H%M%S");
        std::string timestamp = ss.str();
        
        std::cout << "🔄 Creating backup with timestamp: " << timestamp << "\n";
        // Backup implementation would go here
    }
    
    // Placeholder methods for other functionalities
    void teacherManagement() {
        int choice;
        do {
            clearScreen();
            std::cout << "👩‍🏫 TEACHER MANAGEMENT\n";
            std::cout << "========================\n\n";
            
            std::cout << "Current Teachers: " << teachers.size() << "\n";
            std::cout << "Active Teachers: " << countActiveTeachers() << "\n\n";
            
            std::cout << "1. ➕ Add New Teacher\n";
            std::cout << "2. 📋 View All Teachers\n";
            std::cout << "3. 🔍 Search Teacher\n";
            std::cout << "4. ✏️  Edit Teacher\n";
            std::cout << "5. ❌ Deactivate Teacher\n";
            std::cout << "6. ✅ Reactivate Teacher\n";
            std::cout << "7. 📚 Assign to Course\n";
            std::cout << "8. 📤 Unassign from Course\n";
            std::cout << "9. 📊 Teacher Report\n";
            std::cout << "0. ⬅️  Back to Main Menu\n\n";
            
            choice = SafeInput::getInt("Enter your choice: ");
            
            switch (choice) {
                case 1: addTeacher(); break;
                case 2: viewAllTeachers(); break;
                case 3: searchTeacher(); break;
                case 4: editTeacher(); break;
                case 5: deactivateTeacher(); break;
                case 6: reactivateTeacher(); break;
                case 7: assignTeacherToCourse(); break;
                case 8: unassignTeacherFromCourse(); break;
                case 9: generateTeacherReport(); break;
                case 0: break;
                default: 
                    std::cout << "❌ Invalid choice. Please try again.\n";
                    SafeInput::waitForEnter();
            }
        } while (choice != 0);
    }
    
    void addTeacher() {
        clearScreen();
        std::cout << "➕ ADD NEW TEACHER\n";
        std::cout << "==================\n\n";
        
        int id = teachers.empty() ? 1 : 
                 std::max_element(teachers.begin(), teachers.end(),
                     [](const Teacher& a, const Teacher& b) { return a.getId() < b.getId(); })->getId() + 1;
        
        std::string name = SafeInput::getString("Enter teacher name: ");
        std::string email = SafeInput::getString("Enter email address: ");
        std::string phone = SafeInput::getString("Enter phone number: ");
        std::string department = SafeInput::getString("Enter department: ");
        std::string specialization = SafeInput::getString("Enter specialization: ");
        
        Teacher newTeacher(id, name, email, phone, department, specialization);
        teachers.push_back(newTeacher);
        newTeacher.saveToFile(TEACHERS_FILE);
        
        std::cout << "✅ Teacher added successfully! ID: " << id << "\n";
        SafeInput::waitForEnter();
    }
    
    void viewAllTeachers() {
        clearScreen();
        std::cout << "📋 ALL TEACHERS\n";
        std::cout << "===============\n\n";
        
        if (teachers.empty()) {
            std::cout << "No teachers found.\n";
            SafeInput::waitForEnter();
            return;
        }
        
        std::cout << std::setw(5) << "ID" << " | " << std::setw(25) << "Name" << " | " 
                  << std::setw(20) << "Department" << " | " << std::setw(8) << "Status" << "\n";
        std::cout << std::string(65, '-') << "\n";
        
        for (const auto& teacher : teachers) {
            teacher.displaySummary();
        }
        
        SafeInput::waitForEnter();
    }
    
    void searchTeacher() {
        clearScreen();
        std::cout << "🔍 SEARCH TEACHER\n";
        std::cout << "=================\n\n";
        
        std::string searchTerm = SafeInput::getString("Enter teacher name or department to search: ");
        std::vector<Teacher*> results;
        
        for (auto& teacher : teachers) {
            std::string name = teacher.getName();
            std::string dept = teacher.getDepartment();
            std::transform(name.begin(), name.end(), name.begin(), ::tolower);
            std::transform(dept.begin(), dept.end(), dept.begin(), ::tolower);
            std::transform(searchTerm.begin(), searchTerm.end(), searchTerm.begin(), ::tolower);
            
            if (name.find(searchTerm) != std::string::npos || 
                dept.find(searchTerm) != std::string::npos) {
                results.push_back(&teacher);
            }
        }
        
        if (results.empty()) {
            std::cout << "No teachers found matching: " << searchTerm << "\n";
        } else {
            std::cout << "Found " << results.size() << " teacher(s):\n\n";
            for (auto* teacher : results) {
                teacher->displayInfo();
                std::cout << "\n";
            }
        }
        
        SafeInput::waitForEnter();
    }
    
    // Additional Course Management Functions
    void addCourse() {
        clearScreen();
        std::cout << "➕ ADD NEW COURSE\n";
        std::cout << "=================\n\n";
        
        int id = courses.empty() ? 1 : 
                 std::max_element(courses.begin(), courses.end(),
                     [](const Course& a, const Course& b) { return a.getId() < b.getId(); })->getId() + 1;
        
        std::string name = SafeInput::getString("Enter course name: ");
        std::string description = SafeInput::getString("Enter course description: ");
        int credits = SafeInput::getInt("Enter number of credits: ");
        int maxStudents = SafeInput::getInt("Enter maximum students: ");
        double fee = SafeInput::getDouble("Enter course fee: $");
        
        Course newCourse(id, name, description, credits, maxStudents, fee);
        courses.push_back(newCourse);
        newCourse.saveToFile(COURSES_FILE);
        
        std::cout << "✅ Course added successfully! ID: " << id << "\n";
        SafeInput::waitForEnter();
    }
    
    void viewAllCourses() {
        clearScreen();
        std::cout << "📋 ALL COURSES\n";
        std::cout << "==============\n\n";
        
        if (courses.empty()) {
            std::cout << "No courses found.\n";
            SafeInput::waitForEnter();
            return;
        }
        
        std::cout << std::setw(5) << "ID" << " | " << std::setw(25) << "Name" << " | " 
                  << std::setw(8) << "Credits" << " | " << std::setw(12) << "Enrollment" << " | "
                  << std::setw(8) << "Status" << "\n";
        std::cout << std::string(65, '-') << "\n";
        
        for (const auto& course : courses) {
            course.displaySummary();
        }
        
        SafeInput::waitForEnter();
    }
    
    void searchCourse() {
        clearScreen();
        std::cout << "🔍 SEARCH COURSE\n";
        std::cout << "================\n\n";
        
        std::string searchTerm = SafeInput::getString("Enter course name to search: ");
        std::vector<Course*> results;
        
        for (auto& course : courses) {
            std::string name = course.getName();
            std::transform(name.begin(), name.end(), name.begin(), ::tolower);
            std::transform(searchTerm.begin(), searchTerm.end(), searchTerm.begin(), ::tolower);
            
            if (name.find(searchTerm) != std::string::npos) {
                results.push_back(&course);
            }
        }
        
        if (results.empty()) {
            std::cout << "No courses found matching: " << searchTerm << "\n";
        } else {
            std::cout << "Found " << results.size() << " course(s):\n\n";
            for (auto* course : results) {
                course->displayInfo();
                std::cout << "\n";
            }
        }
        
        SafeInput::waitForEnter();
    }
    
    // Reports and Analytics Functions
    void generateSystemOverviewReport() {
        clearScreen();
        std::cout << "📈 SYSTEM OVERVIEW REPORT\n";
        std::cout << "=========================\n\n";
        
        std::cout << "📊 SYSTEM STATISTICS\n";
        std::cout << "====================\n";
        std::cout << "Total Students: " << students.size() << "\n";
        std::cout << "Active Students: " << countActiveStudents() << "\n";
        std::cout << "Total Teachers: " << teachers.size() << "\n";
        std::cout << "Active Teachers: " << countActiveTeachers() << "\n";
        std::cout << "Total Courses: " << courses.size() << "\n";
        std::cout << "Active Courses: " << std::count_if(courses.begin(), courses.end(),
            [](const Course& c) { return c.getIsActive(); }) << "\n";
        std::cout << "Total Classrooms: " << classrooms.size() << "\n\n";
        
        // Enrollment statistics
        int totalEnrollments = 0;
        double totalRevenue = 0.0;
        
        for (const auto& course : courses) {
            totalEnrollments += course.getCurrentEnrollment();
            totalRevenue += course.getCurrentEnrollment() * course.getFee();
        }
        
        std::cout << "📚 ENROLLMENT STATISTICS\n";
        std::cout << "========================\n";
        std::cout << "Total Enrollments: " << totalEnrollments << "\n";
        std::cout << "Total Revenue: $" << std::fixed << std::setprecision(2) << totalRevenue << "\n";
        std::cout << "Average Enrollments per Course: " << 
            (courses.empty() ? 0.0 : static_cast<double>(totalEnrollments) / courses.size()) << "\n\n";
        
        std::cout << "📅 Report Generated: " << getCurrentDate() << "\n";
        SafeInput::waitForEnter();
    }
    
    // Quick Action Functions
    void quickStudentSearch() {
        clearScreen();
        std::cout << "👤 QUICK STUDENT SEARCH\n";
        std::cout << "=======================\n\n";
        
        std::string searchTerm = SafeInput::getString("Enter student name: ");
        auto results = searchStudentsByName(searchTerm);
        
        if (results.empty()) {
            std::cout << "❌ No students found.\n";
        } else {
            std::cout << "✅ Found " << results.size() << " student(s):\n\n";
            for (auto* student : results) {
                std::cout << "ID: " << student->getId() << ", Name: " << student->getName() 
                         << ", Status: " << (student->getIsActive() ? "Active" : "Inactive") << "\n";
            }
        }
        
        SafeInput::waitForEnter();
    }
    
    void quickCourseLookup() {
        clearScreen();
        std::cout << "📚 QUICK COURSE LOOKUP\n";
        std::cout << "======================\n\n";
        
        int id = SafeInput::getInt("Enter Course ID: ");
        Course* course = findCourseById(id);
        
        if (course) {
            course->displayInfo();
        } else {
            std::cout << "❌ Course not found.\n";
        }
        
        SafeInput::waitForEnter();
    }
    
    void quickTeacherSearch() {
        clearScreen();
        std::cout << "👩‍🏫 QUICK TEACHER SEARCH\n";
        std::cout << "=========================\n\n";
        
        int id = SafeInput::getInt("Enter Teacher ID: ");
        Teacher* teacher = findTeacherById(id);
        
        if (teacher) {
            teacher->displayInfo();
        } else {
            std::cout << "❌ Teacher not found.\n";
        }
        
        SafeInput::waitForEnter();
    }
    
    void quickStudentEnrollment() {
        clearScreen();
        std::cout << "➕ QUICK STUDENT ENROLLMENT\n";
        std::cout << "===========================\n\n";
        
        enrollStudentInCourse();
    }
    
    // Placeholder implementations for remaining functions
    void editTeacher() { std::cout << "Teacher editing functionality coming soon...\n"; SafeInput::waitForEnter(); }
    void deactivateTeacher() { std::cout << "Teacher deactivation functionality coming soon...\n"; SafeInput::waitForEnter(); }
    void reactivateTeacher() { std::cout << "Teacher reactivation functionality coming soon...\n"; SafeInput::waitForEnter(); }
    void assignTeacherToCourse() { std::cout << "Teacher assignment functionality coming soon...\n"; SafeInput::waitForEnter(); }
    void unassignTeacherFromCourse() { std::cout << "Teacher unassignment functionality coming soon...\n"; SafeInput::waitForEnter(); }
    void generateTeacherReport() { std::cout << "Teacher report functionality coming soon...\n"; SafeInput::waitForEnter(); }
    
    void editCourse() { std::cout << "Course editing functionality coming soon...\n"; SafeInput::waitForEnter(); }
    void deactivateCourse() { std::cout << "Course deactivation functionality coming soon...\n"; SafeInput::waitForEnter(); }
    void reactivateCourse() { std::cout << "Course reactivation functionality coming soon...\n"; SafeInput::waitForEnter(); }
    void viewCourseEnrollments() { std::cout << "Course enrollment view functionality coming soon...\n"; SafeInput::waitForEnter(); }
    void generateCourseStatistics() { std::cout << "Course statistics functionality coming soon...\n"; SafeInput::waitForEnter(); }
    
    void generateStudentAnalytics() { std::cout << "Student analytics functionality coming soon...\n"; SafeInput::waitForEnter(); }
    void generateTeacherAnalytics() { std::cout << "Teacher analytics functionality coming soon...\n"; SafeInput::waitForEnter(); }
    void generateCourseAnalytics() { std::cout << "Course analytics functionality coming soon...\n"; SafeInput::waitForEnter(); }
    void generateFinancialSummary() { std::cout << "Financial summary functionality coming soon...\n"; SafeInput::waitForEnter(); }
    void generateEnrollmentTrends() { std::cout << "Enrollment trends functionality coming soon...\n"; SafeInput::waitForEnter(); }
    void generateCustomReport() { std::cout << "Custom report functionality coming soon...\n"; SafeInput::waitForEnter(); }
    void exportData() { std::cout << "Data export functionality coming soon...\n"; SafeInput::waitForEnter(); }
    
    void reloadAllData() { loadAllData(); }
    void restoreBackup() { std::cout << "Backup restoration functionality coming soon...\n"; SafeInput::waitForEnter(); }
    void exportToCSV() { std::cout << "CSV export functionality coming soon...\n"; SafeInput::waitForEnter(); }
    void importFromCSV() { std::cout << "CSV import functionality coming soon...\n"; SafeInput::waitForEnter(); }
    void performDataCleanup() { std::cout << "Data cleanup functionality coming soon...\n"; SafeInput::waitForEnter(); }
    void performDataIntegrityCheck() { std::cout << "Data integrity check functionality coming soon...\n"; SafeInput::waitForEnter(); }
    
    void generateTodaysSummary() { std::cout << "Today's summary functionality coming soon...\n"; SafeInput::waitForEnter(); }
    void performSystemHealthCheck() { std::cout << "System health check functionality coming soon...\n"; SafeInput::waitForEnter(); }
    
    void addClassroom() { std::cout << "Classroom addition functionality coming soon...\n"; SafeInput::waitForEnter(); }
    void viewAllClassrooms() { std::cout << "Classroom view functionality coming soon...\n"; SafeInput::waitForEnter(); }
    void searchClassroom() { std::cout << "Classroom search functionality coming soon...\n"; SafeInput::waitForEnter(); }
    void editClassroom() { std::cout << "Classroom editing functionality coming soon...\n"; SafeInput::waitForEnter(); }
    void markClassroomUnavailable() { std::cout << "Classroom unavailable marking functionality coming soon...\n"; SafeInput::waitForEnter(); }
    void markClassroomAvailable() { std::cout << "Classroom available marking functionality coming soon...\n"; SafeInput::waitForEnter(); }
    void manageClassroomSchedule() { std::cout << "Classroom schedule management functionality coming soon...\n"; SafeInput::waitForEnter(); }
    void generateClassroomUtilizationReport() { std::cout << "Classroom utilization report functionality coming soon...\n"; SafeInput::waitForEnter(); }
    
    void configurationSettings() { std::cout << "Configuration settings functionality coming soon...\n"; SafeInput::waitForEnter(); }
    void filePathSettings() { std::cout << "File path settings functionality coming soon...\n"; SafeInput::waitForEnter(); }
    void displayPreferences() { std::cout << "Display preferences functionality coming soon...\n"; SafeInput::waitForEnter(); }
    void securitySettings() { std::cout << "Security settings functionality coming soon...\n"; SafeInput::waitForEnter(); }
    void performanceSettings() { std::cout << "Performance settings functionality coming soon...\n"; SafeInput::waitForEnter(); }
    void resetToDefaults() { std::cout << "Reset to defaults functionality coming soon...\n"; SafeInput::waitForEnter(); }
    void viewCurrentSettings() { std::cout << "View current settings functionality coming soon...\n"; SafeInput::waitForEnter(); }
    void saveSettings() { std::cout << "Save settings functionality coming soon...\n"; SafeInput::waitForEnter(); }
};

// Main function
int main() {
    try {
        IndustrialSchoolManagementSystem system;
        system.run();
    } catch (const std::exception& e) {
        std::cerr << "💥 Fatal Error: " << e.what() << std::endl;
        std::cerr << "The system encountered a critical error and needs to exit.\n";
        std::cerr << "Please contact system administrator.\n";
        return 1;
    }
    
    return 0;
}
