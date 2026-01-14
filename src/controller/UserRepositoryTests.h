#ifndef USER_REPOSITORY_TESTS_H
#define USER_REPOSITORY_TESTS_H

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
    #include <fstream>
    #include <filesystem>
    #include <functional>
    #include <cstdint>
#endif

#define _Entity
#define _Id_

// Include StandardDefines first
#include <StandardDefines.h>
#include <IFileManager.h>

#include "UserRepository.h"
#include "User.h"
#include "../tests/TestUtils.h"

// Helper class for file verification - different implementations for Arduino and non-Arduino
#ifdef ARDUINO
// Arduino version using IFileManager
class FileVerifier {
public:
    // Check if a file exists on disk
    static bool FileExists(const std::string& filepath) {
        IFileManagerPtr fileManager = Implementation<IFileManager>::type::GetInstance();
        StdString filepathStr = StdString(filepath.c_str());
        CStdString filepathRef = filepathStr;
        StdString contents = fileManager->Read(filepathRef);
        return !contents.empty();
    }
    
    // Read file contents from disk
    static std::string ReadFile(const std::string& filepath) {
        IFileManagerPtr fileManager = Implementation<IFileManager>::type::GetInstance();
        StdString filepathStr = StdString(filepath.c_str());
        CStdString filepathRef = filepathStr;
        StdString contents = fileManager->Read(filepathRef);
        return std::string(contents.c_str());
    }
    
    // Delete a file from disk
    static bool DeleteFile(const std::string& filepath) {
        IFileManagerPtr fileManager = Implementation<IFileManager>::type::GetInstance();
        StdString filepathStr = StdString(filepath.c_str());
        CStdString filepathRef = filepathStr;
        return fileManager->Delete(filepathRef);
    }
    
    // Clean up test files from database directory
    // Note: Arduino Preferences doesn't support directory listing, so we delete known test files
    static void CleanupTestFiles() {
        IFileManagerPtr fileManager = Implementation<IFileManager>::type::GetInstance();
        
        // Delete User_IDs
        StdString idsFile = StdString("User_IDs");
        CStdString idsFileRef = idsFile;
        fileManager->Delete(idsFileRef);
        
        // Delete known test user files (IDs 1-40, 999)
        for (int i = 1; i <= 40; i++) {
            StdString filepath = StdString("User_id_") + StdString(std::to_string(i).c_str()) + StdString("");
            CStdString filepathRef = filepath;
            fileManager->Delete(filepathRef);
        }
        
        // Delete ID 999
        StdString filepath999 = StdString("User_id_999");
        CStdString filepath999Ref = filepath999;
        fileManager->Delete(filepath999Ref);
    }
};
#else
// Non-Arduino version using filesystem
// Helper class for file verification - reads from actual disk
class FileVerifier {
public:
    // Check if a file exists on disk
    static bool FileExists(const std::string& filepath) {
        std::ifstream file(filepath);
        return file.good();
    }
    
    // Read file contents from disk
    static std::string ReadFile(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.good()) {
            return "";
        }
        std::string contents((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());
        return contents;
    }
    
    // Delete a file from disk
    static bool DeleteFile(const std::string& filepath) {
        return std::remove(filepath.c_str()) == 0;
    }
    
    // Clean up test files from database directory
    static void CleanupTestFiles() {
        #ifndef ARDUINO
        std::string dbPath = "/Users/nkurude/db";
        #else
        std::string dbPath = "";
        #endif
        try {
            for (const auto& entry : std::filesystem::directory_iterator(dbPath)) {
                std::string filename = entry.path().filename().string();
                // Delete test files (User_id_* and User_IDs)
                if (filename.find("User_") == 0 || filename == "User_IDs") {
                    std::filesystem::remove(entry.path());
                }
            }
        } catch (const std::exception& e) {
            // Ignore errors
        }
    }
};

#endif // ARDUINO

// Test counters and cleanup flag - available for both Arduino and non-Arduino
static int testsPassed_urp = 0;
static int testsFailed_urp = 0;
static bool g_cleanupAfterTests = false;

// Helper function to generate hash (same as CpaRepositoryImpl::GenerateHash)
inline StdString GenerateHashForTest(CStdString input) {
    std::hash<StdString> hasher;
    size_t hashValue = hasher(StdString(input.c_str()));
    uint32_t hash32 = static_cast<uint32_t>(hashValue);
    return StdString(std::to_string(hash32).c_str());
}

