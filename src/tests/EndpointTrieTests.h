#ifndef ENDPOINT_TRIE_TESTS_H
#define ENDPOINT_TRIE_TESTS_H

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
#include "EndpointTrie.h"
#include "TestUtils.h"

// Test counters
static int testsPassed_endpoint_trie = 0;
static int testsFailed_endpoint_trie = 0;

// ========== BASIC INSERTION AND SEARCH TESTS ==========

// Test 1: Insert and search simple literal path
bool TestInsertAndSearchLiteralPath() {
    TEST_START("Test Insert and Search Literal Path");
    
    EndpointTrie trie;
    trie.Insert("/api/user/create");
    
    EndpointMatchResult result = trie.Search("/api/user/create");
    
    ASSERT(result.found, "Should find exact match");
    ASSERT(result.pattern == "/api/user/create", "Pattern should match");
    ASSERT(result.variables.empty(), "No variables for literal path");
    
    testsPassed_endpoint_trie++;
    return true;
}

// Test 2: Insert and search path with single variable
bool TestInsertAndSearchSingleVariable() {
    TEST_START("Test Insert and Search Single Variable");
    
    EndpointTrie trie;
    trie.Insert("/api/user/{userId}/get");
    
    EndpointMatchResult result = trie.Search("/api/user/123/get");
    
    ASSERT(result.found, "Should find match");
    ASSERT(result.pattern == "/api/user/{userId}/get", "Pattern should match");
    ASSERT(result.variables.size() == 1, "Should have one variable");
    ASSERT(result.variables.find("userId") != result.variables.end(), "Should have userId variable");
    ASSERT(result.variables["userId"] == "123", "userId should be 123");
    
    testsPassed_endpoint_trie++;
    return true;
}

// Test 3: Insert and search path with multiple variables
bool TestInsertAndSearchMultipleVariables() {
    TEST_START("Test Insert and Search Multiple Variables");
    
    EndpointTrie trie;
    trie.Insert("/hello/{mno}/{pqr}/{def}");
    
    EndpointMatchResult result = trie.Search("/hello/someval1/someval2/someval3");
    
    ASSERT(result.found, "Should find match");
    ASSERT(result.pattern == "/hello/{mno}/{pqr}/{def}", "Pattern should match");
    ASSERT(result.variables.size() == 3, "Should have three variables");
    ASSERT(result.variables["mno"] == "someval1", "mno should be someval1");
    ASSERT(result.variables["pqr"] == "someval2", "pqr should be someval2");
    ASSERT(result.variables["def"] == "someval3", "def should be someval3");
    
    testsPassed_endpoint_trie++;
    return true;
}

// Test 4: Insert and search path with mixed literal and variables
bool TestInsertAndSearchMixedPath() {
    TEST_START("Test Insert and Search Mixed Path");
    
    EndpointTrie trie;
    trie.Insert("/api/user/{userId}/profile/{section}");
    
    EndpointMatchResult result = trie.Search("/api/user/456/profile/settings");
    
    ASSERT(result.found, "Should find match");
    ASSERT(result.pattern == "/api/user/{userId}/profile/{section}", "Pattern should match");
    ASSERT(result.variables.size() == 2, "Should have two variables");
    ASSERT(result.variables["userId"] == "456", "userId should be 456");
    ASSERT(result.variables["section"] == "settings", "section should be settings");
    
    testsPassed_endpoint_trie++;
    return true;
}

// Test 5: Search with no match
bool TestSearchNoMatch() {
    TEST_START("Test Search No Match");
    
    EndpointTrie trie;
    trie.Insert("/api/user/{userId}/get");
    
    EndpointMatchResult result = trie.Search("/api/user/123/delete");
    
    ASSERT(!result.found, "Should not find match");
    ASSERT(result.pattern.empty(), "Pattern should be empty");
    ASSERT(result.variables.empty(), "Variables should be empty");
    
    testsPassed_endpoint_trie++;
    return true;
}

// Test 6: Search with shorter path
bool TestSearchShorterPath() {
    TEST_START("Test Search Shorter Path");
    
    EndpointTrie trie;
    trie.Insert("/api/user/{userId}/get");
    
    EndpointMatchResult result = trie.Search("/api/user/123");
    
    ASSERT(!result.found, "Should not find match for shorter path");
    
    testsPassed_endpoint_trie++;
    return true;
}

