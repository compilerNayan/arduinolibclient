#ifndef REST_TESTS_H
#define REST_TESTS_H

// Include test utilities
#include "TestUtils.h"

// Include all REST test files
#include "../controller_tests/WifiCredentialsControllerTests.h"
#include "../controller_tests/ResponseEntityControllerTests.h"
#include "../controller_tests/ExceptionTestControllerTests.h"

/**
 * Run all REST API test suites
 * 
 * This function consolidates and runs all REST API test suites:
 * - WifiCredentialsControllerTests
 * 
 * Additional REST tests can be added here in the future.
 * 
 * @param ip Server IP address (default: "localhost")
 * @param port Server port (default: "8080")
 * @return 0 if all tests passed, non-zero if any tests failed
 */
int RunAllRestTests(const std::string& ip, const std::string& port) {
    std_println("");
    std_println("========================================");
    std_println("  Running All REST API Test Suites");
    std_println("========================================");
    std_print("  Server: ");
    std_print(ip.c_str());
    std_print(":");
    std_println(port.c_str());
    std_println("");
    
    int totalFailed = 0;
    
    // Run WifiCredentialsControllerTests
    std_println("----------------------------------------");
    std_println("  WifiCredentialsControllerTests");
    std_println("----------------------------------------");
    int failed_wifi = RunAllWifiCredentialsControllerTests(ip, port);
    totalFailed += failed_wifi;
    std_println("");
    
    // Run ResponseEntityControllerTests
    std_println("----------------------------------------");
    std_println("  ResponseEntityControllerTests");
    std_println("----------------------------------------");
    int failed_response_entity = RunAllResponseEntityControllerTests(ip, port);
    totalFailed += failed_response_entity;
    std_println("");
    
    // Run ExceptionTestControllerTests
    std_println("----------------------------------------");
    std_println("  ExceptionTestControllerTests");
    std_println("----------------------------------------");
    int failed_exception = RunAllExceptionTestControllerTests(ip, port);
    totalFailed += failed_exception;
    std_println("");
    
    // Print final summary
    std_println("========================================");
    std_println("  REST API Test Suites Summary");
    std_println("========================================");
    std_print("Total tests failed: ");
    std_println(std::to_string(totalFailed).c_str());
    if (totalFailed == 0) {
        std_println("✅ All REST API test suites passed!");
    } else {
        std_println("❌ Some REST API test suites failed!");
    }
    std_println("========================================");
    std_println("");
    
    return totalFailed;
}

#endif // REST_TESTS_H

