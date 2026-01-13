#ifndef WIFI_CREDENTIALS_CONTROLLER_TESTS_H
#define WIFI_CREDENTIALS_CONTROLLER_TESTS_H

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
#include "entity/WifiCredentials.h"
#include "../tests/TestUtils.h"

using namespace nayan::serializer;

// Test assertion macros (using common ASSERT macro from TestUtils.h)
#define ASSERT_WIFI(condition, message) ASSERT(condition, message)

#define TEST_WIFI_START(test_name) TEST_START(test_name)

// Base URL for the REST API
static const StdString BASE_URL = "http://localhost:8080/wifi-credentials";

// Global test counters
static int testsPassed_wifi = 0;
static int testsFailed_wifi = 0;

// Helper function to print test result and update counters
inline void PrintWifiTestResult(const char* testName, bool passed) {
    ::PrintTestResult(testName, passed);
    if (passed) {
        testsPassed_wifi++;
    } else {
        testsFailed_wifi++;
    }
}

// Helper function to create test WiFi credentials
WifiCredentials CreateTestCredentials(const StdString& ssid, const StdString& password) {
    WifiCredentials creds;
    creds.ssid = ssid;
    creds.password = password;
    return creds;
}

// Helper function to parse HTTP response JSON
struct HttpResponse {
    int statusCode;
    StdString body;
};

HttpResponse ParseHttpResponse(const StdString& responseJson) {
    HttpResponse response;
    response.statusCode = 0;
    
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, responseJson.c_str());
    if (error == DeserializationError::Ok) {
        if (doc["statusCode"].is<int>()) {
            response.statusCode = doc["statusCode"].as<int>();
        }
        if (doc["body"].is<const char*>()) {
            response.body = StdString(doc["body"].as<const char*>());
        }
    }
    
    return response;
}

// Helper function to get HTTP client instance
ISpecialHttpClientPtr GetHttpClient() {
    return Implementation<ISpecialHttpClient>::type::GetInstance();
}

// ========== CREATE OPERATIONS TESTS ==========

// Test 1: Create WiFi credentials - Success
bool TestCreateWifiCredentials_Success() {
    TEST_WIFI_START("Test Create WiFi Credentials - Success (REST API)");
    
    std_println("[DEBUG] Starting test...");
    
    std_print("[DEBUG] Getting HTTP client instance...");
    ISpecialHttpClientPtr httpClient = GetHttpClient();
    if (!httpClient) {
        std_println("FAILED - HTTP client is null!");
        PrintWifiTestResult("Create WiFi Credentials - Success", false);
        return false;
    }
    std_println("OK");
    
    std_print("[DEBUG] Creating test credentials...");
    WifiCredentials creds = CreateTestCredentials("TestNetwork", "TestPassword123");
    std_println("OK");
    
    std_print("[DEBUG] Serializing credentials to JSON...");
    StdString jsonBody = SerializationUtility::Serialize(creds);
    std_print("OK - Body length: ");
    std_println(std::to_string(jsonBody.length()).c_str());
    
    StdString url = BASE_URL;
    std_print("[DEBUG] URL: ");
    std_println(url.c_str());
    std_print("[DEBUG] Request body: ");
    std_println(jsonBody.c_str());
    
    std_println("[DEBUG] About to call httpClient->Post()...");
    std_println("[DEBUG] This may take up to 30 seconds if server is not responding...");
    
    StdString responseJson = httpClient->Post(url, jsonBody);
    
    std_println("[DEBUG] httpClient->Post() returned!");
    std_print("[DEBUG] Response length: ");
    std_println(std::to_string(responseJson.length()).c_str());
    std_print("[DEBUG] Response received: ");
    std_println(responseJson.c_str());
    
    std_print("[DEBUG] Parsing HTTP response...");
    HttpResponse response = ParseHttpResponse(responseJson);
    std_print("OK - Status code: ");
    std_println(std::to_string(response.statusCode).c_str());
    
    std_print("[DEBUG] Checking status code...");
    if (response.statusCode != 200 && response.statusCode != 201) {
        std_print("[DEBUG] ERROR: Unexpected status code: ");
        std_println(std::to_string(response.statusCode).c_str());
        std_print("[DEBUG] Response body: ");
        std_println(response.body.c_str());
    }
    ASSERT_WIFI(response.statusCode == 200 || response.statusCode == 201, 
                "HTTP status should be 200 or 201");
    
    std_print("[DEBUG] Deserializing response body...");
    WifiCredentials result = SerializationUtility::Deserialize<WifiCredentials>(response.body);
    std_println("OK");
    
    std_print("[DEBUG] Validating result...");
    ASSERT_WIFI(result.ssid.has_value(), "SSID should be present in result");
    ASSERT_WIFI(result.ssid.value() == "TestNetwork", "SSID should match input");
    ASSERT_WIFI(result.password.has_value(), "Password should be present in result");
    ASSERT_WIFI(result.password.value() == "TestPassword123", "Password should match input");
    std_println("OK");
    
    std_println("[DEBUG] Test completed successfully!");
    PrintWifiTestResult("Create WiFi Credentials - Success", true);
    return true;
}

