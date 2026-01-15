#ifndef EXCEPTION_TEST_CONTROLLER_TESTS_H
#define EXCEPTION_TEST_CONTROLLER_TESTS_H

// Conditionally include headers based on platform
#ifdef ARDUINO
    #include <Arduino.h>
    #include <string>
    #include <vector>
#else
    #include <iostream>
    #include <cassert>
    #include <string>
    #include <vector>
#endif

#include <StandardDefines.h>
#include <SerializationUtility.h>
#include "http_client/ISpecialHttpClient.h"
#include "../tests/TestUtils.h"

using namespace nayan::serializer;

// Test assertion macros (using common ASSERT macro from TestUtils.h)
#define ASSERT_EXCEPTION(condition, message) ASSERT(condition, message)

#define TEST_EXCEPTION_START(test_name) TEST_START(test_name)

// Base URL for the REST API (will be set by RunAllExceptionTestControllerTests)
static StdString BASE_URL_EXCEPTION_TEST;

// Global test counters
static int testsPassed_exception = 0;
static int testsFailed_exception = 0;

// Helper function to print test result and update counters
inline void PrintExceptionTestResult(const char* testName, bool passed) {
    ::PrintTestResult(testName, passed);
    if (passed) {
        testsPassed_exception++;
    } else {
        testsFailed_exception++;
    }
}

// Note: Using HttpResponse, ParseHttpResponse, and GetHttpClient from WifiCredentialsControllerTests.h
// They are shared helpers available when both test files are included

// ========== EXCEPTION HANDLING TESTS ==========

// Test 1: Throw std::runtime_error - Should return 500 with error message
bool TestThrowRuntimeException() {
    TEST_EXCEPTION_START("Test Throw Runtime Exception - 500 with message");
    
    ISpecialHttpClientPtr httpClient = GetHttpClient();
    if (!httpClient) {
        std_println("FAILED - HTTP client is null!");
        PrintExceptionTestResult("Throw Runtime Exception", false);
        return false;
    }
    
    StdString url = BASE_URL_EXCEPTION_TEST + "/runtime-error";
    std_print("[DEBUG] URL: ");
    std_println(url.c_str());
    
    StdString responseJson = httpClient->Get(url);
    std_print("[DEBUG] Response received: ");
    std_println(responseJson.c_str());
    
    HttpResponse response = ParseHttpResponse(responseJson);
    std_print("[DEBUG] Status code: ");
    std_println(std::to_string(response.statusCode).c_str());
    std_print("[DEBUG] Body: ");
    std_println(response.body.c_str());
    
    // Should return 500 Internal Server Error
    ASSERT_EXCEPTION(response.statusCode == 500, 
                     "HTTP status should be 500 Internal Server Error");
    
    // Body should contain error information
    bool hasError = response.body.find("error") != StdString::npos || 
                    response.body.find("Internal Server Error") != StdString::npos;
    ASSERT_EXCEPTION(hasError, "Body should contain error information");
    
    // Body should contain the exception message
    bool hasMessage = response.body.find("runtime error") != StdString::npos ||
                      response.body.find("detailed message") != StdString::npos;
    ASSERT_EXCEPTION(hasMessage, "Body should contain the exception message");
    
    PrintExceptionTestResult("Throw Runtime Exception", true);
    return true;
}

