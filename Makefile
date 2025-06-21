# School Management System Makefile
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
SRCDIR = src
INCDIR = include
TESTDIR = tests
OBJDIR = obj
BINDIR = bin
DATADIR = data
REPORTSDIR = reports

# Source files
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# Test files
TEST_SOURCES = $(wildcard $(TESTDIR)/*.cpp)
TEST_OBJECTS = $(TEST_SOURCES:$(TESTDIR)/%.cpp=$(OBJDIR)/test_%.o)

# Executables
MAIN_TARGET = $(BINDIR)/school_management
TEST_TARGET = $(BINDIR)/run_tests

# Default target
.PHONY: all clean test setup directories run run-tests help

all: directories $(MAIN_TARGET)

# Create necessary directories
directories:
	@mkdir -p $(OBJDIR) $(BINDIR) $(DATADIR) $(REPORTSDIR)

# Setup project structure
setup: directories
	@echo "Setting up project structure..."
	@touch $(DATADIR)/students.txt
	@touch $(DATADIR)/teachers.txt
	@touch $(DATADIR)/courses.txt
	@touch $(DATADIR)/classrooms.txt
	@echo "Project setup complete!"

# Main executable
$(MAIN_TARGET): $(OBJECTS)
	@echo "Linking main executable..."
	$(CXX) $(OBJECTS) -o $@
	@echo "Build complete: $(MAIN_TARGET)"

# Test executable
$(TEST_TARGET): $(TEST_OBJECTS) $(filter-out $(OBJDIR)/main.o, $(OBJECTS))
	@echo "Linking test executable..."
	$(CXX) $^ -o $@
	@echo "Test build complete: $(TEST_TARGET)"

# Object files for source
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Object files for tests
$(OBJDIR)/test_%.o: $(TESTDIR)/%.cpp
	@echo "Compiling test $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the main program
run: $(MAIN_TARGET)
	@echo "Running School Management System..."
	./$(MAIN_TARGET)

# Run tests
test: $(TEST_TARGET)
	@echo "Running unit tests..."
	./$(TEST_TARGET)

# Clean build files
clean:
	@echo "Cleaning build files..."
	rm -rf $(OBJDIR) $(BINDIR)
	@echo "Clean complete!"

# Clean all generated files
clean-all: clean
	@echo "Cleaning all generated files..."
	rm -rf $(DATADIR)/*.txt $(REPORTSDIR)/*.txt $(REPORTSDIR)/*.html
	@echo "All files cleaned!"

# Install dependencies (if any)
install-deps:
	@echo "No external dependencies required."

# Help target
help:
	@echo "Available targets:"
	@echo "  all          - Build the main application"
	@echo "  test         - Build and run unit tests"
	@echo "  setup        - Setup project directories and files"
	@echo "  run          - Run the main application"
	@echo "  run-tests    - Run unit tests"
	@echo "  clean        - Clean build files"
	@echo "  clean-all    - Clean all generated files"
	@echo "  help         - Show this help message"

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: all

# Release build
release: CXXFLAGS += -O2 -DNDEBUG
release: all