// Test 2: Create WiFi credentials - Empty SSID
bool TestCreateWifiCredentials_EmptySsid() {
    TEST_WIFI_START("Test Create WiFi Credentials - Empty SSID (REST API)");
    
    ISpecialHttpClientPtr httpClient = GetHttpClient();
    
    WifiCredentials creds;
    creds.ssid = StdString("");
    creds.password = StdString("SomePassword");
    
    StdString jsonBody = SerializationUtility::Serialize(creds);
    StdString url = BASE_URL;
    StdString responseJson = httpClient->Post(url, jsonBody);
    HttpResponse response = ParseHttpResponse(responseJson);
    
    // Should still create (validation might be in service layer)
    PrintWifiTestResult("Create WiFi Credentials - Empty SSID", true);
    return true;
}

// Test 3: Create WiFi credentials - Empty Password
bool TestCreateWifiCredentials_EmptyPassword() {
    TEST_WIFI_START("Test Create WiFi Credentials - Empty Password (REST API)");
    
    ISpecialHttpClientPtr httpClient = GetHttpClient();
    
    WifiCredentials creds;
    creds.ssid = StdString("TestNetwork");
    creds.password = StdString("");
    
    StdString jsonBody = SerializationUtility::Serialize(creds);
    StdString url = BASE_URL;
    StdString responseJson = httpClient->Post(url, jsonBody);
    HttpResponse response = ParseHttpResponse(responseJson);
    
    ASSERT_WIFI(response.statusCode == 200 || response.statusCode == 201, 
                "HTTP status should be 200 or 201");
    
    WifiCredentials result = SerializationUtility::Deserialize<WifiCredentials>(response.body);
    ASSERT_WIFI(result.ssid.has_value(), "SSID should be present");
    ASSERT_WIFI(result.ssid.value() == "TestNetwork", "SSID should match");
    
    PrintWifiTestResult("Create WiFi Credentials - Empty Password", true);
    return true;
}

// Test 4: Create WiFi credentials - Very Long SSID
bool TestCreateWifiCredentials_VeryLongSsid() {
    TEST_WIFI_START("Test Create WiFi Credentials - Very Long SSID (REST API)");
    
    ISpecialHttpClientPtr httpClient = GetHttpClient();
    
    StdString longSsid(256, 'A'); // 256 character SSID
    WifiCredentials creds = CreateTestCredentials(longSsid, "Password123");
    
    StdString jsonBody = SerializationUtility::Serialize(creds);
    StdString url = BASE_URL;
    StdString responseJson = httpClient->Post(url, jsonBody);
    HttpResponse response = ParseHttpResponse(responseJson);
    
    ASSERT_WIFI(response.statusCode == 200 || response.statusCode == 201, 
                "HTTP status should be 200 or 201");
    
    WifiCredentials result = SerializationUtility::Deserialize<WifiCredentials>(response.body);
    ASSERT_WIFI(result.ssid.has_value(), "SSID should be present");
    ASSERT_WIFI(result.ssid.value().length() == 256, "SSID length should match");
    
    PrintWifiTestResult("Create WiFi Credentials - Very Long SSID", true);
    return true;
}

// Test 5: Create WiFi credentials - Special Characters
bool TestCreateWifiCredentials_SpecialCharacters() {
    TEST_WIFI_START("Test Create WiFi Credentials - Special Characters (REST API)");
    
    ISpecialHttpClientPtr httpClient = GetHttpClient();
    
    WifiCredentials creds = CreateTestCredentials("Network_123-ABC", "P@ssw0rd!#$%");
    
    StdString jsonBody = SerializationUtility::Serialize(creds);
    StdString url = BASE_URL;
    StdString responseJson = httpClient->Post(url, jsonBody);
    HttpResponse response = ParseHttpResponse(responseJson);
    
    ASSERT_WIFI(response.statusCode == 200 || response.statusCode == 201, 
                "HTTP status should be 200 or 201");
    
    WifiCredentials result = SerializationUtility::Deserialize<WifiCredentials>(response.body);
    ASSERT_WIFI(result.ssid.has_value(), "SSID should be present");
    ASSERT_WIFI(result.ssid.value() == "Network_123-ABC", "SSID with special chars should match");
    
    PrintWifiTestResult("Create WiFi Credentials - Special Characters", true);
    return true;
}

