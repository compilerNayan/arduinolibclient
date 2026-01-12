#ifndef SERIALIZATION_UTILITY_TESTS_H
#define SERIALIZATION_UTILITY_TESTS_H

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
#include "../TestUtils.h"
#include "Person.h"
#include "Address.h"
#include "Product.h"

using namespace nayan::serializer;

// Test counters
static int testsPassed_serialization = 0;
static int testsFailed_serialization = 0;

// ========== PRIMITIVE TYPE TESTS ==========

// Test 1: Serialize int
bool TestSerializeInt() {
    TEST_START("Test Serialize Int");
    
    int value = 42;
    StdString result = SerializationUtility::Serialize(value);
    
    ASSERT(result == "42", "Serialize int should return '42'");
    
    testsPassed_serialization++;
    return true;
}

// Test 2: Deserialize int
bool TestDeserializeInt() {
    TEST_START("Test Deserialize Int");
    
    StdString input = "42";
    int result = SerializationUtility::Deserialize<int>(input);
    
    ASSERT(result == 42, "Deserialize int should return 42");
    
    testsPassed_serialization++;
    return true;
}

// Test 3: Serialize double
bool TestSerializeDouble() {
    TEST_START("Test Serialize Double");
    
    double value = 3.14159;
    StdString result = SerializationUtility::Serialize(value);
    
    // Check that result contains the number (format may vary)
    ASSERT(!result.empty(), "Serialize double should not be empty");
    
    testsPassed_serialization++;
    return true;
}

// Test 4: Deserialize double
bool TestDeserializeDouble() {
    TEST_START("Test Deserialize Double");
    
    StdString input = "3.14159";
    double result = SerializationUtility::Deserialize<double>(input);
    
    ASSERT(result > 3.14 && result < 3.15, "Deserialize double should return approximately 3.14159");
    
    testsPassed_serialization++;
    return true;
}

// Test 5: Serialize bool
bool TestSerializeBool() {
    TEST_START("Test Serialize Bool");
    
    bool valueTrue = true;
    bool valueFalse = false;
    
    StdString resultTrue = SerializationUtility::Serialize(valueTrue);
    StdString resultFalse = SerializationUtility::Serialize(valueFalse);
    
    ASSERT(resultTrue == "true", "Serialize bool true should return 'true'");
    ASSERT(resultFalse == "false", "Serialize bool false should return 'false'");
    
    testsPassed_serialization++;
    return true;
}

// Test 6: Deserialize bool
bool TestDeserializeBool() {
    TEST_START("Test Deserialize Bool");
    
    bool resultTrue = SerializationUtility::Deserialize<bool>(StdString("true"));
    bool resultFalse = SerializationUtility::Deserialize<bool>(StdString("false"));
    bool resultOne = SerializationUtility::Deserialize<bool>(StdString("1"));
    bool resultZero = SerializationUtility::Deserialize<bool>(StdString("0"));
    
    ASSERT(resultTrue == true, "Deserialize 'true' should return true");
    ASSERT(resultFalse == false, "Deserialize 'false' should return false");
    ASSERT(resultOne == true, "Deserialize '1' should return true");
    ASSERT(resultZero == false, "Deserialize '0' should return false");
    
    testsPassed_serialization++;
    return true;
}

// Test 7: Serialize string
bool TestSerializeString() {
    TEST_START("Test Serialize String");
    
    StdString value = "Hello World";
    StdString result = SerializationUtility::Serialize(value);
    
    ASSERT(result == "Hello World", "Serialize string should return the same string");
    
    testsPassed_serialization++;
    return true;
}

// Test 8: Deserialize string
bool TestDeserializeString() {
    TEST_START("Test Deserialize String");
    
    StdString input = "Hello World";
    StdString result = SerializationUtility::Deserialize<StdString>(input);
    
    ASSERT(result == "Hello World", "Deserialize string should return the same string");
    
    testsPassed_serialization++;
    return true;
}

// ========== COMPLEX TYPE TESTS ==========

// Test 9: Serialize Person
bool TestSerializePerson() {
    TEST_START("Test Serialize Person");
    
    Person person;
    person.id = optional<int>(1);
    person.name = optional<StdString>(StdString("John Doe"));
    person.age = optional<int>(30);
    person.isActive = optional<bool>(true);
    person.salary = optional<double>(50000.50);
    
    StdString result = SerializationUtility::Serialize(person);
    
    ASSERT(!result.empty(), "Serialize Person should not be empty");
    ASSERT(result.find("John Doe") != std::string::npos || result.find("\"name\":\"John Doe\"") != std::string::npos, 
           "Serialized Person should contain name");
    ASSERT(result.find("1") != std::string::npos || result.find("\"id\":1") != std::string::npos,
           "Serialized Person should contain id");
    
    testsPassed_serialization++;
    return true;
}