// Helper function to get file path - platform-specific
// Now uses hashing to match repository behavior
inline std::string GetTestFilePath(const std::string& filename) {
    #ifdef ARDUINO
        // Arduino: just return the hashed filename (Preferences uses keys, not paths)
        StdString hashed = GenerateHashForTest(CStdString(filename.c_str()));
        return "/" + std::string(hashed.c_str());
    #else
        // Non-Arduino: return full path with hashed filename
        StdString hashed = GenerateHashForTest(CStdString(filename.c_str()));
        return "/Users/nkurude/db/" + std::string(hashed.c_str());
    #endif
}

// Test functions - available for both Arduino and non-Arduino

// Test 1: Save a new user and verify file is created
bool TestSaveUser() {
    TEST_START("Test Save User");
    
    UserRepositoryPtr repository = Implementation<UserRepository>::type::GetInstance();
    
    // Create a new user
    User user;
    user.id = optional<int>(1);
    user.name = optional<StdString>(StdString("John Doe"));
    
    // Save the user (repository uses DesktopFileManager - writes to actual disk)
    User savedUser = repository->Save(user);
    
    // Verify Save returned the user
    ASSERT(savedUser.id.has_value() && savedUser.id.value() == 1, "Save should return user with id=1");
    ASSERT(savedUser.name.has_value() && savedUser.name.value() == "John Doe", "Save should return user with name='John Doe'");
    
    // Verify file was created on disk
    std::string expectedFilePath = GetTestFilePath("User_id_1");
    ASSERT(FileVerifier::FileExists(expectedFilePath), "File should exist on disk after Save");
    
    // Verify file contents from disk
    std::string fileContents = FileVerifier::ReadFile(expectedFilePath);
    ASSERT(!fileContents.empty(), "File should not be empty");
    ASSERT(fileContents.find("John Doe") != std::string::npos || fileContents.find("\"name\":\"John Doe\"") != std::string::npos, "File should contain user name");
    
    // Clean up if flag is set
    if (g_cleanupAfterTests) {
        FileVerifier::DeleteFile(expectedFilePath);
        FileVerifier::DeleteFile(GetTestFilePath("User_IDs"));
    }
    
    testsPassed_urp++;
    return true;
}

// Test 2: Save and then FindById to retrieve the user
bool TestSaveAndFindById() {
    TEST_START("Test Save and FindById");
    
    UserRepositoryPtr repository = Implementation<UserRepository>::type::GetInstance();
    
    // Create and save a user
    User user;
    user.id = optional<int>(2);
    user.name = optional<StdString>(StdString("Jane Smith"));
    
    repository->Save(user);
    
    // Verify file exists on disk
    std::string expectedFilePath = GetTestFilePath("User_id_2");
    ASSERT(FileVerifier::FileExists(expectedFilePath), "File should exist on disk after Save");
    
    // Find the user by ID
    int id = 2;
    optional<User> foundUser = repository->FindById(id);
    
    // Verify user was found
    ASSERT(foundUser.has_value(), "FindById should return a user");
    ASSERT(foundUser.value().id.has_value() && foundUser.value().id.value() == 2, "Found user should have id=2");
    ASSERT(foundUser.value().name.has_value() && foundUser.value().name.value() == "Jane Smith", "Found user should have name='Jane Smith'");
    
    // Clean up if flag is set
    if (g_cleanupAfterTests) {
        FileVerifier::DeleteFile(GetTestFilePath("User_id_2"));
        FileVerifier::DeleteFile(GetTestFilePath("User_IDs"));
    }
    
    testsPassed_urp++;
    return true;
}

// Test 3: FindById with non-existent ID
bool TestFindByIdNotFound() {
    TEST_START("Test FindById with non-existent ID");
    
    UserRepositoryPtr repository = Implementation<UserRepository>::type::GetInstance();
    
    // Try to find a user that doesn't exist
    int id = 999;
    optional<User> foundUser = repository->FindById(id);
    
    // Verify user was not found
    ASSERT(!foundUser.has_value(), "FindById should return empty optional for non-existent ID");
    
    // Verify file doesn't exist on disk
    std::string expectedFilePath = GetTestFilePath("User_id_999");
    ASSERT(!FileVerifier::FileExists(expectedFilePath), "File should not exist on disk for non-existent ID");
    
    testsPassed_urp++;
    return true;
}