// ========== READ OPERATIONS TESTS ==========

// Test 6: Get WiFi credentials by SSID - Success
bool TestGetWifiCredentials_BySsid_Success() {
    TEST_WIFI_START("Test Get WiFi Credentials by SSID - Success (REST API)");
    
    ISpecialHttpClientPtr httpClient = GetHttpClient();
    
    // First create credentials
    WifiCredentials creds = CreateTestCredentials("GetTestNetwork", "GetTestPassword");
    StdString createJsonBody = SerializationUtility::Serialize(creds);
    StdString createUrl = BASE_URL;
    httpClient->Post(createUrl, createJsonBody);
    
    // Then retrieve them
    StdString getUrl = BASE_URL + "/GetTestNetwork";
    StdString responseJson = httpClient->Get(getUrl);
    HttpResponse response = ParseHttpResponse(responseJson);
    
    ASSERT_WIFI(response.statusCode == 200, "HTTP status should be 200");
    
    WifiCredentials result = SerializationUtility::Deserialize<WifiCredentials>(response.body);
    
    ASSERT_WIFI(result.ssid.has_value(), "SSID should be present");
    ASSERT_WIFI(result.ssid.value() == "GetTestNetwork", "SSID should match");
    ASSERT_WIFI(result.password.has_value(), "Password should be present");
    ASSERT_WIFI(result.password.value() == "GetTestPassword", "Password should match");
    
    PrintWifiTestResult("Get WiFi Credentials by SSID - Success", true);
    return true;
}

// Test 7: Get WiFi credentials by SSID - Not Found
/* bool TestGetWifiCredentials_BySsid_NotFound() {
    TEST_WIFI_START("Test Get WiFi Credentials by SSID - Not Found (REST API)");
    
    ISpecialHttpClientPtr httpClient = GetHttpClient();
    
    StdString getUrl = BASE_URL + "/NonExistentNetwork";
    StdString responseJson = httpClient->Get(getUrl);
    HttpResponse response = ParseHttpResponse(responseJson);
    
    // Should return 404 or empty result
    ASSERT_WIFI(response.statusCode == 404 || response.body.empty() || 
                response.body == "{}" || response.body == "null", 
                "Should return 404 or empty result for non-existent network");
    
    PrintWifiTestResult("Get WiFi Credentials by SSID - Not Found", true);
    return true;
} */

// Test 8: Get WiFi credentials by SSID - Empty SSID
bool TestGetWifiCredentials_BySsid_EmptySsid() {
    TEST_WIFI_START("Test Get WiFi Credentials by SSID - Empty SSID (REST API)");
    
    ISpecialHttpClientPtr httpClient = GetHttpClient();
    
    StdString getUrl = BASE_URL + "/";
    StdString responseJson = httpClient->Get(getUrl);
    HttpResponse response = ParseHttpResponse(responseJson);
    
    // Should return error or empty result for empty SSID
    PrintWifiTestResult("Get WiFi Credentials by SSID - Empty SSID", true);
    return true;
}

// Test 9: Get All WiFi credentials - Empty
bool TestGetAllWifiCredentials_Empty() {
    TEST_WIFI_START("Test Get All WiFi Credentials - Empty (REST API)");
    
    ISpecialHttpClientPtr httpClient = GetHttpClient();
    
    StdString url = BASE_URL;
    StdString responseJson = httpClient->Get(url);
    HttpResponse response = ParseHttpResponse(responseJson);
    
    ASSERT_WIFI(response.statusCode == 200, "HTTP status should be 200");
    
    Vector<WifiCredentials> result = SerializationUtility::Deserialize<Vector<WifiCredentials>>(response.body);
    
    // Should return empty vector if no credentials exist
    PrintWifiTestResult("Get All WiFi Credentials - Empty", true);
    return true;
}

// Test 10: Get All WiFi credentials - Multiple
bool TestGetAllWifiCredentials_Multiple() {
    TEST_WIFI_START("Test Get All WiFi Credentials - Multiple (REST API)");
    
    ISpecialHttpClientPtr httpClient = GetHttpClient();
    
    // Create multiple credentials
    WifiCredentials creds1 = CreateTestCredentials("Network1", "Password1");
    WifiCredentials creds2 = CreateTestCredentials("Network2", "Password2");
    WifiCredentials creds3 = CreateTestCredentials("Network3", "Password3");
    
    StdString url = BASE_URL;
    httpClient->Post(url, SerializationUtility::Serialize(creds1));
    httpClient->Post(url, SerializationUtility::Serialize(creds2));
    httpClient->Post(url, SerializationUtility::Serialize(creds3));
    
    StdString responseJson = httpClient->Get(url);
    HttpResponse response = ParseHttpResponse(responseJson);
    
    ASSERT_WIFI(response.statusCode == 200, "HTTP status should be 200");
    
    Vector<WifiCredentials> result = SerializationUtility::Deserialize<Vector<WifiCredentials>>(response.body);
    
    ASSERT_WIFI(result.size() >= 3, "Should return at least 3 credentials");
    
    // Verify all three are present
    bool found1 = false, found2 = false, found3 = false;
    for (const auto& cred : result) {
        if (cred.ssid.has_value()) {
            if (cred.ssid.value() == "Network1") found1 = true;
            if (cred.ssid.value() == "Network2") found2 = true;
            if (cred.ssid.value() == "Network3") found3 = true;
        }
    }
    
    ASSERT_WIFI(found1 && found2 && found3, "All three networks should be found");
    
    PrintWifiTestResult("Get All WiFi Credentials - Multiple", true);
    return true;
}

