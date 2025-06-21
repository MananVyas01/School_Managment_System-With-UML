#!/bin/bash

echo "🎓 School Management System Demo"
echo "================================"
echo ""

echo "📊 Current Data Status:"
echo "Students: $(cat students.txt 2>/dev/null | wc -l) records"
echo "Teachers: $(cat teachers.txt 2>/dev/null | wc -l) records"
echo "Courses: $(cat courses.txt 2>/dev/null | wc -l) records"
echo "Classrooms: $(cat classrooms.txt 2>/dev/null | wc -l) records"
echo ""

echo "📋 Available Menu Options:"
echo "1. Add Student - Register new students"
echo "2. Add Teacher - Add faculty members"
echo "3. Add Course - Create new courses"
echo "4. Add Classroom - Set up classrooms"
echo "5. Assign Teacher to Course - Link teachers with courses"
echo "6. Enroll Student in Course - Register students for classes"
echo "7. Record Attendance - Track student attendance"
echo "8. Exit - Close the application"
echo ""

echo "🔥 Example Usage:"
echo "To add a student: Choose 1, then enter ID, Name, Age, Course"
echo "To assign a teacher: Choose 5, then enter Course ID and Teacher ID"
echo "To record attendance: Choose 7, then enter Course ID, Student ID, and presence (1/0)"
echo ""

echo "▶️  Run: ./School-Manage to start the interactive system"
echo "🎯 The system automatically saves all data to text files"