// Test 10: Deserialize Person
bool TestDeserializePerson() {
    TEST_START("Test Deserialize Person");
    
    StdString json = "{\"id\":1,\"name\":\"John Doe\",\"age\":30,\"isActive\":true,\"salary\":50000.50}";
    Person result = SerializationUtility::Deserialize<Person>(json);
    
    ASSERT(result.id.has_value() && result.id.value() == 1, "Deserialized Person should have id=1");
    ASSERT(result.name.has_value() && result.name.value() == "John Doe", "Deserialized Person should have name='John Doe'");
    ASSERT(result.age.has_value() && result.age.value() == 30, "Deserialized Person should have age=30");
    ASSERT(result.isActive.has_value() && result.isActive.value() == true, "Deserialized Person should have isActive=true");
    ASSERT(result.salary.has_value() && result.salary.value() > 50000.0 && result.salary.value() < 50001.0, 
           "Deserialized Person should have salary approximately 50000.50");
    
    testsPassed_serialization++;
    return true;
}

// Test 11: Serialize and Deserialize Person (round trip)
bool TestPersonRoundTrip() {
    TEST_START("Test Person Round Trip");
    
    Person original;
    original.id = optional<int>(2);
    original.name = optional<StdString>(StdString("Jane Smith"));
    original.age = optional<int>(25);
    original.isActive = optional<bool>(false);
    original.salary = optional<double>(75000.75);
    
    StdString serialized = SerializationUtility::Serialize(original);
    Person deserialized = SerializationUtility::Deserialize<Person>(serialized);
    
    ASSERT(deserialized.id.has_value() && deserialized.id.value() == original.id.value(), 
           "Round trip Person should have same id");
    ASSERT(deserialized.name.has_value() && deserialized.name.value() == original.name.value(), 
           "Round trip Person should have same name");
    ASSERT(deserialized.age.has_value() && deserialized.age.value() == original.age.value(), 
           "Round trip Person should have same age");
    ASSERT(deserialized.isActive.has_value() && deserialized.isActive.value() == original.isActive.value(), 
           "Round trip Person should have same isActive");
    
    testsPassed_serialization++;
    return true;
}

// Test 12: Serialize Address
bool TestSerializeAddress() {
    TEST_START("Test Serialize Address");
    
    Address address;
    address.street = optional<StdString>(StdString("123 Main St"));
    address.city = optional<StdString>(StdString("New York"));
    address.state = optional<StdString>(StdString("NY"));
    address.zipCode = optional<int>(10001);
    address.isPrimary = optional<bool>(true);
    
    StdString result = SerializationUtility::Serialize(address);
    
    ASSERT(!result.empty(), "Serialize Address should not be empty");
    ASSERT(result.find("New York") != std::string::npos || result.find("\"city\":\"New York\"") != std::string::npos,
           "Serialized Address should contain city");
    
    testsPassed_serialization++;
    return true;
}

// Test 13: Deserialize Address
bool TestDeserializeAddress() {
    TEST_START("Test Deserialize Address");
    
    StdString json = "{\"street\":\"123 Main St\",\"city\":\"New York\",\"state\":\"NY\",\"zipCode\":10001,\"isPrimary\":true}";
    Address result = SerializationUtility::Deserialize<Address>(json);
    
    ASSERT(result.street.has_value() && result.street.value() == "123 Main St", 
           "Deserialized Address should have correct street");
    ASSERT(result.city.has_value() && result.city.value() == "New York", 
           "Deserialized Address should have correct city");
    ASSERT(result.state.has_value() && result.state.value() == "NY", 
           "Deserialized Address should have correct state");
    ASSERT(result.zipCode.has_value() && result.zipCode.value() == 10001, 
           "Deserialized Address should have correct zipCode");
    ASSERT(result.isPrimary.has_value() && result.isPrimary.value() == true, 
           "Deserialized Address should have correct isPrimary");
    
    testsPassed_serialization++;
    return true;
}

// Test 14: Serialize Product
bool TestSerializeProduct() {
    TEST_START("Test Serialize Product");
    
    Product product;
    product.productId = optional<int>(101);
    product.productName = optional<StdString>(StdString("Laptop"));
    product.price = optional<double>(999.99);
    product.quantity = optional<int>(50);
    product.inStock = optional<bool>(true);
    
    StdString result = SerializationUtility::Serialize(product);
    
    ASSERT(!result.empty(), "Serialize Product should not be empty");
    ASSERT(result.find("Laptop") != std::string::npos || result.find("\"productName\":\"Laptop\"") != std::string::npos,
           "Serialized Product should contain productName");
    
    testsPassed_serialization++;
    return true;
}

// Test 15: Deserialize Product
bool TestDeserializeProduct() {
    TEST_START("Test Deserialize Product");
    
    StdString json = "{\"productId\":101,\"productName\":\"Laptop\",\"price\":999.99,\"quantity\":50,\"inStock\":true}";
    Product result = SerializationUtility::Deserialize<Product>(json);
    
    ASSERT(result.productId.has_value() && result.productId.value() == 101, 
           "Deserialized Product should have correct productId");
    ASSERT(result.productName.has_value() && result.productName.value() == "Laptop", 
           "Deserialized Product should have correct productName");
    ASSERT(result.price.has_value() && result.price.value() > 999.0 && result.price.value() < 1000.0, 
           "Deserialized Product should have correct price");
    ASSERT(result.quantity.has_value() && result.quantity.value() == 50, 
           "Deserialized Product should have correct quantity");
    ASSERT(result.inStock.has_value() && result.inStock.value() == true, 
           "Deserialized Product should have correct inStock");
    
    testsPassed_serialization++;
    return true;
}