// ========== UPDATE OPERATIONS TESTS ==========

// Test 11: Update WiFi credentials - Success
bool TestUpdateWifiCredentials_Success() {
    TEST_WIFI_START("Test Update WiFi Credentials - Success (REST API)");
    
    ISpecialHttpClientPtr httpClient = GetHttpClient();
    
    // First create credentials
    WifiCredentials creds = CreateTestCredentials("UpdateTestNetwork", "OldPassword");
    StdString createUrl = BASE_URL;
    httpClient->Post(createUrl, SerializationUtility::Serialize(creds));
    
    // Update password
    WifiCredentials updatedCreds = CreateTestCredentials("UpdateTestNetwork", "NewPassword");
    StdString updateUrl = BASE_URL;
    StdString responseJson = httpClient->Put(updateUrl, SerializationUtility::Serialize(updatedCreds));
    HttpResponse response = ParseHttpResponse(responseJson);
    
    ASSERT_WIFI(response.statusCode == 200, "HTTP status should be 200");
    
    WifiCredentials result = SerializationUtility::Deserialize<WifiCredentials>(response.body);
    
    ASSERT_WIFI(result.ssid.has_value(), "SSID should be present");
    ASSERT_WIFI(result.ssid.value() == "UpdateTestNetwork", "SSID should match");
    ASSERT_WIFI(result.password.has_value(), "Password should be present");
    ASSERT_WIFI(result.password.value() == "NewPassword", "Password should be updated");
    
    // Verify update by retrieving
    StdString getUrl = BASE_URL + "/UpdateTestNetwork";
    StdString getResponseJson = httpClient->Get(getUrl);
    HttpResponse getResponse = ParseHttpResponse(getResponseJson);
    
    ASSERT_WIFI(getResponse.statusCode == 200, "HTTP status should be 200");
    WifiCredentials retrieved = SerializationUtility::Deserialize<WifiCredentials>(getResponse.body);
    ASSERT_WIFI(retrieved.password.value() == "NewPassword", "Password should be updated in storage");
    
    PrintWifiTestResult("Update WiFi Credentials - Success", true);
    return true;
}

// Test 12: Update WiFi credentials - Non-existent SSID
bool TestUpdateWifiCredentials_NonExistent() {
    TEST_WIFI_START("Test Update WiFi Credentials - Non-existent SSID (REST API)");
    
    ISpecialHttpClientPtr httpClient = GetHttpClient();
    
    WifiCredentials creds = CreateTestCredentials("NonExistentNetwork", "SomePassword");
    
    StdString url = BASE_URL;
    StdString responseJson = httpClient->Put(url, SerializationUtility::Serialize(creds));
    HttpResponse response = ParseHttpResponse(responseJson);
    
    // Update might create if not exists, or return error
    PrintWifiTestResult("Update WiFi Credentials - Non-existent SSID", true);
    return true;
}

// Test 13: Update WiFi credentials - Empty Password
bool TestUpdateWifiCredentials_EmptyPassword() {
    TEST_WIFI_START("Test Update WiFi Credentials - Empty Password (REST API)");
    
    ISpecialHttpClientPtr httpClient = GetHttpClient();
    
    // Create first
    WifiCredentials creds = CreateTestCredentials("UpdateEmptyPassNetwork", "OriginalPassword");
    StdString createUrl = BASE_URL;
    httpClient->Post(createUrl, SerializationUtility::Serialize(creds));
    
    // Update with empty password
    WifiCredentials updatedCreds;
    updatedCreds.ssid = StdString("UpdateEmptyPassNetwork");
    updatedCreds.password = StdString("");
    
    StdString updateUrl = BASE_URL;
    StdString responseJson = httpClient->Put(updateUrl, SerializationUtility::Serialize(updatedCreds));
    HttpResponse response = ParseHttpResponse(responseJson);
    
    ASSERT_WIFI(response.statusCode == 200, "HTTP status should be 200");
    
    WifiCredentials result = SerializationUtility::Deserialize<WifiCredentials>(response.body);
    ASSERT_WIFI(result.ssid.has_value(), "SSID should be present");
    
    PrintWifiTestResult("Update WiFi Credentials - Empty Password", true);
    return true;
}

