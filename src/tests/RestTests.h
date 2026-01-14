#ifndef REST_TESTS_H
#define REST_TESTS_H

// Include test utilities
#include "TestUtils.h"

// Include all REST test files
#include "../controller_tests/WifiCredentialsControllerTests.h"

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
    RunAllWifiCredentialsControllerTests(ip, port);
    std_println("");
    
    // Print final summary
    std_println("========================================");
    std_println("  REST API Test Suites Summary");
    std_println("========================================");
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

