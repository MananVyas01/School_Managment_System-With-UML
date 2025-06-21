#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <iostream>
#include <string>
#include <vector>
#include <functional>

class TestFramework {
private:
    struct TestCase {
        std::string name;
        std::function<void()> testFunction;
        bool passed;
        std::string errorMessage;
    };
    
    std::vector<TestCase> tests;
    int totalTests;
    int passedTests;
    int failedTests;
    
public:
    TestFramework();
    
    void addTest(const std::string& testName, std::function<void()> testFunction);
    void runAllTests();
    void runTest(const std::string& testName);
    void printResults();
    void printSummary();
    
    // Assertion methods
    static void assertTrue(bool condition, const std::string& message = "");
    static void assertFalse(bool condition, const std::string& message = "");
    static void assertEqual(int expected, int actual, const std::string& message = "");
    static void assertEqual(const std::string& expected, const std::string& actual, const std::string& message = "");
    static void assertNotEqual(int expected, int actual, const std::string& message = "");
    static void assertNotNull(void* ptr, const std::string& message = "");
    static void assertNull(void* ptr, const std::string& message = "");
    
private:
    void runSingleTest(TestCase& test);
};

// Macros for easier test writing
#define ASSERT_TRUE(condition) TestFramework::assertTrue(condition, #condition)
#define ASSERT_FALSE(condition) TestFramework::assertFalse(condition, #condition)
#define ASSERT_EQ(expected, actual) TestFramework::assertEqual(expected, actual, #expected " == " #actual)
#define ASSERT_NE(expected, actual) TestFramework::assertNotEqual(expected, actual, #expected " != " #actual)
#define ASSERT_NOT_NULL(ptr) TestFramework::assertNotNull(ptr, #ptr " is not null")
#define ASSERT_NULL(ptr) TestFramework::assertNull(ptr, #ptr " is null")

#endif // TEST_FRAMEWORK_H