// Test 16: Serialize and Deserialize Product (round trip)
bool TestProductRoundTrip() {
    TEST_START("Test Product Round Trip");
    
    Product original;
    original.productId = optional<int>(202);
    original.productName = optional<StdString>(StdString("Smartphone"));
    original.price = optional<double>(699.50);
    original.quantity = optional<int>(25);
    original.inStock = optional<bool>(false);
    
    StdString serialized = SerializationUtility::Serialize(original);
    Product deserialized = SerializationUtility::Deserialize<Product>(serialized);
    
    ASSERT(deserialized.productId.has_value() && deserialized.productId.value() == original.productId.value(), 
           "Round trip Product should have same productId");
    ASSERT(deserialized.productName.has_value() && deserialized.productName.value() == original.productName.value(), 
           "Round trip Product should have same productName");
    ASSERT(deserialized.quantity.has_value() && deserialized.quantity.value() == original.quantity.value(), 
           "Round trip Product should have same quantity");
    ASSERT(deserialized.inStock.has_value() && deserialized.inStock.value() == original.inStock.value(), 
           "Round trip Product should have same inStock");
    
    testsPassed_serialization++;
    return true;
}

// Test 17: Serialize Person with partial fields
bool TestSerializePersonPartial() {
    TEST_START("Test Serialize Person Partial Fields");
    
    Person person;
    person.id = optional<int>(3);
    person.name = optional<StdString>(StdString("Partial Person"));
    // age, isActive, salary are not set (no value)
    
    StdString result = SerializationUtility::Serialize(person);
    
    ASSERT(!result.empty(), "Serialize Person with partial fields should not be empty");
    ASSERT(result.find("Partial Person") != std::string::npos || result.find("\"name\":\"Partial Person\"") != std::string::npos,
           "Serialized Person should contain set fields");
    
    testsPassed_serialization++;
    return true;
}

// Test 18: Deserialize Person with partial fields
bool TestDeserializePersonPartial() {
    TEST_START("Test Deserialize Person Partial Fields");
    
    StdString json = "{\"id\":3,\"name\":\"Partial Person\"}";
    Person result = SerializationUtility::Deserialize<Person>(json);
    
    ASSERT(result.id.has_value() && result.id.value() == 3, 
           "Deserialized Person should have id");
    ASSERT(result.name.has_value() && result.name.value() == "Partial Person", 
           "Deserialized Person should have name");
    // Other fields may or may not have values - that's OK
    
    testsPassed_serialization++;
    return true;
}

// Main test runner function
int RunAllSerializationTests() {
    std_println("");
    std_println("========================================");
    std_println("  SerializationUtility Tests");
    std_println("========================================");
    std_println("");
    
    testsPassed_serialization = 0;
    testsFailed_serialization = 0;
    
    // Primitive type tests
    if (!TestSerializeInt()) testsFailed_serialization++;
    if (!TestDeserializeInt()) testsFailed_serialization++;
    if (!TestSerializeDouble()) testsFailed_serialization++;
    if (!TestDeserializeDouble()) testsFailed_serialization++;
    if (!TestSerializeBool()) testsFailed_serialization++;
    if (!TestDeserializeBool()) testsFailed_serialization++;
    if (!TestSerializeString()) testsFailed_serialization++;
    if (!TestDeserializeString()) testsFailed_serialization++;
    
    // Complex type tests
    if (!TestSerializePerson()) testsFailed_serialization++;
    if (!TestDeserializePerson()) testsFailed_serialization++;
    if (!TestPersonRoundTrip()) testsFailed_serialization++;
    if (!TestSerializeAddress()) testsFailed_serialization++;
    if (!TestDeserializeAddress()) testsFailed_serialization++;
    if (!TestSerializeProduct()) testsFailed_serialization++;
    if (!TestDeserializeProduct()) testsFailed_serialization++;
    if (!TestProductRoundTrip()) testsFailed_serialization++;
    if (!TestSerializePersonPartial()) testsFailed_serialization++;
    if (!TestDeserializePersonPartial()) testsFailed_serialization++;
    
    // Print summary
    std_println("");
    std_println("========================================");
    std_println("  Test Summary");
    std_println("========================================");
    std_print("Tests Passed: ");
    std_println(testsPassed_serialization);
    std_print("Tests Failed: ");
    std_println(testsFailed_serialization);
    std_print("Total Tests: ");
    std_println(testsPassed_serialization + testsFailed_serialization);
    std_println("========================================");
    std_println("");
    
    if (testsFailed_serialization == 0) {
        std_println("✅ All serialization tests passed!");
        return 0;
    } else {
        std_println("❌ Some serialization tests failed!");
        return 1;
    }
}

#endif // SERIALIZATION_UTILITY_TESTS_H