// Test 7: Search with longer path
bool TestSearchLongerPath() {
    TEST_START("Test Search Longer Path");
    
    EndpointTrie trie;
    trie.Insert("/api/user/{userId}/get");
    
    EndpointMatchResult result = trie.Search("/api/user/123/get/extra");
    
    ASSERT(!result.found, "Should not find match for longer path");
    
    testsPassed_endpoint_trie++;
    return true;
}

// ========== MULTIPLE ENDPOINTS TESTS ==========

// Test 8: Multiple endpoints with different patterns
bool TestMultipleEndpoints() {
    TEST_START("Test Multiple Endpoints");
    
    EndpointTrie trie;
    trie.Insert("/api/user/create");
    trie.Insert("/api/user/{userId}/get");
    trie.Insert("/api/user/{userId}/update");
    trie.Insert("/api/user/{userId}/delete");
    
    // Test first endpoint
    EndpointMatchResult result1 = trie.Search("/api/user/create");
    ASSERT(result1.found, "Should find create endpoint");
    ASSERT(result1.pattern == "/api/user/create", "Should match create pattern");
    
    // Test second endpoint
    EndpointMatchResult result2 = trie.Search("/api/user/789/get");
    ASSERT(result2.found, "Should find get endpoint");
    ASSERT(result2.pattern == "/api/user/{userId}/get", "Should match get pattern");
    ASSERT(result2.variables["userId"] == "789", "userId should be 789");
    
    // Test third endpoint
    EndpointMatchResult result3 = trie.Search("/api/user/101/update");
    ASSERT(result3.found, "Should find update endpoint");
    ASSERT(result3.pattern == "/api/user/{userId}/update", "Should match update pattern");
    ASSERT(result3.variables["userId"] == "101", "userId should be 101");
    
    // Test fourth endpoint
    EndpointMatchResult result4 = trie.Search("/api/user/202/delete");
    ASSERT(result4.found, "Should find delete endpoint");
    ASSERT(result4.pattern == "/api/user/{userId}/delete", "Should match delete pattern");
    ASSERT(result4.variables["userId"] == "202", "userId should be 202");
    
    testsPassed_endpoint_trie++;
    return true;
}

// Test 9: Multiple endpoints with same prefix
bool TestMultipleEndpointsSamePrefix() {
    TEST_START("Test Multiple Endpoints Same Prefix");
    
    EndpointTrie trie;
    trie.Insert("/api/user/{userId}");
    trie.Insert("/api/user/{userId}/profile");
    trie.Insert("/api/user/{userId}/settings");
    
    EndpointMatchResult result1 = trie.Search("/api/user/123");
    ASSERT(result1.found, "Should find base endpoint");
    ASSERT(result1.pattern == "/api/user/{userId}", "Should match base pattern");
    
    EndpointMatchResult result2 = trie.Search("/api/user/123/profile");
    ASSERT(result2.found, "Should find profile endpoint");
    ASSERT(result2.pattern == "/api/user/{userId}/profile", "Should match profile pattern");
    
    EndpointMatchResult result3 = trie.Search("/api/user/123/settings");
    ASSERT(result3.found, "Should find settings endpoint");
    ASSERT(result3.pattern == "/api/user/{userId}/settings", "Should match settings pattern");
    
    testsPassed_endpoint_trie++;
    return true;
}

// Test 10: Literal vs variable priority
bool TestLiteralVsVariablePriority() {
    TEST_START("Test Literal Vs Variable Priority");
    
    EndpointTrie trie;
    trie.Insert("/api/user/{userId}");
    trie.Insert("/api/user/admin");
    
    // Literal should match first
    EndpointMatchResult result1 = trie.Search("/api/user/admin");
    ASSERT(result1.found, "Should find literal match");
    ASSERT(result1.pattern == "/api/user/admin", "Should match literal pattern");
    ASSERT(result1.variables.empty(), "No variables for literal match");
    
    // Variable should match for other values
    EndpointMatchResult result2 = trie.Search("/api/user/123");
    ASSERT(result2.found, "Should find variable match");
    ASSERT(result2.pattern == "/api/user/{userId}", "Should match variable pattern");
    ASSERT(result2.variables["userId"] == "123", "userId should be 123");
    
    testsPassed_endpoint_trie++;
    return true;
}