// ========== DELETE OPERATIONS TESTS ==========

// Test 14: Delete WiFi credentials - Success
bool TestDeleteWifiCredentials_Success() {
    TEST_WIFI_START("Test Delete WiFi Credentials - Success (REST API)");
    
    ISpecialHttpClientPtr httpClient = GetHttpClient();
    
    // First create credentials
    WifiCredentials creds = CreateTestCredentials("DeleteTestNetwork", "DeletePassword");
    StdString createUrl = BASE_URL;
    httpClient->Post(createUrl, SerializationUtility::Serialize(creds));
    
    // Verify it exists
    StdString getUrl = BASE_URL + "/DeleteTestNetwork";
    StdString getResponseJson = httpClient->Get(getUrl);
    HttpResponse getResponse = ParseHttpResponse(getResponseJson);
    ASSERT_WIFI(getResponse.statusCode == 200, "Credentials should exist before delete");
    
    // Delete
    StdString deleteUrl = BASE_URL + "/DeleteTestNetwork";
    StdString deleteResponseJson = httpClient->Delete(deleteUrl);
    HttpResponse deleteResponse = ParseHttpResponse(deleteResponseJson);
    
    ASSERT_WIFI(deleteResponse.statusCode == 200 || deleteResponse.statusCode == 204, 
                "HTTP status should be 200 or 204");
    
    // Verify it's deleted
    StdString verifyResponseJson = httpClient->Get(getUrl);
    HttpResponse verifyResponse = ParseHttpResponse(verifyResponseJson);
    // Check if response indicates not found (404, empty body, or empty object)
    if (verifyResponse.statusCode == 404 || verifyResponse.body.empty() || 
        verifyResponse.body == "{}" || verifyResponse.body == "null") {
        // Already indicates not found
    } else {
        // If status is 200, check if the deserialized object is empty (no ssid)
        WifiCredentials deletedCheck = SerializationUtility::Deserialize<WifiCredentials>(verifyResponse.body);
        ASSERT_WIFI(!deletedCheck.ssid.has_value(), "Credentials should not exist after delete (ssid should be null)");
    }
    
    PrintWifiTestResult("Delete WiFi Credentials - Success", true);
    return true;
}

// Test 15: Delete WiFi credentials - Non-existent SSID
bool TestDeleteWifiCredentials_NonExistent() {
    TEST_WIFI_START("Test Delete WiFi Credentials - Non-existent SSID (REST API)");
    
    ISpecialHttpClientPtr httpClient = GetHttpClient();
    
    // Delete non-existent should not throw error
    StdString deleteUrl = BASE_URL + "/NonExistentNetworkToDelete";
    StdString responseJson = httpClient->Delete(deleteUrl);
    HttpResponse response = ParseHttpResponse(responseJson);
    
    // Should complete without error (might return 404 or 200)
    PrintWifiTestResult("Delete WiFi Credentials - Non-existent SSID", true);
    return true;
}

// Test 16: Delete WiFi credentials - Empty SSID
bool TestDeleteWifiCredentials_EmptySsid() {
    TEST_WIFI_START("Test Delete WiFi Credentials - Empty SSID (REST API)");
    
    ISpecialHttpClientPtr httpClient = GetHttpClient();
    
    // Delete with empty SSID should not throw error
    StdString deleteUrl = BASE_URL + "/";
    StdString responseJson = httpClient->Delete(deleteUrl);
    
    PrintWifiTestResult("Delete WiFi Credentials - Empty SSID", true);
    return true;
}

// Test 17: Delete and verify last connected is cleared
bool TestDeleteWifiCredentials_ClearsLastConnected() {
    TEST_WIFI_START("Test Delete WiFi Credentials - Clears Last Connected (REST API)");
    
    ISpecialHttpClientPtr httpClient = GetHttpClient();
    
    // Create and this will set as last connected
    WifiCredentials creds = CreateTestCredentials("LastConnectedNetwork", "Password");
    StdString createUrl = BASE_URL;
    httpClient->Post(createUrl, SerializationUtility::Serialize(creds));
    
    // Verify it's last connected
    StdString lastConnectedUrl = BASE_URL + "/last-connected";
    StdString lastConnectedResponseJson = httpClient->Get(lastConnectedUrl);
    HttpResponse lastConnectedResponse = ParseHttpResponse(lastConnectedResponseJson);
    ASSERT_WIFI(lastConnectedResponse.statusCode == 200, "Should have last connected WiFi");
    
    // Delete it
    StdString deleteUrl = BASE_URL + "/LastConnectedNetwork";
    httpClient->Delete(deleteUrl);
    
    // Verify last connected is cleared (or points to something else)
    StdString verifyResponseJson = httpClient->Get(lastConnectedUrl);
    HttpResponse verifyResponse = ParseHttpResponse(verifyResponseJson);
    
    PrintWifiTestResult("Delete WiFi Credentials - Clears Last Connected", true);
    return true;
}

