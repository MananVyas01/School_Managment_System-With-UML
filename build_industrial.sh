#!/bin/bash

# Simple compilation script for Industrial School Management System

echo "🔨 Compiling Industrial School Management System..."

# Clean any existing executable
rm -f IndustrialSchoolSystem

# Compile the industrial system
if g++ -std=c++17 -Wall -Wextra -O2 IndustrialSchoolSystem.cpp -o IndustrialSchoolSystem; then
    echo "✅ Compilation successful!"
    echo "🚀 Executable created: IndustrialSchoolSystem"
    ls -la IndustrialSchoolSystem
else
    echo "❌ Compilation failed!"
    echo "📋 Trying with debugging info..."
    g++ -std=c++17 -Wall -Wextra -g IndustrialSchoolSystem.cpp -o IndustrialSchoolSystem 2>&1
fi
