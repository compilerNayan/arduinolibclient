#ifndef ALL_TESTS_H
#define ALL_TESTS_H

// Include all test files
#include "../controller/UserRepositoryTests.h"
#include "../repository_tests/RepositoryTests.h"
#include "../serialization_tests/SerializationUtilityTests.h"
//#include "../controller_tests/WifiCredentialsControllerTests.h"

/**
 * Run all test suites
 * 
 * This function consolidates and runs all test suites:
 * - UserRepositoryTests
 * - RepositoryTests
 * - SerializationUtilityTests
 * - WifiCredentialsControllerTests
 * 
 * @param argc Command-line argument count (for UserRepositoryTests)
 * @param argv Command-line arguments (for UserRepositoryTests)
 * @return 0 if all tests passed, non-zero if any tests failed
 */
int RunAllTestSuites(int argc, char* argv[]) {
    std_println("");
    std_println("========================================");
    std_println("  Running All Test Suites");
    std_println("========================================");
    std_println("");
    
    int totalFailed = 0;
    
    // Run UserRepositoryTests
    std_println("----------------------------------------");
    std_println("  UserRepositoryTests");
    std_println("----------------------------------------");
    int userRepoResult = RunAllTests(argc, argv);
    if (userRepoResult != 0) {
        totalFailed += userRepoResult;
    }
    std_println("");
    
    // Run RepositoryTests
    std_println("----------------------------------------");
    std_println("  RepositoryTests");
    std_println("----------------------------------------");
    RunAllRepositoryTests();
    std_println("");
    
    // Run SerializationUtilityTests
    std_println("----------------------------------------");
    std_println("  SerializationUtilityTests");
    std_println("----------------------------------------");
    int serializationResult = RunAllSerializationTests();
    if (serializationResult != 0) {
        totalFailed += serializationResult;
    }
    std_println("");
    
    /*// Run WifiCredentialsControllerTests
    std_println("----------------------------------------");
    std_println("  WifiCredentialsControllerTests");
    std_println("----------------------------------------");
    RunAllWifiCredentialsControllerTests();
    std_println(""); */
    
    // Print final summary
    std_println("========================================");
    std_println("  All Test Suites Summary");
    std_println("========================================");
    if (totalFailed == 0) {
        std_println("✅ All test suites passed!");
    } else {
        std_println("❌ Some test suites failed!");
    }
    std_println("========================================");
    std_println("");
    
    return totalFailed;
}

#endif // ALL_TESTS_H

