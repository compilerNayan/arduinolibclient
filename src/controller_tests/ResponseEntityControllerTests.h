#ifndef RESPONSE_ENTITY_CONTROLLER_TESTS_H
#define RESPONSE_ENTITY_CONTROLLER_TESTS_H

// Conditionally include headers based on platform
#ifdef ARDUINO
    #include <Arduino.h>
    #include <string>
    #include <vector>
    #include <cmath>
#else
    #include <iostream>
    #include <cassert>
    #include <string>
    #include <vector>
    #include <cmath>
#endif

#include <StandardDefines.h>
#include <SerializationUtility.h>
#include "http_client/ISpecialHttpClient.h"
#include "../repository_tests/Order.h"
#include "../tests/TestUtils.h"

using namespace nayan::serializer;

// Test assertion macros (using common ASSERT macro from TestUtils.h)
#define ASSERT_RESPONSE_ENTITY(condition, message) ASSERT(condition, message)

#define TEST_RESPONSE_ENTITY_START(test_name) TEST_START(test_name)

// Base URL for the REST API (will be set by RunAllResponseEntityControllerTests)
// Note: Using a unique name to avoid conflict with WifiCredentialsControllerTests
static StdString BASE_URL_RESPONSE_ENTITY;

// Global test counters
static int testsPassed_response_entity = 0;
static int testsFailed_response_entity = 0;

// Helper function to print test result and update counters
inline void PrintResponseEntityTestResult(const char* testName, bool passed) {
    ::PrintTestResult(testName, passed);
    if (passed) {
        testsPassed_response_entity++;
    } else {
        testsFailed_response_entity++;
    }
}

// Note: Using HttpResponse, ParseHttpResponse, and GetHttpClient from WifiCredentialsControllerTests.h
// They are shared helpers available when both test files are included

// ========== GET STRING RESPONSE TESTS ==========

// Test 1: Get String Response - Should return 201 CREATED
bool TestGetStringResponse() {
    TEST_RESPONSE_ENTITY_START("Test Get String Response - 201 CREATED");
    
    ISpecialHttpClientPtr httpClient = GetHttpClient();
    if (!httpClient) {
        std_println("FAILED - HTTP client is null!");
        PrintResponseEntityTestResult("Get String Response", false);
        return false;
    }
    
    StdString url = BASE_URL_RESPONSE_ENTITY + "/string";
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
    
    // Should return 201 CREATED
    ASSERT_RESPONSE_ENTITY(response.statusCode == 201, 
                          "HTTP status should be 201 CREATED");
    
    // Body should be "Created successfully" (may be JSON-encoded)
    // The body might be wrapped in quotes if it's a string
    bool bodyMatches = false;
    if (response.body == "\"Created successfully\"" || 
        response.body == "Created successfully" ||
        response.body.find("Created successfully") != StdString::npos) {
        bodyMatches = true;
    }
    ASSERT_RESPONSE_ENTITY(bodyMatches, "Body should contain 'Created successfully'");
    
    PrintResponseEntityTestResult("Get String Response", true);
    return true;
}

// ========== GET INT RESPONSE TESTS ==========

// Test 2: Get Int Response - Should return 202 ACCEPTED
bool TestGetIntResponse() {
    TEST_RESPONSE_ENTITY_START("Test Get Int Response - 202 ACCEPTED");
    
    ISpecialHttpClientPtr httpClient = GetHttpClient();
    if (!httpClient) {
        std_println("FAILED - HTTP client is null!");
        PrintResponseEntityTestResult("Get Int Response", false);
        return false;
    }
    
    StdString url = BASE_URL_RESPONSE_ENTITY + "/int";
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
    
    // Should return 202 ACCEPTED
    ASSERT_RESPONSE_ENTITY(response.statusCode == 202, 
                          "HTTP status should be 202 ACCEPTED");
    
    // Body should be 42 (may be JSON-encoded)
    bool bodyMatches = false;
    if (response.body == "42" || 
        response.body.find("42") != StdString::npos) {
        bodyMatches = true;
    }
    ASSERT_RESPONSE_ENTITY(bodyMatches, "Body should contain '42'");
    
    PrintResponseEntityTestResult("Get Int Response", true);
    return true;
}

// ========== GET ORDER RESPONSE TESTS ==========

