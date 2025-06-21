#include "../include/TestFramework.h"
#include <stdexcept>

TestFramework::TestFramework() : totalTests(0), passedTests(0), failedTests(0) {}

void TestFramework::addTest(const std::string& testName, std::function<void()> testFunction) {
    tests.push_back({testName, testFunction, false, ""});
    totalTests++;
}

void TestFramework::runAllTests() {
    std::cout << "Running " << totalTests << " tests...\n";
    std::cout << "=====================================\n";
    
    for (auto& test : tests) {
        runSingleTest(test);
    }
    
    printSummary();
}

void TestFramework::runTest(const std::string& testName) {
    for (auto& test : tests) {
        if (test.name == testName) {
            runSingleTest(test);
            return;
        }
    }
    std::cout << "Test '" << testName << "' not found!\n";
}

void TestFramework::runSingleTest(TestCase& test) {
    std::cout << "Running: " << test.name << " ... ";
    
    try {
        test.testFunction();
        test.passed = true;
        passedTests++;
        std::cout << "PASSED\n";
    } catch (const std::exception& e) {
        test.passed = false;
        test.errorMessage = e.what();
        failedTests++;
        std::cout << "FAILED: " << e.what() << "\n";
    } catch (...) {
        test.passed = false;
        test.errorMessage = "Unknown exception";
        failedTests++;
        std::cout << "FAILED: Unknown exception\n";
    }
}

void TestFramework::printResults() {
    std::cout << "\n=== Test Results ===\n";
    for (const auto& test : tests) {
        std::cout << test.name << ": " << (test.passed ? "PASSED" : "FAILED");
        if (!test.passed && !test.errorMessage.empty()) {
            std::cout << " (" << test.errorMessage << ")";
        }
        std::cout << "\n";
    }
}

void TestFramework::printSummary() {
    std::cout << "\n=== Summary ===\n";
    std::cout << "Total tests: " << totalTests << "\n";
    std::cout << "Passed: " << passedTests << "\n";
    std::cout << "Failed: " << failedTests << "\n";
    std::cout << "Success rate: " << (totalTests > 0 ? (passedTests * 100.0 / totalTests) : 0) << "%\n";
}

void TestFramework::assertTrue(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message.empty() ? "Assertion failed: expected true" : message);
    }
}

void TestFramework::assertFalse(bool condition, const std::string& message) {
    if (condition) {
        throw std::runtime_error(message.empty() ? "Assertion failed: expected false" : message);
    }
}

void TestFramework::assertEqual(int expected, int actual, const std::string& message) {
    if (expected != actual) {
        throw std::runtime_error(message.empty() ? 
            "Assertion failed: expected " + std::to_string(expected) + " but got " + std::to_string(actual) : 
            message);
    }
}

void TestFramework::assertEqual(const std::string& expected, const std::string& actual, const std::string& message) {
    if (expected != actual) {
        throw std::runtime_error(message.empty() ? 
            "Assertion failed: expected '" + expected + "' but got '" + actual + "'" : 
            message);
    }
}

void TestFramework::assertNotEqual(int expected, int actual, const std::string& message) {
    if (expected == actual) {
        throw std::runtime_error(message.empty() ? 
            "Assertion failed: expected not equal to " + std::to_string(expected) : 
            message);
    }
}

void TestFramework::assertNotNull(void* ptr, const std::string& message) {
    if (ptr == nullptr) {
        throw std::runtime_error(message.empty() ? "Assertion failed: pointer is null" : message);
    }
}

void TestFramework::assertNull(void* ptr, const std::string& message) {
    if (ptr != nullptr) {
        throw std::runtime_error(message.empty() ? "Assertion failed: pointer is not null" : message);
    }
}