// ========== EDGE CASES TESTS ==========

// Test 11: Root path
bool TestRootPath() {
    TEST_START("Test Root Path");
    
    EndpointTrie trie;
    trie.Insert("/");
    
    EndpointMatchResult result = trie.Search("/");
    
    ASSERT(result.found, "Should find root path");
    ASSERT(result.pattern == "/", "Pattern should be root");
    ASSERT(result.variables.empty(), "No variables for root");
    
    testsPassed_endpoint_trie++;
    return true;
}

// Test 12: Empty path
bool TestEmptyPath() {
    TEST_START("Test Empty Path");
    
    EndpointTrie trie;
    trie.Insert("/api/test");
    
    EndpointMatchResult result = trie.Search("");
    
    ASSERT(!result.found, "Empty path should not match");
    
    testsPassed_endpoint_trie++;
    return true;
}

// Test 13: Path with trailing slash
bool TestPathWithTrailingSlash() {
    TEST_START("Test Path With Trailing Slash");
    
    EndpointTrie trie;
    trie.Insert("/api/user/{userId}");
    
    EndpointMatchResult result = trie.Search("/api/user/123/");
    
    // Should not match if pattern doesn't have trailing slash
    ASSERT(!result.found, "Path with trailing slash should not match pattern without it");
    
    testsPassed_endpoint_trie++;
    return true;
}

// Test 14: Variable at the beginning
bool TestVariableAtBeginning() {
    TEST_START("Test Variable At Beginning");
    
    EndpointTrie trie;
    trie.Insert("/{version}/api/user");
    
    EndpointMatchResult result = trie.Search("/v1/api/user");
    
    ASSERT(result.found, "Should find match");
    ASSERT(result.pattern == "/{version}/api/user", "Pattern should match");
    ASSERT(result.variables["version"] == "v1", "version should be v1");
    
    testsPassed_endpoint_trie++;
    return true;
}

// Test 15: Variable at the end
bool TestVariableAtEnd() {
    TEST_START("Test Variable At End");
    
    EndpointTrie trie;
    trie.Insert("/api/user/{userId}");
    
    EndpointMatchResult result = trie.Search("/api/user/999");
    
    ASSERT(result.found, "Should find match");
    ASSERT(result.pattern == "/api/user/{userId}", "Pattern should match");
    ASSERT(result.variables["userId"] == "999", "userId should be 999");
    
    testsPassed_endpoint_trie++;
    return true;
}

// Test 16: All variables path
bool TestAllVariablesPath() {
    TEST_START("Test All Variables Path");
    
    EndpointTrie trie;
    trie.Insert("/{a}/{b}/{c}");
    
    EndpointMatchResult result = trie.Search("/x/y/z");
    
    ASSERT(result.found, "Should find match");
    ASSERT(result.pattern == "/{a}/{b}/{c}", "Pattern should match");
    ASSERT(result.variables.size() == 3, "Should have three variables");
    ASSERT(result.variables["a"] == "x", "a should be x");
    ASSERT(result.variables["b"] == "y", "b should be y");
    ASSERT(result.variables["c"] == "z", "c should be z");
    
    testsPassed_endpoint_trie++;
    return true;
}

// Test 17: Single segment path
bool TestSingleSegmentPath() {
    TEST_START("Test Single Segment Path");
    
    EndpointTrie trie;
    trie.Insert("/hello");
    
    EndpointMatchResult result = trie.Search("/hello");
    
    ASSERT(result.found, "Should find match");
    ASSERT(result.pattern == "/hello", "Pattern should match");
    
    testsPassed_endpoint_trie++;
    return true;
}

// Test 18: Single variable segment
bool TestSingleVariableSegment() {
    TEST_START("Test Single Variable Segment");
    
    EndpointTrie trie;
    trie.Insert("/{id}");
    
    EndpointMatchResult result = trie.Search("/12345");
    
    ASSERT(result.found, "Should find match");
    ASSERT(result.pattern == "/{id}", "Pattern should match");
    ASSERT(result.variables["id"] == "12345", "id should be 12345");
    
    testsPassed_endpoint_trie++;
    return true;
}