// Test 2: Throw std::logic_error - Should return 500 with error message
bool TestThrowLogicException() {
    TEST_EXCEPTION_START("Test Throw Logic Exception - 500 with message");
    
    ISpecialHttpClientPtr httpClient = GetHttpClient();
    if (!httpClient) {
        std_println("FAILED - HTTP client is null!");
        PrintExceptionTestResult("Throw Logic Exception", false);
        return false;
    }
    
    StdString url = BASE_URL_EXCEPTION_TEST + "/logic-error";
    std_print("[DEBUG] URL: ");
    std_println(url.c_str());
    
    StdString responseJson = httpClient->Get(url);
    std_print("[DEBUG] Response received: ");
    std_println(responseJson.c_str());
    
    HttpResponse response = ParseHttpResponse(responseJson);
    std_print("[DEBUG] Status code: ");
    std_println(std::to_string(response.statusCode).c_str());
    std_print("[DEBUG] Body: ");
    std_println(response.body.c_str());
    
    // Should return 500 Internal Server Error
    ASSERT_EXCEPTION(response.statusCode == 500, 
                     "HTTP status should be 500 Internal Server Error");
    
    // Body should contain error information
    bool hasError = response.body.find("error") != StdString::npos || 
                    response.body.find("Internal Server Error") != StdString::npos;
    ASSERT_EXCEPTION(hasError, "Body should contain error information");
    
    // Body should contain the exception message
    bool hasMessage = response.body.find("logic error") != StdString::npos ||
                      response.body.find("invalid operation") != StdString::npos;
    ASSERT_EXCEPTION(hasMessage, "Body should contain the exception message");
    
    PrintExceptionTestResult("Throw Logic Exception", true);
    return true;
}

// Test 3: Throw const char* - Should return 500 with generic message (catch-all)
bool TestThrowStringException() {
    TEST_EXCEPTION_START("Test Throw String Exception - 500 with generic message");
    
    ISpecialHttpClientPtr httpClient = GetHttpClient();
    if (!httpClient) {
        std_println("FAILED - HTTP client is null!");
        PrintExceptionTestResult("Throw String Exception", false);
        return false;
    }
    
    StdString url = BASE_URL_EXCEPTION_TEST + "/string-exception";
    std_print("[DEBUG] URL: ");
    std_println(url.c_str());
    
    StdString responseJson = httpClient->Get(url);
    std_print("[DEBUG] Response received: ");
    std_println(responseJson.c_str());
    
    HttpResponse response = ParseHttpResponse(responseJson);
    std_print("[DEBUG] Status code: ");
    std_println(std::to_string(response.statusCode).c_str());
    std_print("[DEBUG] Body: ");
    std_println(response.body.c_str());
    
    // Should return 500 Internal Server Error
    ASSERT_EXCEPTION(response.statusCode == 500, 
                     "HTTP status should be 500 Internal Server Error");
    
    // Body should contain error information
    bool hasError = response.body.find("error") != StdString::npos || 
                    response.body.find("Internal Server Error") != StdString::npos;
    ASSERT_EXCEPTION(hasError, "Body should contain error information");
    
    // For catch-all exceptions, we get "Unknown exception occurred"
    bool hasUnknownMessage = response.body.find("Unknown exception") != StdString::npos ||
                             response.body.find("Unknown") != StdString::npos;
    ASSERT_EXCEPTION(hasUnknownMessage, "Body should contain 'Unknown exception occurred' for catch-all");
    
    PrintExceptionTestResult("Throw String Exception", true);
    return true;
}

// Test 4: Throw int - Should return 500 with generic message (catch-all)
bool TestThrowIntException() {
    TEST_EXCEPTION_START("Test Throw Int Exception - 500 with generic message");
    
    ISpecialHttpClientPtr httpClient = GetHttpClient();
    if (!httpClient) {
        std_println("FAILED - HTTP client is null!");
        PrintExceptionTestResult("Throw Int Exception", false);
        return false;
    }
    
    StdString url = BASE_URL_EXCEPTION_TEST + "/int-exception";
    std_print("[DEBUG] URL: ");
    std_println(url.c_str());
    
    StdString responseJson = httpClient->Get(url);
    std_print("[DEBUG] Response received: ");
    std_println(responseJson.c_str());
    
    HttpResponse response = ParseHttpResponse(responseJson);
    std_print("[DEBUG] Status code: ");
    std_println(std::to_string(response.statusCode).c_str());
    std_print("[DEBUG] Body: ");
    std_println(response.body.c_str());
    
    // Should return 500 Internal Server Error
    ASSERT_EXCEPTION(response.statusCode == 500, 
                     "HTTP status should be 500 Internal Server Error");
    
    // Body should contain error information
    bool hasError = response.body.find("error") != StdString::npos || 
                    response.body.find("Internal Server Error") != StdString::npos;
    ASSERT_EXCEPTION(hasError, "Body should contain error information");
    
    // For catch-all exceptions, we get "Unknown exception occurred"
    bool hasUnknownMessage = response.body.find("Unknown exception") != StdString::npos ||
                             response.body.find("Unknown") != StdString::npos;
    ASSERT_EXCEPTION(hasUnknownMessage, "Body should contain 'Unknown exception occurred' for catch-all");
    
    PrintExceptionTestResult("Throw Int Exception", true);
    return true;
}