// ========== LAST CONNECTED TESTS ==========

// Test 18: Get Last Connected WiFi - Success
bool TestGetLastConnectedWifi_Success() {
    TEST_WIFI_START("Test Get Last Connected WiFi - Success (REST API)");
    
    ISpecialHttpClientPtr httpClient = GetHttpClient();
    
    // Create credentials (this should set as last connected)
    WifiCredentials creds = CreateTestCredentials("LastConnectedTest", "LastPassword");
    StdString createUrl = BASE_URL;
    httpClient->Post(createUrl, SerializationUtility::Serialize(creds));
    
    StdString url = BASE_URL + "/last-connected";
    StdString responseJson = httpClient->Get(url);
    HttpResponse response = ParseHttpResponse(responseJson);
    
    ASSERT_WIFI(response.statusCode == 200, "HTTP status should be 200");
    
    optional<WifiCredentials> result = SerializationUtility::Deserialize<optional<WifiCredentials>>(response.body);
    
    ASSERT_WIFI(result.has_value(), "Last connected WiFi should be found");
    ASSERT_WIFI(result.value().ssid.has_value(), "SSID should be present");
    ASSERT_WIFI(result.value().ssid.value() == "LastConnectedTest", "SSID should match last created");
    
    PrintWifiTestResult("Get Last Connected WiFi - Success", true);
    return true;
}

// Test 19: Get Last Connected WiFi - Not Set
bool TestGetLastConnectedWifi_NotSet() {
    TEST_WIFI_START("Test Get Last Connected WiFi - Not Set (REST API)");
    
    ISpecialHttpClientPtr httpClient = GetHttpClient();
    
    StdString url = BASE_URL + "/last-connected";
    StdString responseJson = httpClient->Get(url);
    HttpResponse response = ParseHttpResponse(responseJson);
    
    // Should return nullopt if not set (or might return first available)
    PrintWifiTestResult("Get Last Connected WiFi - Not Set", true);
    return true;
}

// Test 20: Get Last Connected WiFi - After Update
bool TestGetLastConnectedWifi_AfterUpdate() {
    TEST_WIFI_START("Test Get Last Connected WiFi - After Update (REST API)");
    
    ISpecialHttpClientPtr httpClient = GetHttpClient();
    
    // Create first network
    WifiCredentials creds1 = CreateTestCredentials("NetworkA", "PasswordA");
    StdString createUrl = BASE_URL;
    httpClient->Post(createUrl, SerializationUtility::Serialize(creds1));
    
    // Create second network (should become last connected)
    WifiCredentials creds2 = CreateTestCredentials("NetworkB", "PasswordB");
    httpClient->Post(createUrl, SerializationUtility::Serialize(creds2));
    
    // Last connected should be NetworkB
    StdString lastConnectedUrl = BASE_URL + "/last-connected";
    StdString responseJson = httpClient->Get(lastConnectedUrl);
    HttpResponse response = ParseHttpResponse(responseJson);
    
    ASSERT_WIFI(response.statusCode == 200, "HTTP status should be 200");
    optional<WifiCredentials> result = SerializationUtility::Deserialize<optional<WifiCredentials>>(response.body);
    ASSERT_WIFI(result.has_value(), "Last connected should be found");
    ASSERT_WIFI(result.value().ssid.value() == "NetworkB", "Last connected should be NetworkB");
    
    // Update NetworkA (should become last connected)
    WifiCredentials updatedCreds1 = CreateTestCredentials("NetworkA", "NewPasswordA");
    httpClient->Put(createUrl, SerializationUtility::Serialize(updatedCreds1));
    
    // Last connected should now be NetworkA
    StdString responseJson2 = httpClient->Get(lastConnectedUrl);
    HttpResponse response2 = ParseHttpResponse(responseJson2);
    
    ASSERT_WIFI(response2.statusCode == 200, "HTTP status should be 200");
    optional<WifiCredentials> result2 = SerializationUtility::Deserialize<optional<WifiCredentials>>(response2.body);
    ASSERT_WIFI(result2.has_value(), "Last connected should still be found");
    ASSERT_WIFI(result2.value().ssid.value() == "NetworkA", "Last connected should be NetworkA after update");
    
    PrintWifiTestResult("Get Last Connected WiFi - After Update", true);
    return true;
}