// ========== COMPLEX SCENARIOS TESTS ==========

// Test 19: Nested paths with variables
bool TestNestedPathsWithVariables() {
    TEST_START("Test Nested Paths With Variables");
    
    EndpointTrie trie;
    trie.Insert("/api/{version}/user/{userId}/post/{postId}");
    
    EndpointMatchResult result = trie.Search("/api/v2/user/456/post/789");
    
    ASSERT(result.found, "Should find match");
    ASSERT(result.pattern == "/api/{version}/user/{userId}/post/{postId}", "Pattern should match");
    ASSERT(result.variables.size() == 3, "Should have three variables");
    ASSERT(result.variables["version"] == "v2", "version should be v2");
    ASSERT(result.variables["userId"] == "456", "userId should be 456");
    ASSERT(result.variables["postId"] == "789", "postId should be 789");
    
    testsPassed_endpoint_trie++;
    return true;
}

// Test 20: Multiple variables with same name (should work)
bool TestMultipleVariablesSameName() {
    TEST_START("Test Multiple Variables Same Name");
    
    EndpointTrie trie;
    trie.Insert("/api/{id}/user/{id}");
    
    EndpointMatchResult result = trie.Search("/api/123/user/456");
    
    ASSERT(result.found, "Should find match");
    ASSERT(result.pattern == "/api/{id}/user/{id}", "Pattern should match");
    // Note: The second {id} will overwrite the first in the map
    ASSERT(result.variables["id"] == "456", "Last id should be 456");
    
    testsPassed_endpoint_trie++;
    return true;
}

// Test 21: Variable with special characters in value
bool TestVariableWithSpecialCharacters() {
    TEST_START("Test Variable With Special Characters");
    
    EndpointTrie trie;
    trie.Insert("/api/user/{userId}");
    
    EndpointMatchResult result = trie.Search("/api/user/user-123_abc");
    
    ASSERT(result.found, "Should find match");
    ASSERT(result.variables["userId"] == "user-123_abc", "userId should preserve special chars");
    
    testsPassed_endpoint_trie++;
    return true;
}

// Test 22: Variable with numbers
bool TestVariableWithNumbers() {
    TEST_START("Test Variable With Numbers");
    
    EndpointTrie trie;
    trie.Insert("/api/user/{userId}");
    
    EndpointMatchResult result = trie.Search("/api/user/123456");
    
    ASSERT(result.found, "Should find match");
    ASSERT(result.variables["userId"] == "123456", "userId should be numeric string");
    
    testsPassed_endpoint_trie++;
    return true;
}

// Test 23: Long path with many segments
bool TestLongPathManySegments() {
    TEST_START("Test Long Path Many Segments");
    
    EndpointTrie trie;
    trie.Insert("/a/b/c/d/e/f/g/h/i/j");
    
    EndpointMatchResult result = trie.Search("/a/b/c/d/e/f/g/h/i/j");
    
    ASSERT(result.found, "Should find match");
    ASSERT(result.pattern == "/a/b/c/d/e/f/g/h/i/j", "Pattern should match");
    
    testsPassed_endpoint_trie++;
    return true;
}

// Test 24: Long path with many variables
bool TestLongPathManyVariables() {
    TEST_START("Test Long Path Many Variables");
    
    EndpointTrie trie;
    trie.Insert("/{a}/{b}/{c}/{d}/{e}/{f}/{g}/{h}");
    
    EndpointMatchResult result = trie.Search("/1/2/3/4/5/6/7/8");
    
    ASSERT(result.found, "Should find match");
    ASSERT(result.variables.size() == 8, "Should have eight variables");
    ASSERT(result.variables["a"] == "1", "a should be 1");
    ASSERT(result.variables["h"] == "8", "h should be 8");
    
    testsPassed_endpoint_trie++;
    return true;
}

// ========== TRIE OPERATIONS TESTS ==========

// Test 25: IsEmpty on new trie
bool TestIsEmptyNewTrie() {
    TEST_START("Test IsEmpty New Trie");
    
    EndpointTrie trie;
    
    ASSERT(trie.IsEmpty(), "New trie should be empty");
    
    testsPassed_endpoint_trie++;
    return true;
}

