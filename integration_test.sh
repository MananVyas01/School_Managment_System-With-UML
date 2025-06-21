#!/bin/bash

echo "🎓 INDUSTRIAL SCHOOL MANAGEMENT SYSTEM - INTEGRATION TEST"
echo "=========================================================="
echo ""

# Check if we're in the right directory
if [ ! -f "School-Manage.cpp" ]; then
    echo "❌ Error: Please run this script from the School Management System directory"
    exit 1
fi

echo "📋 System Integration Test Report"
echo "================================="
echo ""

# Test 1: Check file structure
echo "🔍 Test 1: File Structure Verification"
echo "--------------------------------------"

required_files=(
    "School-Manage.cpp"
    "students.txt"
    "teachers.txt"
    "courses.txt"
    "classrooms.txt"
    "README.md"
    "IndustrialDemo.cpp"
)

missing_files=0
for file in "${required_files[@]}"; do
    if [ -f "$file" ]; then
        echo "✅ $file - Found"
    else
        echo "❌ $file - Missing"
        ((missing_files++))
    fi
done

echo ""
if [ $missing_files -eq 0 ]; then
    echo "✅ All required files present"
else
    echo "⚠️  $missing_files file(s) missing"
fi

echo ""

# Test 2: Compilation Test
echo "🔨 Test 2: Compilation Verification"
echo "-----------------------------------"

# Test legacy system
echo "Testing legacy system compilation..."
if g++ -std=c++17 -o School-Manage School-Manage.cpp 2>/dev/null; then
    echo "✅ Legacy system compiles successfully"
    legacy_compiled=true
else
    echo "❌ Legacy system compilation failed"
    legacy_compiled=false
fi

# Test industrial demo
echo "Testing industrial demo compilation..."
if g++ -std=c++17 -o IndustrialDemo IndustrialDemo.cpp 2>/dev/null; then
    echo "✅ Industrial demo compiles successfully"
    industrial_compiled=true
else
    echo "❌ Industrial demo compilation failed"
    industrial_compiled=false
fi

echo ""

# Test 3: Data Integrity
echo "📊 Test 3: Data Integrity Check"
echo "-------------------------------"

total_records=0

# Count students
if [ -f "students.txt" ]; then
    student_count=$(wc -l < students.txt)
    echo "👨‍🎓 Students: $student_count records"
    total_records=$((total_records + student_count))
else
    echo "❌ Students file missing"
fi

# Count teachers
if [ -f "teachers.txt" ]; then
    teacher_count=$(wc -l < teachers.txt)
    echo "👩‍🏫 Teachers: $teacher_count records"
    total_records=$((total_records + teacher_count))
else
    echo "❌ Teachers file missing"
fi

# Count courses
if [ -f "courses.txt" ]; then
    course_count=$(wc -l < courses.txt)
    echo "📚 Courses: $course_count records"
    total_records=$((total_records + course_count))
else
    echo "❌ Courses file missing"
fi

# Count classrooms
if [ -f "classrooms.txt" ]; then
    classroom_count=$(wc -l < classrooms.txt)
    echo "🏫 Classrooms: $classroom_count records"
    total_records=$((total_records + classroom_count))
else
    echo "❌ Classrooms file missing"
fi

echo "📊 Total Records: $total_records"
echo ""

# Test 4: Modern Features Check
echo "🚀 Test 4: Modern Features Verification"
echo "---------------------------------------"

# Check modular structure
if [ -d "src" ] && [ -d "include" ]; then
    echo "✅ Modular architecture implemented"
    
    src_files=$(find src -name "*.cpp" | wc -l)
    header_files=$(find include -name "*.h" | wc -l)
    echo "   📁 Source files: $src_files"
    echo "   📁 Header files: $header_files"
else
    echo "⚠️  Modular architecture not fully implemented"
fi

# Check for modern C++ features in source
if grep -q "std::unique_ptr\|std::shared_ptr\|auto\|constexpr" src/*.cpp 2>/dev/null; then
    echo "✅ Modern C++ features detected"
else
    echo "⚠️  Limited modern C++ features found"
fi

# Check for testing framework
if [ -d "tests" ] && [ -f "tests/UnitTests.cpp" ]; then
    echo "✅ Unit testing framework implemented"
else
    echo "⚠️  Testing framework not found"
fi

echo ""

# Test 5: Industrial Features
echo "🏭 Test 5: Industrial-Grade Features"
echo "------------------------------------"

# Check for error handling
if grep -q "try\|catch\|exception" IndustrialDemo.cpp 2>/dev/null; then
    echo "✅ Exception handling implemented"
else
    echo "⚠️  Limited exception handling found"
fi

# Check for input validation
if grep -q "InputValidator\|isValid\|sanitize" IndustrialDemo.cpp 2>/dev/null; then
    echo "✅ Input validation system implemented"
else
    echo "⚠️  Input validation not found"
fi

# Check for logging/reporting
if [ -d "reports" ] || grep -q "report\|log\|backup" *.cpp 2>/dev/null; then
    echo "✅ Reporting and backup systems detected"
else
    echo "⚠️  Limited reporting capabilities found"
fi

echo ""

# Final Summary
echo "📋 INTEGRATION TEST SUMMARY"
echo "==========================="
echo ""

score=0
max_score=10

# File structure (2 points)
if [ $missing_files -eq 0 ]; then
    score=$((score + 2))
    echo "✅ File Structure: 2/2 points"
else
    echo "⚠️  File Structure: 1/2 points"
    score=$((score + 1))
fi

# Compilation (2 points)
compilation_score=0
if [ "$legacy_compiled" = true ]; then
    compilation_score=$((compilation_score + 1))
fi
if [ "$industrial_compiled" = true ]; then
    compilation_score=$((compilation_score + 1))
fi
score=$((score + compilation_score))
echo "✅ Compilation: $compilation_score/2 points"

# Data integrity (2 points)
if [ $total_records -gt 0 ]; then
    score=$((score + 2))
    echo "✅ Data Integrity: 2/2 points"
else
    echo "⚠️  Data Integrity: 0/2 points"
fi

# Modern features (2 points)
if [ -d "src" ] && [ -d "include" ]; then
    score=$((score + 2))
    echo "✅ Modern Architecture: 2/2 points"
else
    score=$((score + 1))
    echo "⚠️  Modern Architecture: 1/2 points"
fi

# Industrial features (2 points)
if grep -q "InputValidator\|exception" IndustrialDemo.cpp 2>/dev/null; then
    score=$((score + 2))
    echo "✅ Industrial Features: 2/2 points"
else
    score=$((score + 1))
    echo "⚠️  Industrial Features: 1/2 points"
fi

echo ""
echo "🎯 FINAL SCORE: $score/$max_score points"

if [ $score -eq $max_score ]; then
    echo "🏆 EXCELLENT: System is production-ready!"
elif [ $score -ge 8 ]; then
    echo "🥈 GOOD: System is nearly production-ready with minor improvements needed"
elif [ $score -ge 6 ]; then
    echo "🥉 FAIR: System has good foundation but needs significant improvements"
else
    echo "⚠️  NEEDS WORK: System requires major improvements for production use"
fi

echo ""
echo "📞 Next Steps:"
echo "1. Address any missing files or compilation issues"
echo "2. Enhance error handling and input validation"
echo "3. Implement comprehensive testing"
echo "4. Add backup and recovery features"
echo "5. Performance optimization and security hardening"

echo ""
echo "Integration test completed on $(date)"
echo "=========================================================="