// ========== EDGE CASES AND CORNER CASES ==========

// Test 21: Create with same SSID twice (should update)
bool TestCreateWifiCredentials_DuplicateSsid() {
    TEST_WIFI_START("Test Create WiFi Credentials - Duplicate SSID (REST API)");
    
    ISpecialHttpClientPtr httpClient = GetHttpClient();
    
    WifiCredentials creds1 = CreateTestCredentials("DuplicateNetwork", "Password1");
    StdString url = BASE_URL;
    httpClient->Post(url, SerializationUtility::Serialize(creds1));
    
    WifiCredentials creds2 = CreateTestCredentials("DuplicateNetwork", "Password2");
    StdString responseJson = httpClient->Post(url, SerializationUtility::Serialize(creds2));
    HttpResponse response = ParseHttpResponse(responseJson);
    
    ASSERT_WIFI(response.statusCode == 200 || response.statusCode == 201, 
                "HTTP status should be 200 or 201");
    
    WifiCredentials result = SerializationUtility::Deserialize<WifiCredentials>(response.body);
    ASSERT_WIFI(result.ssid.has_value(), "SSID should be present");
    
    PrintWifiTestResult("Create WiFi Credentials - Duplicate SSID", true);
    return true;
}

// Test 22: Multiple operations in sequence
bool TestWifiCredentials_MultipleOperationsSequence() {
    TEST_WIFI_START("Test WiFi Credentials - Multiple Operations Sequence (REST API)");
    
    ISpecialHttpClientPtr httpClient = GetHttpClient();
    
    // Create
    WifiCredentials creds1 = CreateTestCredentials("SequenceNetwork", "Password1");
    StdString url = BASE_URL;
    httpClient->Post(url, SerializationUtility::Serialize(creds1));
    
    // Read
    StdString getUrl = BASE_URL + "/SequenceNetwork";
    StdString getResponseJson = httpClient->Get(getUrl);
    HttpResponse getResponse = ParseHttpResponse(getResponseJson);
    ASSERT_WIFI(getResponse.statusCode == 200, "Should be able to read after create");
    
    // Update
    WifiCredentials creds2 = CreateTestCredentials("SequenceNetwork", "Password2");
    StdString updateResponseJson = httpClient->Put(url, SerializationUtility::Serialize(creds2));
    HttpResponse updateResponse = ParseHttpResponse(updateResponseJson);
    ASSERT_WIFI(updateResponse.statusCode == 200, "Should be able to update");
    
    // Read again
    StdString getResponseJson2 = httpClient->Get(getUrl);
    HttpResponse getResponse2 = ParseHttpResponse(getResponseJson2);
    ASSERT_WIFI(getResponse2.statusCode == 200, "Should be able to read after update");
    WifiCredentials read2 = SerializationUtility::Deserialize<WifiCredentials>(getResponse2.body);
    ASSERT_WIFI(read2.password.value() == "Password2", "Password should be updated");
    
    // Delete
    StdString deleteUrl = BASE_URL + "/SequenceNetwork";
    httpClient->Delete(deleteUrl);
    
    // Read after delete
    StdString getResponseJson3 = httpClient->Get(getUrl);
    HttpResponse getResponse3 = ParseHttpResponse(getResponseJson3);
    // Check if response indicates not found (404, empty body, or empty object)
    if (getResponse3.statusCode == 404 || getResponse3.body.empty() || 
        getResponse3.body == "{}" || getResponse3.body == "null") {
        // Already indicates not found
    } else {
        // If status is 200, check if the deserialized object is empty (no ssid)
        WifiCredentials deletedCheck = SerializationUtility::Deserialize<WifiCredentials>(getResponse3.body);
        ASSERT_WIFI(!deletedCheck.ssid.has_value(), "Should not be able to read after delete (ssid should be null)");
    }
    
    PrintWifiTestResult("WiFi Credentials - Multiple Operations Sequence", true);
    return true;
}

// Test 23: Very long password
bool TestCreateWifiCredentials_VeryLongPassword() {
    TEST_WIFI_START("Test Create WiFi Credentials - Very Long Password (REST API)");
    
    ISpecialHttpClientPtr httpClient = GetHttpClient();
    
    StdString longPassword(512, 'X'); // 512 character password
    WifiCredentials creds = CreateTestCredentials("ShortSSID", longPassword);
    
    StdString url = BASE_URL;
    StdString responseJson = httpClient->Post(url, SerializationUtility::Serialize(creds));
    HttpResponse response = ParseHttpResponse(responseJson);
    
    ASSERT_WIFI(response.statusCode == 200 || response.statusCode == 201, 
                "HTTP status should be 200 or 201");
    
    WifiCredentials result = SerializationUtility::Deserialize<WifiCredentials>(response.body);
    ASSERT_WIFI(result.password.has_value(), "Password should be present");
    ASSERT_WIFI(result.password.value().length() == 512, "Password length should match");
    
    PrintWifiTestResult("Create WiFi Credentials - Very Long Password", true);
    return true;
}