// Test 5: Throw custom exception - Should return 500 with generic message (catch-all)
bool TestThrowCustomException() {
    TEST_EXCEPTION_START("Test Throw Custom Exception - 500 with generic message");
    
    ISpecialHttpClientPtr httpClient = GetHttpClient();
    if (!httpClient) {
        std_println("FAILED - HTTP client is null!");
        PrintExceptionTestResult("Throw Custom Exception", false);
        return false;
    }
    
    StdString url = BASE_URL_EXCEPTION_TEST + "/custom-exception";
    std_print("[DEBUG] URL: ");
    std_println(url.c_str());
    
    StdString responseJson = httpClient->Get(url);
    std_print("[DEBUG] Response received: ");
    std_println(responseJson.c_str());
    
    HttpResponse response = ParseHttpResponse(responseJson);
    std_print("[DEBUG] Status code: ");
    std_println(std::to_string(response.statusCode).c_str());
    std_print("[DEBUG] Body: ");
    std_println(response.body.c_str());
    
    // Should return 500 Internal Server Error
    ASSERT_EXCEPTION(response.statusCode == 500, 
                     "HTTP status should be 500 Internal Server Error");
    
    // Body should contain error information
    bool hasError = response.body.find("error") != StdString::npos || 
                    response.body.find("Internal Server Error") != StdString::npos;
    ASSERT_EXCEPTION(hasError, "Body should contain error information");
    
    // For catch-all exceptions, we get "Unknown exception occurred"
    bool hasUnknownMessage = response.body.find("Unknown exception") != StdString::npos ||
                             response.body.find("Unknown") != StdString::npos;
    ASSERT_EXCEPTION(hasUnknownMessage, "Body should contain 'Unknown exception occurred' for catch-all");
    
    PrintExceptionTestResult("Throw Custom Exception", true);
    return true;
}

// ========== RUN ALL TESTS ==========

/**
 * Run all ExceptionTestController tests
 * 
 * @param ip Server IP address (default: "localhost")
 * @param port Server port (default: "8080")
 * @return Number of failed tests
 */
int RunAllExceptionTestControllerTests(const std::string& ip, const std::string& port) {
    std_println("");
    std_println("----------------------------------------");
    std_println("  ExceptionTestControllerTests");
    std_println("----------------------------------------");
    
    // Reset counters
    testsPassed_exception = 0;
    testsFailed_exception = 0;
    
    // Set base URL
    BASE_URL_EXCEPTION_TEST = "http://" + StdString(ip.c_str()) + ":" + StdString(port.c_str()) + "/exception-test";
    std_print("Base URL: ");
    std_println(BASE_URL_EXCEPTION_TEST.c_str());
    std_println("");
    
    // Run all tests
    TestThrowRuntimeException();
    TestThrowLogicException();
    TestThrowStringException();
    TestThrowIntException();
    TestThrowCustomException();
    
    // Print summary
    std_println("");
    std_print("Tests passed: ");
    std_println(std::to_string(testsPassed_exception).c_str());
    std_print("Tests failed: ");
    std_println(std::to_string(testsFailed_exception).c_str());
    std_println("----------------------------------------");
    std_println("");
    
    return testsFailed_exception;
}

#endif // EXCEPTION_TEST_CONTROLLER_TESTS_H