// Test 3: Get Order Response - Should return 203 NON_AUTHORITATIVE_INFORMATION
bool TestGetOrderResponse() {
    TEST_RESPONSE_ENTITY_START("Test Get Order Response - 203 NON_AUTHORITATIVE_INFORMATION");
    
    ISpecialHttpClientPtr httpClient = GetHttpClient();
    if (!httpClient) {
        std_println("FAILED - HTTP client is null!");
        PrintResponseEntityTestResult("Get Order Response", false);
        return false;
    }
    
    StdString url = BASE_URL_RESPONSE_ENTITY + "/order";
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
    
    // Should return 203 NON_AUTHORITATIVE_INFORMATION
    ASSERT_RESPONSE_ENTITY(response.statusCode == 203, 
                          "HTTP status should be 203 NON_AUTHORITATIVE_INFORMATION");
    
    // Deserialize the Order from the body
    Order order = SerializationUtility::Deserialize<Order>(response.body);
    
    // Verify Order fields
    ASSERT_RESPONSE_ENTITY(order.id.has_value(), "Order id should be present");
    ASSERT_RESPONSE_ENTITY(order.id.value() == 1, "Order id should be 1");
    ASSERT_RESPONSE_ENTITY(order.orderNumber.has_value(), "Order orderNumber should be present");
    ASSERT_RESPONSE_ENTITY(order.orderNumber.value() == "ORD-12345", "Order orderNumber should be 'ORD-12345'");
    ASSERT_RESPONSE_ENTITY(order.customerId.has_value(), "Order customerId should be present");
    ASSERT_RESPONSE_ENTITY(order.customerId.value() == 100, "Order customerId should be 100");
    ASSERT_RESPONSE_ENTITY(order.totalAmount.has_value(), "Order totalAmount should be present");
    // Allow for floating point comparison tolerance
    double expectedAmount = 99.99;
    double actualAmount = order.totalAmount.value();
    double tolerance = 0.01;
    ASSERT_RESPONSE_ENTITY(std::abs(actualAmount - expectedAmount) < tolerance, 
                          "Order totalAmount should be approximately 99.99");
    
    PrintResponseEntityTestResult("Get Order Response", true);
    return true;
}

// ========== GET VOID RESPONSE TESTS ==========

// Test 4: Get Void Response - Should return 404 NOT_FOUND
bool TestGetVoidResponse() {
    TEST_RESPONSE_ENTITY_START("Test Get Void Response - 404 NOT_FOUND");
    
    ISpecialHttpClientPtr httpClient = GetHttpClient();
    if (!httpClient) {
        std_println("FAILED - HTTP client is null!");
        PrintResponseEntityTestResult("Get Void Response", false);
        return false;
    }
    
    StdString url = BASE_URL_RESPONSE_ENTITY + "/void";
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
    
    // Should return 404 NOT_FOUND
    ASSERT_RESPONSE_ENTITY(response.statusCode == 404, 
                          "HTTP status should be 404 NOT_FOUND");
    
    // Body should be empty or empty JSON object for Void
    bool bodyIsEmpty = response.body.empty() || 
                       response.body == "{}" || 
                       response.body == "null" ||
                       response.body == "\"\"" ||
                       response.body.find("{}") != StdString::npos;
    ASSERT_RESPONSE_ENTITY(bodyIsEmpty, "Body should be empty for Void response");
    
    PrintResponseEntityTestResult("Get Void Response", true);
    return true;
}

// ========== RUN ALL TESTS ==========

/**
 * Run all ResponseEntityController tests
 * 
 * @param ip Server IP address (default: "localhost")
 * @param port Server port (default: "8080")
 * @return Number of failed tests
 */
int RunAllResponseEntityControllerTests(const std::string& ip, const std::string& port) {
    std_println("");
    std_println("----------------------------------------");
    std_println("  ResponseEntityControllerTests");
    std_println("----------------------------------------");
    
    // Reset counters
    testsPassed_response_entity = 0;
    testsFailed_response_entity = 0;
    
    // Set base URL
    BASE_URL_RESPONSE_ENTITY = "http://" + StdString(ip.c_str()) + ":" + StdString(port.c_str()) + "/response-entity";
    std_print("Base URL: ");
    std_println(BASE_URL_RESPONSE_ENTITY.c_str());
    std_println("");
    
    // Run all tests
    TestGetStringResponse();
    TestGetIntResponse();
    TestGetOrderResponse();
    TestGetVoidResponse();
    
    // Print summary
    std_println("");
    std_print("Tests passed: ");
    std_println(std::to_string(testsPassed_response_entity).c_str());
    std_print("Tests failed: ");
    std_println(std::to_string(testsFailed_response_entity).c_str());
    std_println("----------------------------------------");
    std_println("");
    
    return testsFailed_response_entity;
}

#endif // RESPONSE_ENTITY_CONTROLLER_TESTS_H