// Test 26: IsEmpty after insertion
bool TestIsEmptyAfterInsertion() {
    TEST_START("Test IsEmpty After Insertion");
    
    EndpointTrie trie;
    trie.Insert("/api/test");
    
    ASSERT(!trie.IsEmpty(), "Trie should not be empty after insertion");
    
    testsPassed_endpoint_trie++;
    return true;
}

// Test 27: Clear trie
bool TestClearTrie() {
    TEST_START("Test Clear Trie");
    
    EndpointTrie trie;
    trie.Insert("/api/test1");
    trie.Insert("/api/test2");
    trie.Insert("/api/{id}");
    
    ASSERT(!trie.IsEmpty(), "Trie should not be empty");
    
    trie.Clear();
    
    ASSERT(trie.IsEmpty(), "Trie should be empty after clear");
    
    EndpointMatchResult result = trie.Search("/api/test1");
    ASSERT(!result.found, "Should not find anything after clear");
    
    testsPassed_endpoint_trie++;
    return true;
}

// Test 28: Insert same pattern twice
bool TestInsertSamePatternTwice() {
    TEST_START("Test Insert Same Pattern Twice");
    
    EndpointTrie trie;
    trie.Insert("/api/user/{id}");
    trie.Insert("/api/user/{id}");  // Insert again
    
    EndpointMatchResult result = trie.Search("/api/user/123");
    
    ASSERT(result.found, "Should still find match");
    ASSERT(result.pattern == "/api/user/{id}", "Pattern should match");
    
    testsPassed_endpoint_trie++;
    return true;
}

// ========== REAL-WORLD SCENARIOS TESTS ==========

// Test 29: RESTful API endpoints
bool TestRESTfulAPIEndpoints() {
    TEST_START("Test RESTful API Endpoints");
    
    EndpointTrie trie;
    trie.Insert("/api/users");
    trie.Insert("/api/users/{userId}");
    trie.Insert("/api/users/{userId}/posts");
    trie.Insert("/api/users/{userId}/posts/{postId}");
    trie.Insert("/api/users/{userId}/posts/{postId}/comments");
    
    // Test collection endpoint
    EndpointMatchResult result1 = trie.Search("/api/users");
    ASSERT(result1.found, "Should find users collection");
    ASSERT(result1.pattern == "/api/users", "Should match users pattern");
    
    // Test resource endpoint
    EndpointMatchResult result2 = trie.Search("/api/users/123");
    ASSERT(result2.found, "Should find user resource");
    ASSERT(result2.variables["userId"] == "123", "userId should be 123");
    
    // Test nested collection
    EndpointMatchResult result3 = trie.Search("/api/users/456/posts");
    ASSERT(result3.found, "Should find posts collection");
    ASSERT(result3.variables["userId"] == "456", "userId should be 456");
    
    // Test nested resource
    EndpointMatchResult result4 = trie.Search("/api/users/789/posts/101");
    ASSERT(result4.found, "Should find post resource");
    ASSERT(result4.variables["userId"] == "789", "userId should be 789");
    ASSERT(result4.variables["postId"] == "101", "postId should be 101");
    
    // Test deeply nested collection
    EndpointMatchResult result5 = trie.Search("/api/users/202/posts/303/comments");
    ASSERT(result5.found, "Should find comments collection");
    ASSERT(result5.variables["userId"] == "202", "userId should be 202");
    ASSERT(result5.variables["postId"] == "303", "postId should be 303");
    
    testsPassed_endpoint_trie++;
    return true;
}