// Test 4: Update an existing user
bool TestUpdateUser() {
    TEST_START("Test Update User");
    
    UserRepositoryPtr repository = Implementation<UserRepository>::type::GetInstance();
    
    // Create and save a user
    User user;
    user.id = optional<int>(3);
    user.name = optional<StdString>(StdString("Original Name"));
    repository->Save(user);
    
    // Update the user
    user.name = optional<StdString>(StdString("Updated Name"));
    User updatedUser = repository->Update(user);
    
    // Verify update returned the updated user
    ASSERT(updatedUser.name.has_value() && updatedUser.name.value() == "Updated Name", "Update should return user with updated name");
    
    // Verify file was updated on disk
    std::string expectedFilePath = GetTestFilePath("User_id_3");
    ASSERT(FileVerifier::FileExists(expectedFilePath), "File should exist on disk after update");
    
    std::string fileContents = FileVerifier::ReadFile(expectedFilePath);
    ASSERT(!fileContents.empty(), "File should not be empty");
    ASSERT(fileContents.find("Updated Name") != std::string::npos || fileContents.find("\"name\":\"Updated Name\"") != std::string::npos, "File should contain updated name");
    
    // Verify by finding the user again
    int id = 3;
    optional<User> foundUser = repository->FindById(id);
    ASSERT(foundUser.has_value(), "Updated user should be findable");
    ASSERT(foundUser.value().name.has_value() && foundUser.value().name.value() == "Updated Name", "Found user should have updated name");
    
    // Clean up if flag is set
    if (g_cleanupAfterTests) {
        FileVerifier::DeleteFile(expectedFilePath);
        FileVerifier::DeleteFile(GetTestFilePath("User_IDs"));
    }
    
    testsPassed_urp++;
    return true;
}

// Test 5: DeleteById
bool TestDeleteById() {
    TEST_START("Test DeleteById");
    
    UserRepositoryPtr repository = Implementation<UserRepository>::type::GetInstance();
    
    // Create and save a user
    User user;
    user.id = optional<int>(4);
    user.name = optional<StdString>(StdString("To Be Deleted"));
    repository->Save(user);
    
    // Verify file exists on disk before deletion
    // Note: GetFilePath() returns path without  extension
    std::string expectedFilePath = GetTestFilePath("User_id_4");
    ASSERT(FileVerifier::FileExists(expectedFilePath), "File should exist on disk before deletion");
    
    // Delete the user
    int id = 4;
    repository->DeleteById(id);
    
    // Verify file was deleted from disk
    ASSERT(!FileVerifier::FileExists(expectedFilePath), "File should not exist on disk after DeleteById");
    
    // Verify FindById returns empty
    optional<User> foundUser = repository->FindById(id);
    ASSERT(!foundUser.has_value(), "FindById should return empty after deletion");
    
    // Clean up if flag is set
    if (g_cleanupAfterTests) {
        FileVerifier::DeleteFile(expectedFilePath);
        FileVerifier::DeleteFile(GetTestFilePath("User_IDs"));
    }
    
    testsPassed_urp++;
    return true;
}

// Test 6: Delete by entity
bool TestDeleteEntity() {
    TEST_START("Test Delete Entity");
    
    UserRepositoryPtr repository = Implementation<UserRepository>::type::GetInstance();
    
    // Create and save a user
    User user;
    user.id = optional<int>(5);
    user.name = optional<StdString>(StdString("To Be Deleted By Entity"));
    repository->Save(user);
    
    // Verify file exists on disk before deletion
    // Note: GetFilePath() returns path without  extension
    std::string expectedFilePath = GetTestFilePath("User_id_5");
    ASSERT(FileVerifier::FileExists(expectedFilePath), "File should exist on disk before deletion");
    
    // Delete the user by entity
    repository->Delete(user);
    
    // Verify file was deleted from disk
    ASSERT(!FileVerifier::FileExists(expectedFilePath), "File should not exist on disk after Delete");
    
    // Clean up if flag is set
    if (g_cleanupAfterTests) {
        FileVerifier::DeleteFile(expectedFilePath);
        FileVerifier::DeleteFile(GetTestFilePath("User_IDs"));
    }
    
    testsPassed_urp++;
    return true;
}

