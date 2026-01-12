#ifndef TEST_UTILS_H
#define TEST_UTILS_H

// Conditionally include headers based on platform
#ifdef ARDUINO
    #include <Arduino.h>
    #include <string>
#else
    #include <iostream>
    #include <string>
#endif

// Print macros - compatible with both Arduino and non-Arduino
#ifdef ARDUINO
    // Arduino version using Serial.print/Serial.println
    #define std_print(x) Serial.print(x)
    #define std_println(x) Serial.println(x)
#else
    // Non-Arduino version using std::cout
    #define std_print(x) std::cout << x
    #define std_println(x) std::cout << x << std::endl
#endif

// Test helper macros - compatible with both Arduino and non-Arduino
#ifdef ARDUINO
    // Arduino version using std_print/std_println macros
    #define ASSERT(condition, message) \
        do { \
            if (!(condition)) { \
                std_print("❌ FAIL: "); \
                std_println(message); \
                return false; \
            } else { \
                std_print("✅ PASS: "); \
                std_println(message); \
            } \
        } while(0)

    #define TEST_START(testName) \
        std_print("\n========== "); \
        std_print(testName); \
        std_println(" ==========");
#else
    // Non-Arduino version using std_print/std_println macros
    #define ASSERT(condition, message) \
        do { \
            if (!(condition)) { \
                std_print("❌ FAIL: "); \
                std_println(message); \
                return false; \
            } else { \
                std_print("✅ PASS: "); \
                std_println(message); \
            } \
        } while(0)

    #define TEST_START(testName) \
        std_print("\n========== "); \
        std_print(testName); \
        std_println(" ==========");
#endif

// Test helper function - compatible with both Arduino and non-Arduino
// Overload for const char*
inline void PrintTestResult(const char* testName, bool passed) {
    std_print("Test: ");
    std_print(testName);
    std_print(" - ");
    if (passed) {
        std_println("PASSED");
    } else {
        std_println("FAILED");
    }
}

// Overload for std::string (works with StdString which is a typedef for std::string)
inline void PrintTestResult(const std::string& testName, bool passed) {
    PrintTestResult(testName.c_str(), passed);
}

#endif // TEST_UTILS_H