// Test 30: WiFi credentials endpoints
bool TestWifiCredentialsEndpoints() {
    TEST_START("Test WiFi Credentials Endpoints");
    
    EndpointTrie trie;
    trie.Insert("/wifi-credentials");
    trie.Insert("/wifi-credentials/{ssid}");
    trie.Insert("/wifi-credentials/last-connected");
    
    // Test base endpoint
    EndpointMatchResult result1 = trie.Search("/wifi-credentials");
    ASSERT(result1.found, "Should find base endpoint");
    ASSERT(result1.pattern == "/wifi-credentials", "Should match base pattern");
    
    // Test with SSID
    EndpointMatchResult result2 = trie.Search("/wifi-credentials/MyNetwork");
    ASSERT(result2.found, "Should find SSID endpoint");
    ASSERT(result2.pattern == "/wifi-credentials/{ssid}", "Should match SSID pattern");
    ASSERT(result2.variables["ssid"] == "MyNetwork", "ssid should be MyNetwork");
    
    // Test last-connected (literal should take priority)
    EndpointMatchResult result3 = trie.Search("/wifi-credentials/last-connected");
    ASSERT(result3.found, "Should find last-connected endpoint");
    ASSERT(result3.pattern == "/wifi-credentials/last-connected", "Should match literal pattern");
    ASSERT(result3.variables.empty(), "No variables for literal match");
    
    testsPassed_endpoint_trie++;
    return true;
}

// Test 31: Conflicting patterns - literal vs variable
bool TestConflictingPatterns() {
    TEST_START("Test Conflicting Patterns");
    
    EndpointTrie trie;
    trie.Insert("/api/user/{id}");
    trie.Insert("/api/user/all");
    
    // Literal should match
    EndpointMatchResult result1 = trie.Search("/api/user/all");
    ASSERT(result1.found, "Should find literal match");
    ASSERT(result1.pattern == "/api/user/all", "Should match literal");
    
    // Variable should match for other values
    EndpointMatchResult result2 = trie.Search("/api/user/123");
    ASSERT(result2.found, "Should find variable match");
    ASSERT(result2.pattern == "/api/user/{id}", "Should match variable");
    ASSERT(result2.variables["id"] == "123", "id should be 123");
    
    testsPassed_endpoint_trie++;
    return true;
}

// Test 32: Variable in middle of path
bool TestVariableInMiddle() {
    TEST_START("Test Variable In Middle");
    
    EndpointTrie trie;
    trie.Insert("/api/{version}/user/create");
    
    EndpointMatchResult result = trie.Search("/api/v2/user/create");
    
    ASSERT(result.found, "Should find match");
    ASSERT(result.pattern == "/api/{version}/user/create", "Pattern should match");
    ASSERT(result.variables["version"] == "v2", "version should be v2");
    
    testsPassed_endpoint_trie++;
    return true;
}

// Test 33: Multiple variables in sequence
bool TestMultipleVariablesInSequence() {
    TEST_START("Test Multiple Variables In Sequence");
    
    EndpointTrie trie;
    trie.Insert("/{a}/{b}/{c}/end");
    
    EndpointMatchResult result = trie.Search("/val1/val2/val3/end");
    
    ASSERT(result.found, "Should find match");
    ASSERT(result.variables.size() == 3, "Should have three variables");
    ASSERT(result.variables["a"] == "val1", "a should be val1");
    ASSERT(result.variables["b"] == "val2", "b should be val2");
    ASSERT(result.variables["c"] == "val3", "c should be val3");
    
    testsPassed_endpoint_trie++;
    return true;
}

// Test 34: Path with empty segments (should handle gracefully)
bool TestPathWithEmptySegments() {
    TEST_START("Test Path With Empty Segments");
    
    EndpointTrie trie;
    trie.Insert("/api//user");  // Double slash creates empty segment
    
    // This might create an empty segment, but should still work
    EndpointMatchResult result = trie.Search("/api/user");
    
    // The behavior depends on how SplitPath handles empty segments
    // For now, we'll just test that it doesn't crash
    
    testsPassed_endpoint_trie++;
    return true;
}

// Test 35: Very long variable value
bool TestVeryLongVariableValue() {
    TEST_START("Test Very Long Variable Value");
    
    EndpointTrie trie;
    trie.Insert("/api/user/{userId}");
    
    StdString longValue(500, 'A');
    StdString searchPath = "/api/user/" + longValue;
    
    EndpointMatchResult result = trie.Search(searchPath);
    
    ASSERT(result.found, "Should find match");
    ASSERT(result.variables["userId"] == longValue, "Should preserve long value");
    
    testsPassed_endpoint_trie++;
    return true;
}