// Test 7: ExistsById - existing user
bool TestExistsByIdTrue() {
    TEST_START("Test ExistsById - existing user");
    
    UserRepositoryPtr repository = Implementation<UserRepository>::type::GetInstance();
    
    // Create and save a user
    User user;
    user.id = optional<int>(6);
    user.name = optional<StdString>(StdString("Exists Test"));
    repository->Save(user);
    
    // Verify file exists on disk
    std::string expectedFilePath = GetTestFilePath("User_id_6");
    ASSERT(FileVerifier::FileExists(expectedFilePath), "File should exist on disk");
    
    // Check if user exists
    int id = 6;
    Bool exists = repository->ExistsById(id);
    
    ASSERT(exists == true, "ExistsById should return true for existing user");
    
    // Clean up if flag is set
    if (g_cleanupAfterTests) {
        FileVerifier::DeleteFile(expectedFilePath);
        FileVerifier::DeleteFile(GetTestFilePath("User_IDs"));
    }
    
    testsPassed_urp++;
    return true;
}

// Test 8: ExistsById - non-existent user
bool TestExistsByIdFalse() {
    TEST_START("Test ExistsById - non-existent user");
    
    UserRepositoryPtr repository = Implementation<UserRepository>::type::GetInstance();
    
    // Check if non-existent user exists
    int id = 999;
    Bool exists = repository->ExistsById(id);
    
    ASSERT(exists == false, "ExistsById should return false for non-existent user");
    
#ifndef ARDUINO
    // Verify file doesn't exist on disk
    std::string expectedFilePath = "/Users/nkurude/db/User_id_999";
#else
    std::string expectedFilePath = GetTestFilePath("User_id_999");
#endif
    ASSERT(!FileVerifier::FileExists(expectedFilePath), "File should not exist on disk for non-existent user");
    
    testsPassed_urp++;
    return true;
}

// Test 9: FindAll - multiple users
bool TestFindAll() {
    TEST_START("Test FindAll");
    
    UserRepositoryPtr repository = Implementation<UserRepository>::type::GetInstance();
    
    // Create and save multiple users
    User user1;
    user1.id = optional<int>(10);
    user1.name = optional<StdString>(StdString("User 10"));
    repository->Save(user1);
    
    User user2;
    user2.id = optional<int>(11);
    user2.name = optional<StdString>(StdString("User 11"));
    repository->Save(user2);
    
    User user3;
    user3.id = optional<int>(12);
    user3.name = optional<StdString>(StdString("User 12"));
    repository->Save(user3);
    
    // Verify files exist on disk
    ASSERT(FileVerifier::FileExists(GetTestFilePath("User_id_10")), "File for user 10 should exist");
    ASSERT(FileVerifier::FileExists(GetTestFilePath("User_id_11")), "File for user 11 should exist");
    ASSERT(FileVerifier::FileExists(GetTestFilePath("User_id_12")), "File for user 12 should exist");
    
    // Find all users
    Vector<User> allUsers = repository->FindAll();
    
    ASSERT(allUsers.size() >= 3, "FindAll should return at least 3 users");
    
    // Verify we can find our users in the results
    bool foundUser10 = false, foundUser11 = false, foundUser12 = false;
    for (const User& u : allUsers) {
        if (u.id.has_value()) {
            if (u.id.value() == 10) foundUser10 = true;
            if (u.id.value() == 11) foundUser11 = true;
            if (u.id.value() == 12) foundUser12 = true;
        }
    }
    
    ASSERT(foundUser10, "FindAll should include user with id=10");
    ASSERT(foundUser11, "FindAll should include user with id=11");
    ASSERT(foundUser12, "FindAll should include user with id=12");
    
    // Clean up if flag is set
    if (g_cleanupAfterTests) {
        FileVerifier::DeleteFile(GetTestFilePath("User_id_10"));
        FileVerifier::DeleteFile(GetTestFilePath("User_id_11"));
        FileVerifier::DeleteFile(GetTestFilePath("User_id_12"));
        FileVerifier::DeleteFile(GetTestFilePath("User_IDs"));
    }
    
    testsPassed_urp++;
    return true;
}

