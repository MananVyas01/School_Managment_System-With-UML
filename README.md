# 🎓 School Management System v2.0

A comprehensive **C++ School Management System** with advanced features, modern architecture, and professional development practices.

## ✨ New Features & Improvements

### 🏗️ **Architecture Improvements**
- **Modular Design**: Separated into header files and implementation files
- **Modern C++17**: Smart pointers, const correctness, and RAII principles
- **Object-Oriented Design**: Proper encapsulation, inheritance, and polymorphism
- **Configuration System**: Centralized settings with singleton pattern

### 🧪 **Testing & Quality Assurance**
- **Custom Unit Testing Framework**: Built-in assertion methods and test runners
- **Comprehensive Test Coverage**: Tests for all core classes and functionality
- **Automated Testing**: Integrated with build system (`make test`)
- **Test Reporting**: Detailed pass/fail statistics and error reporting

### 📊 **Advanced Features**
- **Reports & Statistics**: Comprehensive reporting system with multiple formats
- **Attendance Analytics**: Statistical analysis of attendance patterns
- **Performance Metrics**: Student, teacher, and course performance tracking
- **Data Visualization**: Professional report generation with timestamps

### 🛠️ **Developer Experience**
- **Professional Build System**: Makefile with multiple targets and configurations
- **Data Migration Tools**: Automated migration from legacy data format
- **Backup System**: Automatic backup creation during data operations
- **Development Workflow**: Debug/release builds, clean targets, and help system

### � **System Features**
- **Enhanced Data Model**: Email, phone, department fields for entities
- **Validation System**: Input validation and data integrity checks
- **Error Handling**: Comprehensive error handling and user feedback
- **File Management**: Configurable file paths and organized data storage

## 📁 Project Structure

```
School_Managment_System-With-UML/
├── include/               # Header files
│   ├── Config.h          # Configuration management
│   ├── Student.h         # Student class definition
│   ├── Teacher.h         # Teacher class definition
│   ├── Course.h          # Course class definition
│   ├── Classroom.h       # Classroom class definition
│   ├── Reports.h         # Reporting system
│   └── TestFramework.h   # Unit testing framework
├── src/                  # Implementation files
│   ├── main.cpp          # Main application
│   ├── Config.cpp        # Configuration implementation
│   ├── TestFramework.cpp # Testing framework implementation
│   └── test_runner.cpp   # Test execution
├── tests/                # Unit tests
│   └── UnitTests.cpp     # Comprehensive test suite
├── data/                 # Data files
│   ├── students.txt      # Student records
│   ├── teachers.txt      # Teacher records
│   ├── courses.txt       # Course information
│   └── classrooms.txt    # Classroom data
├── reports/              # Generated reports
├── backup/               # Backup files
├── config.txt            # System configuration
├── Makefile             # Build system
├── migrate_data.sh      # Data migration script
└── README.md            # This file
```

## � Quick Start

### Prerequisites
- C++17 compatible compiler (g++, clang++)
- Make build system
- Linux/Unix environment (or WSL on Windows)

### Installation & Setup
```bash
# Clone or navigate to the project directory
cd School_Managment_System-With-UML

# Setup project structure
make setup

# Build the application
make all

# Run unit tests
make test

# Run the application
make run
```

### Development Workflow
```bash
# Build and test
make all && make test

# Debug build
make debug

# Clean and rebuild
make clean && make all

# View all available targets
make help
```

## 🧪 Testing

The project includes a comprehensive unit testing framework:

```bash
# Run all tests
make test

# Run specific test categories
./bin/run_tests
```

### Test Coverage
- ✅ Student creation, validation, and management
- ✅ Teacher assignment and course management
- ✅ Course enrollment and capacity limits
- ✅ Configuration system functionality
- ✅ Data validation and error handling

## 📊 Reports & Analytics

The system provides comprehensive reporting capabilities:

### Available Reports
- **Student Reports**: Individual and batch student information
- **Teacher Reports**: Workload analysis and assignment tracking
- **Course Reports**: Enrollment statistics and performance metrics
- **Attendance Reports**: Attendance patterns and statistics
- **System Statistics**: Overall system metrics and insights

### Report Features
- Multiple output formats (HTML, text)
- Automatic timestamp generation
- Professional formatting
- Statistical analysis
- Data visualization

## ⚙️ Configuration

The system uses a flexible configuration system:

```
# config.txt
students_file=data/students.txt
teachers_file=data/teachers.txt
courses_file=data/courses.txt
classrooms_file=data/classrooms.txt
reports_dir=reports/
max_students_per_course=50
backup_enabled=true
```

## 🔄 Data Migration

For existing installations:

```bash
# Run migration script
./migrate_data.sh
```

This will:
- Backup existing data
- Move files to new structure
- Create necessary directories
- Generate .gitignore file

## 🎯 Future Enhancements

The architecture supports easy extension:

- **Database Integration**: Ready for MySQL/PostgreSQL integration
- **Web Interface**: RESTful API endpoints can be added
- **Mobile App**: JSON export/import for mobile applications
- **Advanced Analytics**: Machine learning for predictive analytics
- **Notification System**: Email/SMS notifications for events
- **Multi-tenant Support**: Support for multiple schools

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch
3. Run tests: `make test`
4. Commit changes with descriptive messages
5. Submit a pull request

## 📝 License

This project is licensed under the MIT License.

## 🙏 Acknowledgements

- Modern C++ best practices and design patterns
- Professional software development methodologies
- Educational institution management requirements
- Open source testing frameworks and tools