// Main test runner function
int RunAllEndpointTrieTests() {
    std_println("\n========================================");
    std_println("  EndpointTrie Tests");
    std_println("========================================");
    std_println("");
    
    testsPassed_endpoint_trie = 0;
    testsFailed_endpoint_trie = 0;
    
    // Basic tests
    if (!TestInsertAndSearchLiteralPath()) testsFailed_endpoint_trie++;
    if (!TestInsertAndSearchSingleVariable()) testsFailed_endpoint_trie++;
    if (!TestInsertAndSearchMultipleVariables()) testsFailed_endpoint_trie++;
    if (!TestInsertAndSearchMixedPath()) testsFailed_endpoint_trie++;
    if (!TestSearchNoMatch()) testsFailed_endpoint_trie++;
    if (!TestSearchShorterPath()) testsFailed_endpoint_trie++;
    if (!TestSearchLongerPath()) testsFailed_endpoint_trie++;
    
    // Multiple endpoints tests
    if (!TestMultipleEndpoints()) testsFailed_endpoint_trie++;
    if (!TestMultipleEndpointsSamePrefix()) testsFailed_endpoint_trie++;
    if (!TestLiteralVsVariablePriority()) testsFailed_endpoint_trie++;
    
    // Edge cases tests
    if (!TestRootPath()) testsFailed_endpoint_trie++;
    if (!TestEmptyPath()) testsFailed_endpoint_trie++;
    if (!TestPathWithTrailingSlash()) testsFailed_endpoint_trie++;
    if (!TestVariableAtBeginning()) testsFailed_endpoint_trie++;
    if (!TestVariableAtEnd()) testsFailed_endpoint_trie++;
    if (!TestAllVariablesPath()) testsFailed_endpoint_trie++;
    if (!TestSingleSegmentPath()) testsFailed_endpoint_trie++;
    if (!TestSingleVariableSegment()) testsFailed_endpoint_trie++;
    
    // Complex scenarios tests
    if (!TestNestedPathsWithVariables()) testsFailed_endpoint_trie++;
    if (!TestMultipleVariablesSameName()) testsFailed_endpoint_trie++;
    if (!TestVariableWithSpecialCharacters()) testsFailed_endpoint_trie++;
    if (!TestVariableWithNumbers()) testsFailed_endpoint_trie++;
    if (!TestLongPathManySegments()) testsFailed_endpoint_trie++;
    if (!TestLongPathManyVariables()) testsFailed_endpoint_trie++;
    
    // Trie operations tests
    if (!TestIsEmptyNewTrie()) testsFailed_endpoint_trie++;
    if (!TestIsEmptyAfterInsertion()) testsFailed_endpoint_trie++;
    if (!TestClearTrie()) testsFailed_endpoint_trie++;
    if (!TestInsertSamePatternTwice()) testsFailed_endpoint_trie++;
    
    // Real-world scenarios tests
    if (!TestRESTfulAPIEndpoints()) testsFailed_endpoint_trie++;
    if (!TestWifiCredentialsEndpoints()) testsFailed_endpoint_trie++;
    if (!TestConflictingPatterns()) testsFailed_endpoint_trie++;
    if (!TestVariableInMiddle()) testsFailed_endpoint_trie++;
    if (!TestMultipleVariablesInSequence()) testsFailed_endpoint_trie++;
    if (!TestPathWithEmptySegments()) testsFailed_endpoint_trie++;
    if (!TestVeryLongVariableValue()) testsFailed_endpoint_trie++;
    
    // Print summary
    std_println("");
    std_println("========================================");
    std_print("Tests Passed: ");
    std_println(std::to_string(testsPassed_endpoint_trie).c_str());
    std_print("Tests Failed: ");
    std_println(std::to_string(testsFailed_endpoint_trie).c_str());
    std_print("Total Tests: ");
    std_println(std::to_string(testsPassed_endpoint_trie + testsFailed_endpoint_trie).c_str());
    std_println("========================================");
    std_println("");
    
    if (testsFailed_endpoint_trie == 0) {
        std_println("✅ All EndpointTrie tests passed!");
        return 0;
    } else {
        std_println("❌ Some EndpointTrie tests failed!");
        return 1;
    }
}

#endif // ENDPOINT_TRIE_TESTS_H