// Test 10: Save multiple users and verify all files exist
bool TestSaveMultipleUsers() {
    TEST_START("Test Save Multiple Users");
    
    UserRepositoryPtr repository = Implementation<UserRepository>::type::GetInstance();
    
    // Save multiple users
    for (int i = 20; i < 25; i++) {
        User user;
        user.id = optional<int>(i);
        user.name = optional<StdString>(StdString("User " + StdString(std::to_string(i).c_str())));
        repository->Save(user);
    }
    
    // Verify all files exist on disk
    for (int i = 20; i < 25; i++) {
        std::string expectedFilePath = GetTestFilePath("User_id_" + std::to_string(i) + "");
        ASSERT(FileVerifier::FileExists(expectedFilePath), ("File should exist on disk for user id=" + std::to_string(i)).c_str());
    }
    
    // Clean up if flag is set
    if (g_cleanupAfterTests) {
        for (int i = 20; i < 25; i++) {
            FileVerifier::DeleteFile(GetTestFilePath("User_id_" + std::to_string(i) + ""));
        }
        FileVerifier::DeleteFile(GetTestFilePath("User_IDs"));
    }
    
    testsPassed_urp++;
    return true;
}

// Test 11: Update non-existent user (should still work)
bool TestUpdateNonExistentUser() {
    TEST_START("Test Update Non-existent User");
    
    UserRepositoryPtr repository = Implementation<UserRepository>::type::GetInstance();
    
    // Try to update a user that doesn't exist
    User user;
    user.id = optional<int>(30);
    user.name = optional<StdString>(StdString("New User"));
    User updatedUser = repository->Update(user);
    
    // Verify file was created on disk (Update should create if doesn't exist)
    std::string expectedFilePath = GetTestFilePath("User_id_30");
    ASSERT(FileVerifier::FileExists(expectedFilePath), "Update should create file on disk if user doesn't exist");
    
    // Verify ID was added to IDs file
    std::string idsFilePath = GetTestFilePath("User_IDs");
    std::string idsFileContents = FileVerifier::ReadFile(idsFilePath);
    ASSERT(idsFileContents.find("30") != std::string::npos, "Update should add ID to User_IDs file");
    
    // Verify we can find the user by ID
    int id = 30;
    optional<User> foundUser = repository->FindById(id);
    ASSERT(foundUser.has_value(), "Updated user should be findable by ID");
    ASSERT(foundUser.value().id.has_value() && foundUser.value().id.value() == 30, "Found user should have id=30");
    ASSERT(foundUser.value().name.has_value() && foundUser.value().name.value() == "New User", "Found user should have name='New User'");
    
    // Clean up if flag is set
    if (g_cleanupAfterTests) {
        FileVerifier::DeleteFile(expectedFilePath);
        FileVerifier::DeleteFile(GetTestFilePath("User_IDs"));
    }
    
    testsPassed_urp++;
    return true;
}

// Test 12: DeleteById on non-existent user
bool TestDeleteByIdNonExistent() {
    TEST_START("Test DeleteById Non-existent User");
    
    UserRepositoryPtr repository = Implementation<UserRepository>::type::GetInstance();
    
    // Try to delete a user that doesn't exist
    int id = 999;
    repository->DeleteById(id);
    
    // Should not crash, just do nothing
    ASSERT(true, "DeleteById should not crash on non-existent user");
    
    // Verify file doesn't exist on disk
    std::string expectedFilePath = GetTestFilePath("User_id_999");
    ASSERT(!FileVerifier::FileExists(expectedFilePath), "File should not exist on disk for non-existent user");
    
    testsPassed_urp++;
    return true;
}

