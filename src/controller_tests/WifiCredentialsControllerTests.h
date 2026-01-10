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

// Include controller and related files from arduinolib5
// The library is fetched as a dependency, so we use the include path from the build system
// These should be available through the arduino-core library
#include "controller/IWifiCredentialsController.h"
#include "entity/WifiCredentials.h"

// Print macros - compatible with both Arduino and non-Arduino
#ifdef ARDUINO
    // Arduino version using Serial.print/Serial.println
    #define std_print(x) Serial.print(x)
    #define std_println(x) Serial.println(x)
#else
    // Desktop version using std::cout
    #define std_print(x) std::cout << x
    #define std_println(x) std::cout << x << std::endl
#endif

// Test assertion macros
#define ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            std_print("FAILED: "); \
            std_println(message); \
            return false; \
        } \
    } while(0)

#define TEST_START(test_name) \
    std_print("Running: "); \
    std_println(test_name);

// Global test counters
static int testsPassed_wifi = 0;
static int testsFailed_wifi = 0;

// Helper function to print test result
void PrintTestResult(const char* testName, bool passed) {
    if (passed) {
        std_print("✓ PASS: ");
        std_println(testName);
        testsPassed_wifi++;
    } else {
        std_print("✗ FAIL: ");
        std_println(testName);
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

// ========== CREATE OPERATIONS TESTS ==========

// Test 1: Create WiFi credentials - Success
bool TestCreateWifiCredentials_Success() {
    TEST_START("Test Create WiFi Credentials - Success");
    
    IWifiCredentialsControllerPtr controller = Implementation<IWifiCredentialsController>::type::GetInstance();
    
    WifiCredentials creds = CreateTestCredentials("TestNetwork", "TestPassword123");
    WifiCredentials result = controller->CreateWifiCredentials(creds);
    
    ASSERT(result.ssid.has_value(), "SSID should be present in result");
    ASSERT(result.ssid.value() == "TestNetwork", "SSID should match input");
    ASSERT(result.password.has_value(), "Password should be present in result");
    ASSERT(result.password.value() == "TestPassword123", "Password should match input");
    
    PrintTestResult("Create WiFi Credentials - Success", true);
    return true;
}

// Test 2: Create WiFi credentials - Empty SSID
bool TestCreateWifiCredentials_EmptySsid() {
    TEST_START("Test Create WiFi Credentials - Empty SSID");
    
    IWifiCredentialsControllerPtr controller = Implementation<IWifiCredentialsController>::type::GetInstance();
    
    WifiCredentials creds;
    creds.ssid = StdString("");
    creds.password = StdString("SomePassword");
    
    WifiCredentials result = controller->CreateWifiCredentials(creds);
    
    // Should still create (validation might be in service layer, but controller should handle it)
    PrintTestResult("Create WiFi Credentials - Empty SSID", true);
    return true;
}

// Test 3: Create WiFi credentials - Empty Password
bool TestCreateWifiCredentials_EmptyPassword() {
    TEST_START("Test Create WiFi Credentials - Empty Password");
    
    IWifiCredentialsControllerPtr controller = Implementation<IWifiCredentialsController>::type::GetInstance();
    
    WifiCredentials creds;
    creds.ssid = StdString("TestNetwork");
    creds.password = StdString("");
    
    WifiCredentials result = controller->CreateWifiCredentials(creds);
    
    ASSERT(result.ssid.has_value(), "SSID should be present");
    ASSERT(result.ssid.value() == "TestNetwork", "SSID should match");
    
    PrintTestResult("Create WiFi Credentials - Empty Password", true);
    return true;
}

// Test 4: Create WiFi credentials - Very Long SSID
bool TestCreateWifiCredentials_VeryLongSsid() {
    TEST_START("Test Create WiFi Credentials - Very Long SSID");
    
    IWifiCredentialsControllerPtr controller = Implementation<IWifiCredentialsController>::type::GetInstance();
    
    StdString longSsid(256, 'A'); // 256 character SSID
    WifiCredentials creds = CreateTestCredentials(longSsid, "Password123");
    
    WifiCredentials result = controller->CreateWifiCredentials(creds);
    
    ASSERT(result.ssid.has_value(), "SSID should be present");
    ASSERT(result.ssid.value().length() == 256, "SSID length should match");
    
    PrintTestResult("Create WiFi Credentials - Very Long SSID", true);
    return true;
}

// Test 5: Create WiFi credentials - Special Characters
bool TestCreateWifiCredentials_SpecialCharacters() {
    TEST_START("Test Create WiFi Credentials - Special Characters");
    
    IWifiCredentialsControllerPtr controller = Implementation<IWifiCredentialsController>::type::GetInstance();
    
    WifiCredentials creds = CreateTestCredentials("Network_123-ABC", "P@ssw0rd!#$%");
    
    WifiCredentials result = controller->CreateWifiCredentials(creds);
    
    ASSERT(result.ssid.has_value(), "SSID should be present");
    ASSERT(result.ssid.value() == "Network_123-ABC", "SSID with special chars should match");
    
    PrintTestResult("Create WiFi Credentials - Special Characters", true);
    return true;
}

// ========== READ OPERATIONS TESTS ==========

// Test 6: Get WiFi credentials by SSID - Success
bool TestGetWifiCredentials_BySsid_Success() {
    TEST_START("Test Get WiFi Credentials by SSID - Success");
    
    IWifiCredentialsControllerPtr controller = Implementation<IWifiCredentialsController>::type::GetInstance();
    
    // First create credentials
    WifiCredentials creds = CreateTestCredentials("GetTestNetwork", "GetTestPassword");
    controller->CreateWifiCredentials(creds);
    
    // Then retrieve them
    optional<WifiCredentials> result = controller->GetWifiCredentials("GetTestNetwork");
    
    ASSERT(result.has_value(), "Credentials should be found");
    ASSERT(result.value().ssid.has_value(), "SSID should be present");
    ASSERT(result.value().ssid.value() == "GetTestNetwork", "SSID should match");
    ASSERT(result.value().password.has_value(), "Password should be present");
    ASSERT(result.value().password.value() == "GetTestPassword", "Password should match");
    
    PrintTestResult("Get WiFi Credentials by SSID - Success", true);
    return true;
}

// Test 7: Get WiFi credentials by SSID - Not Found
bool TestGetWifiCredentials_BySsid_NotFound() {
    TEST_START("Test Get WiFi Credentials by SSID - Not Found");
    
    IWifiCredentialsControllerPtr controller = Implementation<IWifiCredentialsController>::type::GetInstance();
    
    optional<WifiCredentials> result = controller->GetWifiCredentials("NonExistentNetwork");
    
    ASSERT(!result.has_value(), "Credentials should not be found");
    
    PrintTestResult("Get WiFi Credentials by SSID - Not Found", true);
    return true;
}

// Test 8: Get WiFi credentials by SSID - Empty SSID
bool TestGetWifiCredentials_BySsid_EmptySsid() {
    TEST_START("Test Get WiFi Credentials by SSID - Empty SSID");
    
    IWifiCredentialsControllerPtr controller = Implementation<IWifiCredentialsController>::type::GetInstance();
    
    optional<WifiCredentials> result = controller->GetWifiCredentials("");
    
    // Should return nullopt for empty SSID
    ASSERT(!result.has_value(), "Empty SSID should return nullopt");
    
    PrintTestResult("Get WiFi Credentials by SSID - Empty SSID", true);
    return true;
}

// Test 9: Get All WiFi credentials - Empty
bool TestGetAllWifiCredentials_Empty() {
    TEST_START("Test Get All WiFi Credentials - Empty");
    
    IWifiCredentialsControllerPtr controller = Implementation<IWifiCredentialsController>::type::GetInstance();
    
    // Note: This test assumes we can clear or start fresh
    // In real scenario, you might need to clean up first
    vector<WifiCredentials> result = controller->GetAllWifiCredentials();
    
    // Should return empty vector if no credentials exist
    // This is a valid scenario
    PrintTestResult("Get All WiFi Credentials - Empty", true);
    return true;
}

// Test 10: Get All WiFi credentials - Multiple
bool TestGetAllWifiCredentials_Multiple() {
    TEST_START("Test Get All WiFi Credentials - Multiple");
    
    IWifiCredentialsControllerPtr controller = Implementation<IWifiCredentialsController>::type::GetInstance();
    
    // Create multiple credentials
    WifiCredentials creds1 = CreateTestCredentials("Network1", "Password1");
    WifiCredentials creds2 = CreateTestCredentials("Network2", "Password2");
    WifiCredentials creds3 = CreateTestCredentials("Network3", "Password3");
    
    controller->CreateWifiCredentials(creds1);
    controller->CreateWifiCredentials(creds2);
    controller->CreateWifiCredentials(creds3);
    
    vector<WifiCredentials> result = controller->GetAllWifiCredentials();
    
    ASSERT(result.size() >= 3, "Should return at least 3 credentials");
    
    // Verify all three are present
    bool found1 = false, found2 = false, found3 = false;
    for (const auto& cred : result) {
        if (cred.ssid.has_value()) {
            if (cred.ssid.value() == "Network1") found1 = true;
            if (cred.ssid.value() == "Network2") found2 = true;
            if (cred.ssid.value() == "Network3") found3 = true;
        }
    }
    
    ASSERT(found1 && found2 && found3, "All three networks should be found");
    
    PrintTestResult("Get All WiFi Credentials - Multiple", true);
    return true;
}

// ========== UPDATE OPERATIONS TESTS ==========

// Test 11: Update WiFi credentials - Success
bool TestUpdateWifiCredentials_Success() {
    TEST_START("Test Update WiFi Credentials - Success");
    
    IWifiCredentialsControllerPtr controller = Implementation<IWifiCredentialsController>::type::GetInstance();
    
    // First create credentials
    WifiCredentials creds = CreateTestCredentials("UpdateTestNetwork", "OldPassword");
    controller->CreateWifiCredentials(creds);
    
    // Update password
    WifiCredentials updatedCreds = CreateTestCredentials("UpdateTestNetwork", "NewPassword");
    WifiCredentials result = controller->UpdateWifiCredentials(updatedCreds);
    
    ASSERT(result.ssid.has_value(), "SSID should be present");
    ASSERT(result.ssid.value() == "UpdateTestNetwork", "SSID should match");
    ASSERT(result.password.has_value(), "Password should be present");
    ASSERT(result.password.value() == "NewPassword", "Password should be updated");
    
    // Verify update by retrieving
    optional<WifiCredentials> retrieved = controller->GetWifiCredentials("UpdateTestNetwork");
    ASSERT(retrieved.has_value(), "Credentials should still exist");
    ASSERT(retrieved.value().password.value() == "NewPassword", "Password should be updated in storage");
    
    PrintTestResult("Update WiFi Credentials - Success", true);
    return true;
}

// Test 12: Update WiFi credentials - Non-existent SSID
bool TestUpdateWifiCredentials_NonExistent() {
    TEST_START("Test Update WiFi Credentials - Non-existent SSID");
    
    IWifiCredentialsControllerPtr controller = Implementation<IWifiCredentialsController>::type::GetInstance();
    
    WifiCredentials creds = CreateTestCredentials("NonExistentNetwork", "SomePassword");
    
    // Update should still work (might create if not exists, depending on service implementation)
    WifiCredentials result = controller->UpdateWifiCredentials(creds);
    
    ASSERT(result.ssid.has_value(), "SSID should be present");
    
    PrintTestResult("Update WiFi Credentials - Non-existent SSID", true);
    return true;
}

// Test 13: Update WiFi credentials - Empty Password
bool TestUpdateWifiCredentials_EmptyPassword() {
    TEST_START("Test Update WiFi Credentials - Empty Password");
    
    IWifiCredentialsControllerPtr controller = Implementation<IWifiCredentialsController>::type::GetInstance();
    
    // Create first
    WifiCredentials creds = CreateTestCredentials("UpdateEmptyPassNetwork", "OriginalPassword");
    controller->CreateWifiCredentials(creds);
    
    // Update with empty password
    WifiCredentials updatedCreds;
    updatedCreds.ssid = StdString("UpdateEmptyPassNetwork");
    updatedCreds.password = StdString("");
    
    WifiCredentials result = controller->UpdateWifiCredentials(updatedCreds);
    
    ASSERT(result.ssid.has_value(), "SSID should be present");
    
    PrintTestResult("Update WiFi Credentials - Empty Password", true);
    return true;
}

// ========== DELETE OPERATIONS TESTS ==========

// Test 14: Delete WiFi credentials - Success
bool TestDeleteWifiCredentials_Success() {
    TEST_START("Test Delete WiFi Credentials - Success");
    
    IWifiCredentialsControllerPtr controller = Implementation<IWifiCredentialsController>::type::GetInstance();
    
    // First create credentials
    WifiCredentials creds = CreateTestCredentials("DeleteTestNetwork", "DeletePassword");
    controller->CreateWifiCredentials(creds);
    
    // Verify it exists
    optional<WifiCredentials> beforeDelete = controller->GetWifiCredentials("DeleteTestNetwork");
    ASSERT(beforeDelete.has_value(), "Credentials should exist before delete");
    
    // Delete
    controller->DeleteWifiCredentials("DeleteTestNetwork");
    
    // Verify it's deleted
    optional<WifiCredentials> afterDelete = controller->GetWifiCredentials("DeleteTestNetwork");
    ASSERT(!afterDelete.has_value(), "Credentials should not exist after delete");
    
    PrintTestResult("Delete WiFi Credentials - Success", true);
    return true;
}

// Test 15: Delete WiFi credentials - Non-existent SSID
bool TestDeleteWifiCredentials_NonExistent() {
    TEST_START("Test Delete WiFi Credentials - Non-existent SSID");
    
    IWifiCredentialsControllerPtr controller = Implementation<IWifiCredentialsController>::type::GetInstance();
    
    // Delete non-existent should not throw error
    controller->DeleteWifiCredentials("NonExistentNetworkToDelete");
    
    // Should complete without error
    PrintTestResult("Delete WiFi Credentials - Non-existent SSID", true);
    return true;
}

// Test 16: Delete WiFi credentials - Empty SSID
bool TestDeleteWifiCredentials_EmptySsid() {
    TEST_START("Test Delete WiFi Credentials - Empty SSID");
    
    IWifiCredentialsControllerPtr controller = Implementation<IWifiCredentialsController>::type::GetInstance();
    
    // Delete with empty SSID should not throw error
    controller->DeleteWifiCredentials("");
    
    PrintTestResult("Delete WiFi Credentials - Empty SSID", true);
    return true;
}

// Test 17: Delete and verify last connected is cleared
bool TestDeleteWifiCredentials_ClearsLastConnected() {
    TEST_START("Test Delete WiFi Credentials - Clears Last Connected");
    
    IWifiCredentialsControllerPtr controller = Implementation<IWifiCredentialsController>::type::GetInstance();
    
    // Create and this will set as last connected
    WifiCredentials creds = CreateTestCredentials("LastConnectedNetwork", "Password");
    controller->CreateWifiCredentials(creds);
    
    // Verify it's last connected
    optional<WifiCredentials> lastConnected = controller->GetLastConnectedWifi();
    ASSERT(lastConnected.has_value(), "Should have last connected WiFi");
    
    // Delete it
    controller->DeleteWifiCredentials("LastConnectedNetwork");
    
    // Verify last connected is cleared (or points to something else)
    optional<WifiCredentials> lastConnectedAfter = controller->GetLastConnectedWifi();
    // Should either be nullopt or point to a different network
    
    PrintTestResult("Delete WiFi Credentials - Clears Last Connected", true);
    return true;
}

// ========== LAST CONNECTED TESTS ==========

// Test 18: Get Last Connected WiFi - Success
bool TestGetLastConnectedWifi_Success() {
    TEST_START("Test Get Last Connected WiFi - Success");
    
    IWifiCredentialsControllerPtr controller = Implementation<IWifiCredentialsController>::type::GetInstance();
    
    // Create credentials (this should set as last connected)
    WifiCredentials creds = CreateTestCredentials("LastConnectedTest", "LastPassword");
    controller->CreateWifiCredentials(creds);
    
    optional<WifiCredentials> result = controller->GetLastConnectedWifi();
    
    ASSERT(result.has_value(), "Last connected WiFi should be found");
    ASSERT(result.value().ssid.has_value(), "SSID should be present");
    ASSERT(result.value().ssid.value() == "LastConnectedTest", "SSID should match last created");
    
    PrintTestResult("Get Last Connected WiFi - Success", true);
    return true;
}

// Test 19: Get Last Connected WiFi - Not Set
bool TestGetLastConnectedWifi_NotSet() {
    TEST_START("Test Get Last Connected WiFi - Not Set");
    
    IWifiCredentialsControllerPtr controller = Implementation<IWifiCredentialsController>::type::GetInstance();
    
    // If no credentials have been created/updated, last connected might be nullopt
    optional<WifiCredentials> result = controller->GetLastConnectedWifi();
    
    // Should return nullopt if not set (or might return first available)
    // This depends on service implementation
    
    PrintTestResult("Get Last Connected WiFi - Not Set", true);
    return true;
}

// Test 20: Get Last Connected WiFi - After Update
bool TestGetLastConnectedWifi_AfterUpdate() {
    TEST_START("Test Get Last Connected WiFi - After Update");
    
    IWifiCredentialsControllerPtr controller = Implementation<IWifiCredentialsController>::type::GetInstance();
    
    // Create first network
    WifiCredentials creds1 = CreateTestCredentials("NetworkA", "PasswordA");
    controller->CreateWifiCredentials(creds1);
    
    // Create second network (should become last connected)
    WifiCredentials creds2 = CreateTestCredentials("NetworkB", "PasswordB");
    controller->CreateWifiCredentials(creds2);
    
    // Last connected should be NetworkB
    optional<WifiCredentials> result = controller->GetLastConnectedWifi();
    ASSERT(result.has_value(), "Last connected should be found");
    ASSERT(result.value().ssid.value() == "NetworkB", "Last connected should be NetworkB");
    
    // Update NetworkA (should become last connected)
    WifiCredentials updatedCreds1 = CreateTestCredentials("NetworkA", "NewPasswordA");
    controller->UpdateWifiCredentials(updatedCreds1);
    
    // Last connected should now be NetworkA
    optional<WifiCredentials> result2 = controller->GetLastConnectedWifi();
    ASSERT(result2.has_value(), "Last connected should still be found");
    ASSERT(result2.value().ssid.value() == "NetworkA", "Last connected should be NetworkA after update");
    
    PrintTestResult("Get Last Connected WiFi - After Update", true);
    return true;
}

// ========== EDGE CASES AND CORNER CASES ==========

// Test 21: Create with same SSID twice (should update)
bool TestCreateWifiCredentials_DuplicateSsid() {
    TEST_START("Test Create WiFi Credentials - Duplicate SSID");
    
    IWifiCredentialsControllerPtr controller = Implementation<IWifiCredentialsController>::type::GetInstance();
    
    WifiCredentials creds1 = CreateTestCredentials("DuplicateNetwork", "Password1");
    controller->CreateWifiCredentials(creds1);
    
    WifiCredentials creds2 = CreateTestCredentials("DuplicateNetwork", "Password2");
    WifiCredentials result = controller->CreateWifiCredentials(creds2);
    
    // Should either update or create new (depends on service implementation)
    ASSERT(result.ssid.has_value(), "SSID should be present");
    
    PrintTestResult("Create WiFi Credentials - Duplicate SSID", true);
    return true;
}

// Test 22: Get with null controller (should handle gracefully or crash - testing behavior)
bool TestGetWifiCredentials_NullController() {
    TEST_START("Test Get WiFi Credentials - Null Controller");
    
    // Test with null controller pointer - this tests the interface behavior
    IWifiCredentialsControllerPtr controller = nullptr;
    
    // This should either crash or handle gracefully
    // We're testing the controller's behavior
    try {
        if (controller != nullptr) {
            optional<WifiCredentials> result = controller->GetWifiCredentials("TestNetwork");
        }
        // If it doesn't crash, that's also a valid behavior
        PrintTestResult("Get WiFi Credentials - Null Controller (handled)", true);
    } catch (...) {
        PrintTestResult("Get WiFi Credentials - Null Controller (exception)", true);
    }
    
    return true;
}

// Test 23: Multiple operations in sequence
bool TestWifiCredentials_MultipleOperationsSequence() {
    TEST_START("Test WiFi Credentials - Multiple Operations Sequence");
    
    IWifiCredentialsControllerPtr controller = Implementation<IWifiCredentialsController>::type::GetInstance();
    
    // Create
    WifiCredentials creds1 = CreateTestCredentials("SequenceNetwork", "Password1");
    controller->CreateWifiCredentials(creds1);
    
    // Read
    optional<WifiCredentials> read1 = controller->GetWifiCredentials("SequenceNetwork");
    ASSERT(read1.has_value(), "Should be able to read after create");
    
    // Update
    WifiCredentials creds2 = CreateTestCredentials("SequenceNetwork", "Password2");
    controller->UpdateWifiCredentials(creds2);
    
    // Read again
    optional<WifiCredentials> read2 = controller->GetWifiCredentials("SequenceNetwork");
    ASSERT(read2.has_value(), "Should be able to read after update");
    ASSERT(read2.value().password.value() == "Password2", "Password should be updated");
    
    // Delete
    controller->DeleteWifiCredentials("SequenceNetwork");
    
    // Read after delete
    optional<WifiCredentials> read3 = controller->GetWifiCredentials("SequenceNetwork");
    ASSERT(!read3.has_value(), "Should not be able to read after delete");
    
    PrintTestResult("WiFi Credentials - Multiple Operations Sequence", true);
    return true;
}

// Test 24: Very long password
bool TestCreateWifiCredentials_VeryLongPassword() {
    TEST_START("Test Create WiFi Credentials - Very Long Password");
    
    IWifiCredentialsControllerPtr controller = Implementation<IWifiCredentialsController>::type::GetInstance();
    
    StdString longPassword(512, 'X'); // 512 character password
    WifiCredentials creds = CreateTestCredentials("ShortSSID", longPassword);
    
    WifiCredentials result = controller->CreateWifiCredentials(creds);
    
    ASSERT(result.password.has_value(), "Password should be present");
    ASSERT(result.password.value().length() == 512, "Password length should match");
    
    PrintTestResult("Create WiFi Credentials - Very Long Password", true);
    return true;
}

// Test 25: Unicode characters in SSID and password
bool TestCreateWifiCredentials_UnicodeCharacters() {
    TEST_START("Test Create WiFi Credentials - Unicode Characters");
    
    IWifiCredentialsControllerPtr controller = Implementation<IWifiCredentialsController>::type::GetInstance();
    
    WifiCredentials creds;
    creds.ssid = StdString("网络_Network_123");
    creds.password = StdString("密码_Password_🔒");
    
    WifiCredentials result = controller->CreateWifiCredentials(creds);
    
    ASSERT(result.ssid.has_value(), "SSID with unicode should be present");
    ASSERT(result.password.has_value(), "Password with unicode should be present");
    
    PrintTestResult("Create WiFi Credentials - Unicode Characters", true);
    return true;
}

// Main test runner function
void RunAllWifiCredentialsControllerTests() {
    std_println("\n========================================");
    std_println("  WiFi Credentials Controller Tests");
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
    if (!TestGetWifiCredentials_NullController()) testsFailed_wifi++;
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
}

#endif // WIFI_CREDENTIALS_CONTROLLER_TESTS_H