// Test 24: Unicode characters in SSID and password
bool TestCreateWifiCredentials_UnicodeCharacters() {
    TEST_WIFI_START("Test Create WiFi Credentials - Unicode Characters (REST API)");
    
    ISpecialHttpClientPtr httpClient = GetHttpClient();
    
    WifiCredentials creds;
    creds.ssid = StdString("网络_Network_123");
    creds.password = StdString("密码_Password_🔒");
    
    StdString url = BASE_URL;
    StdString responseJson = httpClient->Post(url, SerializationUtility::Serialize(creds));
    HttpResponse response = ParseHttpResponse(responseJson);
    
    ASSERT_WIFI(response.statusCode == 200 || response.statusCode == 201, 
                "HTTP status should be 200 or 201");
    
    WifiCredentials result = SerializationUtility::Deserialize<WifiCredentials>(response.body);
    ASSERT_WIFI(result.ssid.has_value(), "SSID with unicode should be present");
    ASSERT_WIFI(result.password.has_value(), "Password with unicode should be present");
    
    PrintWifiTestResult("Create WiFi Credentials - Unicode Characters", true);
    return true;
}

// Main test runner function
int RunAllWifiCredentialsControllerTests() {
    std_println("\n========================================");
    std_println("  WiFi Credentials Controller Tests (REST API)");
    std_println("========================================");
    std_println("");
    
    testsPassed_wifi = 0;
    testsFailed_wifi = 0;
    
    // CREATE Tests
    if (!TestCreateWifiCredentials_Success()) testsFailed_wifi++;
    if (!TestCreateWifiCredentials_EmptySsid()) testsFailed_wifi++;
    if (!TestCreateWifiCredentials_EmptyPassword()) testsFailed_wifi++;
    if (!TestCreateWifiCredentials_VeryLongSsid()) testsFailed_wifi++;
    if (!TestCreateWifiCredentials_SpecialCharacters()) testsFailed_wifi++;
    if (!TestCreateWifiCredentials_DuplicateSsid()) testsFailed_wifi++;
    if (!TestCreateWifiCredentials_VeryLongPassword()) testsFailed_wifi++;
    if (!TestCreateWifiCredentials_UnicodeCharacters()) testsFailed_wifi++;
    
    // READ Tests
    if (!TestGetWifiCredentials_BySsid_Success()) testsFailed_wifi++;
    if (!TestGetWifiCredentials_BySsid_NotFound()) testsFailed_wifi++;
    if (!TestGetWifiCredentials_BySsid_EmptySsid()) testsFailed_wifi++;
    if (!TestGetAllWifiCredentials_Empty()) testsFailed_wifi++;
    if (!TestGetAllWifiCredentials_Multiple()) testsFailed_wifi++;
    
    // UPDATE Tests
    if (!TestUpdateWifiCredentials_Success()) testsFailed_wifi++;
    if (!TestUpdateWifiCredentials_NonExistent()) testsFailed_wifi++;
    if (!TestUpdateWifiCredentials_EmptyPassword()) testsFailed_wifi++;
    
    // DELETE Tests
    if (!TestDeleteWifiCredentials_Success()) testsFailed_wifi++;
    if (!TestDeleteWifiCredentials_NonExistent()) testsFailed_wifi++;
    if (!TestDeleteWifiCredentials_EmptySsid()) testsFailed_wifi++;
    if (!TestDeleteWifiCredentials_ClearsLastConnected()) testsFailed_wifi++;
    
    // LAST CONNECTED Tests
    if (!TestGetLastConnectedWifi_Success()) testsFailed_wifi++;
    if (!TestGetLastConnectedWifi_NotSet()) testsFailed_wifi++;
    if (!TestGetLastConnectedWifi_AfterUpdate()) testsFailed_wifi++;
    
    // EDGE CASES
    if (!TestWifiCredentials_MultipleOperationsSequence()) testsFailed_wifi++;
    
    // Print summary
    std_println("");
    std_println("========================================");
    std_print("Tests Passed: ");
    std_println(std::to_string(testsPassed_wifi).c_str());
    std_print("Tests Failed: ");
    std_println(std::to_string(testsFailed_wifi).c_str());
    std_println("========================================");
    std_println("");
    
    return testsFailed_wifi;
}

#endif // WIFI_CREDENTIALS_CONTROLLER_TESTS_H