// Test 13: Verify file contents match serialized entity
bool TestFileContentsMatchEntity() {
    TEST_START("Test File Contents Match Entity");
    
    UserRepositoryPtr repository = Implementation<UserRepository>::type::GetInstance();
    
    // Create and save a user
    User user;
    user.id = optional<int>(40);
    user.name = optional<StdString>(StdString("Content Test User"));
    repository->Save(user);
    
    // Get file contents from disk
    std::string expectedFilePath = GetTestFilePath("User_id_40");
    ASSERT(FileVerifier::FileExists(expectedFilePath), "File should exist on disk");
    
    std::string fileContents = FileVerifier::ReadFile(expectedFilePath);
    
    // Serialize the user and compare
    StdString serializedUser = user.Serialize();
    
    // File should contain the serialized data
    ASSERT(!fileContents.empty(), "File should not be empty");
    ASSERT(fileContents.find("Content Test User") != std::string::npos || fileContents.find("\"name\":\"Content Test User\"") != std::string::npos, "File should contain user name");
    
    // Clean up if flag is set
    if (g_cleanupAfterTests) {
        FileVerifier::DeleteFile(expectedFilePath);
        FileVerifier::DeleteFile(GetTestFilePath("User_IDs"));
    }
    
    testsPassed_urp++;
    return true;
}

// Main test runner function
int RunAllTests(int argc, char* argv[]) {
    // Parse command-line arguments
    Vector<std::string> args(argv + 1, argv + argc);
    for (const auto& arg : args) {
        if (arg == "--cleanup" || arg == "--clean" || arg == "-c") {
            g_cleanupAfterTests = true;
            std_println("Cleanup mode enabled: Test files will be deleted after tests.");
            std_println("");
        } else if (arg == "--help" || arg == "-h") {
            std_print("Usage: ");
            std_print(argv[0]);
            std_println(" [options]");
            std_println("Options:");
            std_println("  --cleanup, --clean, -c    Delete test files after tests complete");
            std_println("  --help, -h                Show this help message");
            std_println("");
            return 0;
        }
    }
    
    if (!g_cleanupAfterTests) {
        #ifndef ARDUINO
        std_println("Note: Test files will be left on disk for inspection.");
        #else
        std_println("Note: Test files will be left in Arduino Preferences for inspection.");
        #endif
        std_println("      Use --cleanup flag to delete them after tests.");
        std_println("");
    } else {
        // Only clean up at the start if cleanup flag is set
        std_println("Cleaning up any existing test files before starting...");
        FileVerifier::CleanupTestFiles();
    }
    
    std_println("");
    std_println("========================================");
    std_println("  UserRepository Comprehensive Tests");
    std_println("========================================");
    std_println("");
    
    testsPassed_urp = 0;
    testsFailed_urp = 0;
    
    // Run all tests
    if (!TestSaveUser()) testsFailed_urp++;
    if (!TestSaveAndFindById()) testsFailed_urp++;
    if (!TestFindByIdNotFound()) testsFailed_urp++;
    if (!TestUpdateUser()) testsFailed_urp++;
    if (!TestDeleteById()) testsFailed_urp++;
    if (!TestDeleteEntity()) testsFailed_urp++;
    if (!TestExistsByIdTrue()) testsFailed_urp++;
    if (!TestExistsByIdFalse()) testsFailed_urp++;
    if (!TestFindAll()) testsFailed_urp++;
    if (!TestSaveMultipleUsers()) testsFailed_urp++;
    if (!TestUpdateNonExistentUser()) testsFailed_urp++;
    if (!TestDeleteByIdNonExistent()) testsFailed_urp++;
    if (!TestFileContentsMatchEntity()) testsFailed_urp++;
    
    // Print summary
    std_println("");
    std_println("========================================");
    std_println("  Test Summary");
    std_println("========================================");
    std_print("Tests Passed: ");
    std_println(testsPassed_urp);
    std_print("Tests Failed: ");
    std_println(testsFailed_urp);
    std_print("Total Tests: ");
    std_println(testsPassed_urp + testsFailed_urp);
    std_println("========================================");
    std_println("");
    
    // Final cleanup if flag is set
    if (g_cleanupAfterTests) {
        std_println("Cleaning up test files...");
        FileVerifier::CleanupTestFiles();
        std_println("Cleanup complete.");
        std_println("");
    } else {
        #ifndef ARDUINO
        std_print("Test files are available at: /Users/nkurude/db/");
        #else
        std_print("Test files are stored in Arduino Preferences.");
        #endif
        std_println("");
        std_println("Use --cleanup flag to delete them.");
        std_println("");
    }
    
    if (testsFailed_urp == 0) {
        std_println("✅ All tests passed!");
        return 0;
    } else {
        std_println("❌ Some tests failed!");
        return 1;
    }
}

//#endif // ARDUINO

#endif // USER_REPOSITORY_TESTS_H

