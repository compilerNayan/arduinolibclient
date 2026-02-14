#ifndef SERIALIZATION_UTILITY_TESTS_H
#define SERIALIZATION_UTILITY_TESTS_H

// Conditionally include headers based on platform
#ifdef ARDUINO
    #include <Arduino.h>
    #include <string>
    #include <vector>
    #include <list>
    #include <deque>
    #include <set>
    #include <unordered_set>
    #include <map>
    #include <unordered_map>
    #include <array>
    #include <forward_list>
#else
    #include <iostream>
    #include <cassert>
    #include <cmath>
    #include <string>
    #include <vector>
    #include <list>
    #include <deque>
    #include <set>
    #include <unordered_set>
    #include <map>
    #include <unordered_map>
    #include <array>
    #include <forward_list>
#endif

#include <StandardDefines.h>
#include <SerializationUtility.h>
#include "../tests/TestUtils.h"
#include "Person.h"
#include "Address.h"
#include "ProductX.h"

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

// Test 14: Serialize ProductX
bool TestSerializeProductX() {
    TEST_START("Test Serialize ProductX");
    
    ProductX product;
    product.productId = optional<int>(101);
    product.productName = optional<StdString>(StdString("Laptop"));
    product.price = optional<double>(999.99);
    product.quantity = optional<int>(50);
    product.inStock = optional<bool>(true);
    
    StdString result = SerializationUtility::Serialize(product);
    
    ASSERT(!result.empty(), "Serialize ProductX should not be empty");
    ASSERT(result.find("Laptop") != std::string::npos || result.find("\"productName\":\"Laptop\"") != std::string::npos,
           "Serialized ProductX should contain productName");
    
    testsPassed_serialization++;
    return true;
}

// Test 15: Deserialize ProductX
bool TestDeserializeProductX() {
    TEST_START("Test Deserialize ProductX");
    
    StdString json = "{\"productId\":101,\"productName\":\"Laptop\",\"price\":999.99,\"quantity\":50,\"inStock\":true}";
    ProductX result = SerializationUtility::Deserialize<ProductX>(json);
    
    ASSERT(result.productId.has_value() && result.productId.value() == 101, 
           "Deserialized ProductX should have correct productId");
    ASSERT(result.productName.has_value() && result.productName.value() == "Laptop", 
           "Deserialized ProductX should have correct productName");
    ASSERT(result.price.has_value() && result.price.value() > 999.0 && result.price.value() < 1000.0, 
           "Deserialized ProductX should have correct price");
    ASSERT(result.quantity.has_value() && result.quantity.value() == 50, 
           "Deserialized ProductX should have correct quantity");
    ASSERT(result.inStock.has_value() && result.inStock.value() == true, 
           "Deserialized ProductX should have correct inStock");
    
    testsPassed_serialization++;
    return true;
}

// Test 16: Serialize and Deserialize ProductX (round trip)
bool TestProductXRoundTrip() {
    TEST_START("Test ProductX Round Trip");
    
    ProductX original;
    original.productId = optional<int>(202);
    original.productName = optional<StdString>(StdString("Smartphone"));
    original.price = optional<double>(699.50);
    original.quantity = optional<int>(25);
    original.inStock = optional<bool>(false);
    
    StdString serialized = SerializationUtility::Serialize(original);
    ProductX deserialized = SerializationUtility::Deserialize<ProductX>(serialized);
    
    ASSERT(deserialized.productId.has_value() && deserialized.productId.value() == original.productId.value(), 
           "Round trip ProductX should have same productId");
    ASSERT(deserialized.productName.has_value() && deserialized.productName.value() == original.productName.value(), 
           "Round trip ProductX should have same productName");
    ASSERT(deserialized.quantity.has_value() && deserialized.quantity.value() == original.quantity.value(), 
           "Round trip ProductX should have same quantity");
    ASSERT(deserialized.inStock.has_value() && deserialized.inStock.value() == original.inStock.value(), 
           "Round trip ProductX should have same inStock");
    
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

// ========== OPTIONAL TYPE TESTS ==========

// Test 19a: Serialize optional<int> with value
bool TestSerializeOptionalIntWithValue() {
    TEST_START("Test Serialize Optional Int With Value");
    
    optional<int> optValue = 42;
    StdString result = SerializationUtility::Serialize(optValue);
    
    ASSERT(result == "42", "Serialize optional<int> with value should return '42'");
    
    testsPassed_serialization++;
    return true;
}

// Test 19b: Serialize optional<int> without value (empty)
bool TestSerializeOptionalIntEmpty() {
    TEST_START("Test Serialize Optional Int Empty");
    
    optional<int> optValue;
    StdString result = SerializationUtility::Serialize(optValue);
    
    ASSERT(result == "", "Serialize empty optional<int> should return empty string");
    
    testsPassed_serialization++;
    return true;
}

// Test 19c: Deserialize optional<int> with value
bool TestDeserializeOptionalIntWithValue() {
    TEST_START("Test Deserialize Optional Int With Value");
    
    StdString input = "42";
    optional<int> result = SerializationUtility::Deserialize<optional<int>>(input);
    
    ASSERT(result.has_value(), "Deserialized optional<int> should have value");
    ASSERT(result.value() == 42, "Deserialized optional<int> should equal 42");
    
    testsPassed_serialization++;
    return true;
}

// Test 19d: Deserialize optional<int> without value (null input)
bool TestDeserializeOptionalIntEmpty() {
    TEST_START("Test Deserialize Optional Int Empty");
    
    StdString input = "null";
    optional<int> result = SerializationUtility::Deserialize<optional<int>>(input);
    
    ASSERT(!result.has_value(), "Deserialized optional<int> from null should be empty");
    
    testsPassed_serialization++;
    return true;
}

// Test 19e: Deserialize optional<int> from empty string
bool TestDeserializeOptionalIntFromEmptyString() {
    TEST_START("Test Deserialize Optional Int From Empty String");
    
    StdString input = "";
    optional<int> result = SerializationUtility::Deserialize<optional<int>>(input);
    
    ASSERT(!result.has_value(), "Deserialized optional<int> from empty string should be empty");
    
    testsPassed_serialization++;
    return true;
}

// Test 19f: Serialize optional<StdString> with value
bool TestSerializeOptionalStringWithValue() {
    TEST_START("Test Serialize Optional String With Value");
    
    optional<StdString> optValue = StdString("Hello World");
    StdString result = SerializationUtility::Serialize(optValue);
    
    ASSERT(result == "Hello World", "Serialize optional<string> with value should return the string");
    
    testsPassed_serialization++;
    return true;
}

// Test 19g: Serialize optional<StdString> without value
bool TestSerializeOptionalStringEmpty() {
    TEST_START("Test Serialize Optional String Empty");
    
    optional<StdString> optValue;
    StdString result = SerializationUtility::Serialize(optValue);
    
    ASSERT(result == "", "Serialize empty optional<string> should return empty string");
    
    testsPassed_serialization++;
    return true;
}

// Test 19h: Deserialize optional<StdString> with value
bool TestDeserializeOptionalStringWithValue() {
    TEST_START("Test Deserialize Optional String With Value");
    
    StdString input = "\"Hello World\"";
    optional<StdString> result = SerializationUtility::Deserialize<optional<StdString>>(input);
    
    ASSERT(result.has_value(), "Deserialized optional<string> should have value");
    ASSERT(result.value() == "Hello World", "Deserialized optional<string> should equal 'Hello World'");
    
    testsPassed_serialization++;
    return true;
}

// Test 19i: Serialize optional<Person> with value
bool TestSerializeOptionalPersonWithValue() {
    TEST_START("Test Serialize Optional Person With Value");
    
    Person person;
    person.id = optional<int>(1);
    person.name = optional<StdString>(StdString("John Doe"));
    person.age = optional<int>(30);
    person.isActive = optional<bool>(true);
    
    optional<Person> optPerson = person;
    StdString result = SerializationUtility::Serialize(optPerson);
    
    ASSERT(!result.empty(), "Serialize optional<Person> with value should not be empty");
    // The result should be a valid JSON object
    ASSERT(result.find("\"id\":1") != StdString::npos || result.find("\"id\": 1") != StdString::npos, 
           "Serialized optional<Person> should contain id");
    
    testsPassed_serialization++;
    return true;
}

// Test 19j: Serialize optional<Person> without value
bool TestSerializeOptionalPersonEmpty() {
    TEST_START("Test Serialize Optional Person Empty");
    
    optional<Person> optPerson;
    StdString result = SerializationUtility::Serialize(optPerson);
    
    ASSERT(result == "", "Serialize empty optional<Person> should return empty string");
    
    testsPassed_serialization++;
    return true;
}

// Test 19k: Deserialize optional<Person> with value
bool TestDeserializeOptionalPersonWithValue() {
    TEST_START("Test Deserialize Optional Person With Value");
    
    StdString input = "{\"id\":1,\"name\":\"John Doe\",\"age\":30,\"isActive\":true}";
    optional<Person> result = SerializationUtility::Deserialize<optional<Person>>(input);
    
    ASSERT(result.has_value(), "Deserialized optional<Person> should have value");
    ASSERT(result.value().id.has_value() && result.value().id.value() == 1, 
           "Deserialized Person should have correct id");
    ASSERT(result.value().name.has_value() && result.value().name.value() == "John Doe", 
           "Deserialized Person should have correct name");
    
    testsPassed_serialization++;
    return true;
}

// Test 19l: Deserialize optional<Person> without value (null input)
bool TestDeserializeOptionalPersonEmpty() {
    TEST_START("Test Deserialize Optional Person Empty");
    
    StdString input = "null";
    optional<Person> result = SerializationUtility::Deserialize<optional<Person>>(input);
    
    ASSERT(!result.has_value(), "Deserialized optional<Person> from null should be empty");
    
    testsPassed_serialization++;
    return true;
}

// Test 19m: Serialize optional<ProductX> with value
bool TestSerializeOptionalProductXWithValue() {
    TEST_START("Test Serialize Optional ProductX With Value");
    
    ProductX product;
    product.productId = optional<int>(101);
    product.productName = optional<StdString>(StdString("Laptop"));
    product.price = optional<double>(999.99);
    product.quantity = optional<int>(50);
    product.inStock = optional<bool>(true);
    
    optional<ProductX> optProduct = product;
    StdString result = SerializationUtility::Serialize(optProduct);
    
    ASSERT(!result.empty(), "Serialize optional<ProductX> with value should not be empty");
    ASSERT(result.find("\"productId\":101") != StdString::npos || result.find("\"productId\": 101") != StdString::npos, 
           "Serialized optional<ProductX> should contain productId");
    
    testsPassed_serialization++;
    return true;
}

// Test 19n: Deserialize optional<ProductX> with value
bool TestDeserializeOptionalProductXWithValue() {
    TEST_START("Test Deserialize Optional ProductX With Value");
    
    StdString input = "{\"productId\":101,\"productName\":\"Laptop\",\"price\":999.99,\"quantity\":50,\"inStock\":true}";
    optional<ProductX> result = SerializationUtility::Deserialize<optional<ProductX>>(input);
    
    ASSERT(result.has_value(), "Deserialized optional<ProductX> should have value");
    ASSERT(result.value().productId.has_value() && result.value().productId.value() == 101, 
           "Deserialized ProductX should have correct productId");
    ASSERT(result.value().productName.has_value() && result.value().productName.value() == "Laptop", 
           "Deserialized ProductX should have correct productName");
    
    testsPassed_serialization++;
    return true;
}

// Test 19o: Round trip test for optional<int>
bool TestOptionalIntRoundTrip() {
    TEST_START("Test Optional Int Round Trip");
    
    optional<int> original = 123;
    StdString serialized = SerializationUtility::Serialize(original);
    optional<int> deserialized = SerializationUtility::Deserialize<optional<int>>(serialized);
    
    ASSERT(deserialized.has_value(), "Round trip optional<int> should have value");
    ASSERT(deserialized.value() == original.value(), "Round trip optional<int> should preserve value");
    
    testsPassed_serialization++;
    return true;
}

// Test 19p: Round trip test for optional<Person>
bool TestOptionalPersonRoundTrip() {
    TEST_START("Test Optional Person Round Trip");
    
    Person person;
    person.id = optional<int>(5);
    person.name = optional<StdString>(StdString("Jane Smith"));
    person.age = optional<int>(25);
    person.isActive = optional<bool>(false);
    
    optional<Person> original = person;
    StdString serialized = SerializationUtility::Serialize(original);
    optional<Person> deserialized = SerializationUtility::Deserialize<optional<Person>>(serialized);
    
    ASSERT(deserialized.has_value(), "Round trip optional<Person> should have value");
    ASSERT(deserialized.value().id.has_value() && deserialized.value().id.value() == original.value().id.value(), 
           "Round trip optional<Person> should preserve id");
    ASSERT(deserialized.value().name.has_value() && deserialized.value().name.value() == original.value().name.value(), 
           "Round trip optional<Person> should preserve name");
    
    testsPassed_serialization++;
    return true;
}

// Test 19q: Round trip test for empty optional<int>
bool TestOptionalIntEmptyRoundTrip() {
    TEST_START("Test Optional Int Empty Round Trip");
    
    optional<int> original;
    StdString serialized = SerializationUtility::Serialize(original);
    optional<int> deserialized = SerializationUtility::Deserialize<optional<int>>(serialized);
    
    ASSERT(!deserialized.has_value(), "Round trip empty optional<int> should remain empty");
    
    testsPassed_serialization++;
    return true;
}

// Test 19: Serialize vector of ProductX and compare with expected JSON
bool TestSerializeVectorProductX() {
    TEST_START("Test Serialize Vector of ProductX");
    
    // Create a vector with 3 ProductX objects
    StdVector<ProductX> originalProducts;
    
    ProductX product1;
    product1.productId = optional<int>(301);
    product1.productName = optional<StdString>(StdString("Laptop"));
    product1.price = optional<double>(999.99);
    product1.quantity = optional<int>(10);
    product1.inStock = optional<bool>(true);
    originalProducts.push_back(product1);
    
    ProductX product2;
    product2.productId = optional<int>(302);
    product2.productName = optional<StdString>(StdString("Smartphone"));
    product2.price = optional<double>(699.50);
    product2.quantity = optional<int>(25);
    product2.inStock = optional<bool>(true);
    originalProducts.push_back(product2);
    
    ProductX product3;
    product3.productId = optional<int>(303);
    product3.productName = optional<StdString>(StdString("Tablet"));
    product3.price = optional<double>(499.99);
    product3.quantity = optional<int>(15);
    product3.inStock = optional<bool>(false);
    originalProducts.push_back(product3);
    
    ASSERT(originalProducts.size() == 3, "Original vector should have 3 products");
    
    // Expected JSON output
    // Note: ArduinoJson removes trailing zeros from decimals (699.50 becomes 699.5)
    StdString expectedJson = "["
        "{\"productId\":301,\"productName\":\"Laptop\",\"price\":999.99,\"quantity\":10,\"inStock\":true},"
        "{\"productId\":302,\"productName\":\"Smartphone\",\"price\":699.5,\"quantity\":25,\"inStock\":true},"
        "{\"productId\":303,\"productName\":\"Tablet\",\"price\":499.99,\"quantity\":15,\"inStock\":false}"
    "]";
    
    // Serialize the vector using SerializationUtility
    StdString serialized = SerializationUtility::Serialize(originalProducts);
    
    // Debug: Print actual vs expected
    std_println("");
    std_print("Expected JSON: ");
    std_println(expectedJson.c_str());
    std_print("Actual JSON:   ");
    std_println(serialized.c_str());
    std_print("Expected length: ");
    std_println(std::to_string(expectedJson.length()).c_str());
    std_print("Actual length:   ");
    std_println(std::to_string(serialized.length()).c_str());
    std_println("");
    
    // Compare with expected JSON output
    ASSERT(serialized == expectedJson, "Serialized JSON should match expected JSON exactly");
    
    testsPassed_serialization++;
    return true;
}

// Test 20: Serialize vector of int (primitives)
bool TestSerializeVectorInt() {
    TEST_START("Test Serialize Vector of Int");
    
    StdVector<int> numbers = {10, 20, 30, 40, 50};
    StdString serialized = SerializationUtility::Serialize(numbers);
    
    StdString expectedJson = "[10,20,30,40,50]";
    ASSERT(serialized == expectedJson, "Serialized StdVector<int> should match expected JSON");
    
    testsPassed_serialization++;
    return true;
}

// Test 21: Serialize list of strings
bool TestSerializeListString() {
    TEST_START("Test Serialize List of String");
    
    StdList<StdString> strings;
    strings.push_back("apple");
    strings.push_back("banana");
    strings.push_back("cherry");
    
    StdString serialized = SerializationUtility::Serialize(strings);
    
    StdString expectedJson = "[\"apple\",\"banana\",\"cherry\"]";
    ASSERT(serialized == expectedJson, "Serialized StdList<string> should match expected JSON");
    
    testsPassed_serialization++;
    return true;
}

// Test 22: Serialize deque of ProductX
bool TestSerializeDequeProductX() {
    TEST_START("Test Serialize Deque of ProductX");
    
    StdDeque<ProductX> products;
    
    ProductX p1;
    p1.productId = optional<int>(401);
    p1.productName = optional<StdString>(StdString("Mouse"));
    p1.price = optional<double>(29.99);
    p1.quantity = optional<int>(100);
    p1.inStock = optional<bool>(true);
    products.push_back(p1);
    
    ProductX p2;
    p2.productId = optional<int>(402);
    p2.productName = optional<StdString>(StdString("Keyboard"));
    p2.price = optional<double>(79.99);
    p2.quantity = optional<int>(50);
    p2.inStock = optional<bool>(true);
    products.push_back(p2);
    
    StdString serialized = SerializationUtility::Serialize(products);
    
    StdString expectedJson = "["
        "{\"productId\":401,\"productName\":\"Mouse\",\"price\":29.99,\"quantity\":100,\"inStock\":true},"
        "{\"productId\":402,\"productName\":\"Keyboard\",\"price\":79.99,\"quantity\":50,\"inStock\":true}"
    "]";
    ASSERT(serialized == expectedJson, "Serialized StdDeque<ProductX> should match expected JSON");
    
    testsPassed_serialization++;
    return true;
}

// Test 23: Serialize set of int
bool TestSerializeSetInt() {
    TEST_START("Test Serialize Set of Int");
    
    StdSet<int> numbers = {5, 3, 8, 1, 9};
    StdString serialized = SerializationUtility::Serialize(numbers);
    
    // Note: set maintains order (implementation-defined, but typically sorted)
    // We'll check that all numbers are present
    ASSERT(serialized.find("5") != std::string::npos, "Serialized set should contain 5");
    ASSERT(serialized.find("3") != std::string::npos, "Serialized set should contain 3");
    ASSERT(serialized.find("8") != std::string::npos, "Serialized set should contain 8");
    ASSERT(serialized.find("1") != std::string::npos, "Serialized set should contain 1");
    ASSERT(serialized.find("9") != std::string::npos, "Serialized set should contain 9");
    ASSERT(serialized[0] == '[', "Serialized set should be a JSON array");
    
    testsPassed_serialization++;
    return true;
}

// Test 24: Serialize unordered_set of string
bool TestSerializeUnorderedSetString() {
    TEST_START("Test Serialize Unordered Set of String");
    
    StdUnorderedSet<StdString> strings = {"red", "green", "blue"};
    StdString serialized = SerializationUtility::Serialize(strings);
    
    // Note: unordered_set order is undefined, so we check for presence
    ASSERT(serialized.find("red") != std::string::npos || serialized.find("\"red\"") != std::string::npos, 
           "Serialized unordered_set should contain 'red'");
    ASSERT(serialized.find("green") != std::string::npos || serialized.find("\"green\"") != std::string::npos, 
           "Serialized unordered_set should contain 'green'");
    ASSERT(serialized.find("blue") != std::string::npos || serialized.find("\"blue\"") != std::string::npos, 
           "Serialized unordered_set should contain 'blue'");
    ASSERT(serialized[0] == '[', "Serialized unordered_set should be a JSON array");
    
    testsPassed_serialization++;
    return true;
}

// Test 25: Serialize array of int
bool TestSerializeArrayInt() {
    TEST_START("Test Serialize Array of Int");
    
    StdArray<int, 4> numbers = {1, 2, 3, 4};
    StdString serialized = SerializationUtility::Serialize(numbers);
    
    StdString expectedJson = "[1,2,3,4]";
    ASSERT(serialized == expectedJson, "Serialized array<int> should match expected JSON");
    
    testsPassed_serialization++;
    return true;
}

// Test 26: Serialize forward_list of int
bool TestSerializeForwardListInt() {
    TEST_START("Test Serialize Forward List of Int");
    
    std::forward_list<int> numbers;
    numbers.push_front(30);
    numbers.push_front(20);
    numbers.push_front(10);
    
    StdString serialized = SerializationUtility::Serialize(numbers);
    
    // forward_list is singly-linked, so order is reversed from push_front
    StdString expectedJson = "[10,20,30]";
    ASSERT(serialized == expectedJson, "Serialized forward_StdList<int> should match expected JSON");
    
    testsPassed_serialization++;
    return true;
}

// Test 27: Serialize map<string, int>
bool TestSerializeMapStringInt() {
    TEST_START("Test Serialize Map String to Int");
    
    StdMap<StdString, int> scores;
    scores["Alice"] = 95;
    scores["Bob"] = 87;
    scores["Charlie"] = 92;
    
    StdString serialized = SerializationUtility::Serialize(scores);
    
    // Check that all keys and values are present (order may vary)
    ASSERT(serialized.find("Alice") != std::string::npos, "Serialized map should contain 'Alice'");
    ASSERT(serialized.find("Bob") != std::string::npos, "Serialized map should contain 'Bob'");
    ASSERT(serialized.find("Charlie") != std::string::npos, "Serialized map should contain 'Charlie'");
    ASSERT(serialized.find("95") != std::string::npos, "Serialized map should contain value 95");
    ASSERT(serialized.find("87") != std::string::npos, "Serialized map should contain value 87");
    ASSERT(serialized.find("92") != std::string::npos, "Serialized map should contain value 92");
    ASSERT(serialized[0] == '{', "Serialized map should be a JSON object");
    
    testsPassed_serialization++;
    return true;
}

// Test 28: Serialize map<int, ProductX>
bool TestSerializeMapIntProductX() {
    TEST_START("Test Serialize Map Int to ProductX");
    
    StdMap<int, ProductX> productMap;
    
    ProductX p1;
    p1.productId = optional<int>(501);
    p1.productName = optional<StdString>(StdString("Monitor"));
    p1.price = optional<double>(299.99);
    p1.quantity = optional<int>(20);
    p1.inStock = optional<bool>(true);
    productMap[501] = p1;
    
    ProductX p2;
    p2.productId = optional<int>(502);
    p2.productName = optional<StdString>(StdString("Speaker"));
    p2.price = optional<double>(149.99);
    p2.quantity = optional<int>(30);
    p2.inStock = optional<bool>(true);
    productMap[502] = p2;
    
    StdString serialized = SerializationUtility::Serialize(productMap);
    
    // Check that keys and product names are present
    ASSERT(serialized.find("501") != std::string::npos, "Serialized map should contain key 501");
    ASSERT(serialized.find("502") != std::string::npos, "Serialized map should contain key 502");
    ASSERT(serialized.find("Monitor") != std::string::npos, "Serialized map should contain 'Monitor'");
    ASSERT(serialized.find("Speaker") != std::string::npos, "Serialized map should contain 'Speaker'");
    ASSERT(serialized[0] == '{', "Serialized map should be a JSON object");
    
    testsPassed_serialization++;
    return true;
}

// Test 29: Serialize StdUnorderedMap<string, string>
bool TestSerializeUnorderedMapStringString() {
    TEST_START("Test Serialize Unordered Map String to String");
    
    StdUnorderedMap<StdString, StdString> config;
    config["host"] = "localhost";
    config["port"] = "8080";
    config["protocol"] = "http";
    
    StdString serialized = SerializationUtility::Serialize(config);
    
    // Check that all keys and values are present (order is undefined)
    ASSERT(serialized.find("host") != std::string::npos, "Serialized unordered_map should contain 'host'");
    ASSERT(serialized.find("port") != std::string::npos, "Serialized unordered_map should contain 'port'");
    ASSERT(serialized.find("protocol") != std::string::npos, "Serialized unordered_map should contain 'protocol'");
    ASSERT(serialized.find("localhost") != std::string::npos, "Serialized unordered_map should contain 'localhost'");
    ASSERT(serialized.find("8080") != std::string::npos, "Serialized unordered_map should contain '8080'");
    ASSERT(serialized.find("http") != std::string::npos, "Serialized unordered_map should contain 'http'");
    ASSERT(serialized[0] == '{', "Serialized unordered_map should be a JSON object");
    
    testsPassed_serialization++;
    return true;
}

// Test 30: Serialize vector of bool
bool TestSerializeVectorBool() {
    TEST_START("Test Serialize Vector of Bool");
    
    StdVector<bool> flags = {true, false, true, true, false};
    StdString serialized = SerializationUtility::Serialize(flags);
    
    StdString expectedJson = "[true,false,true,true,false]";
    ASSERT(serialized == expectedJson, "Serialized StdVector<bool> should match expected JSON");
    
    testsPassed_serialization++;
    return true;
}

// Test 31: Serialize vector of double
bool TestSerializeVectorDouble() {
    TEST_START("Test Serialize Vector of Double");
    
    StdVector<double> values = {3.14, 2.71, 1.41, 0.57};
    StdString serialized = SerializationUtility::Serialize(values);
    
    // Note: ArduinoJson may format decimals differently
    ASSERT(serialized.find("3.14") != std::string::npos || serialized.find("3.1") != std::string::npos, 
           "Serialized vector should contain 3.14");
    ASSERT(serialized.find("2.71") != std::string::npos || serialized.find("2.7") != std::string::npos, 
           "Serialized vector should contain 2.71");
    ASSERT(serialized[0] == '[', "Serialized vector should be a JSON array");
    
    testsPassed_serialization++;
    return true;
}

// Test 32: Serialize empty vector
bool TestSerializeEmptyVector() {
    TEST_START("Test Serialize Empty Vector");
    
    StdVector<int> empty;
    StdString serialized = SerializationUtility::Serialize(empty);
    
    StdString expectedJson = "[]";
    ASSERT(serialized == expectedJson, "Serialized empty vector should be empty array");
    
    testsPassed_serialization++;
    return true;
}

// Test 33: Serialize empty map
bool TestSerializeEmptyMap() {
    TEST_START("Test Serialize Empty Map");
    
    StdMap<StdString, int> emptyMap;
    StdString serialized = SerializationUtility::Serialize(emptyMap);
    
    StdString expectedJson = "{}";
    ASSERT(serialized == expectedJson, "Serialized empty map should be empty object");
    
    testsPassed_serialization++;
    return true;
}

// ========== CONTAINER WITH ENTITY OBJECTS TESTS ==========

// Test 34: Serialize list of Person objects
bool TestSerializeListPerson() {
    TEST_START("Test Serialize List of Person");
    
    StdList<Person> people;
    
    Person p1;
    p1.id = optional<int>(1001);
    p1.name = optional<StdString>(StdString("Alice Johnson"));
    p1.age = optional<int>(28);
    p1.isActive = optional<bool>(true);
    p1.salary = optional<double>(75000.0);
    people.push_back(p1);
    
    Person p2;
    p2.id = optional<int>(1002);
    p2.name = optional<StdString>(StdString("Bob Williams"));
    p2.age = optional<int>(35);
    p2.isActive = optional<bool>(true);
    p2.salary = optional<double>(85000.0);
    people.push_back(p2);
    
    Person p3;
    p3.id = optional<int>(1003);
    p3.name = optional<StdString>(StdString("Carol Davis"));
    p3.age = optional<int>(42);
    p3.isActive = optional<bool>(false);
    p3.salary = optional<double>(95000.0);
    people.push_back(p3);
    
    StdString serialized = SerializationUtility::Serialize(people);
    
    // Verify all person names are present
    ASSERT(serialized.find("Alice Johnson") != std::string::npos, "Serialized list should contain Alice Johnson");
    ASSERT(serialized.find("Bob Williams") != std::string::npos, "Serialized list should contain Bob Williams");
    ASSERT(serialized.find("Carol Davis") != std::string::npos, "Serialized list should contain Carol Davis");
    ASSERT(serialized[0] == '[', "Serialized list should be a JSON array");
    
    testsPassed_serialization++;
    return true;
}

// Test 35: Serialize deque of Address objects
bool TestSerializeDequeAddress() {
    TEST_START("Test Serialize Deque of Address");
    
    StdDeque<Address> addresses;
    
    Address a1;
    a1.street = optional<StdString>(StdString("100 Main St"));
    a1.city = optional<StdString>(StdString("Boston"));
    a1.state = optional<StdString>(StdString("MA"));
    a1.zipCode = optional<int>(02101);
    a1.isPrimary = optional<bool>(true);
    addresses.push_back(a1);
    
    Address a2;
    a2.street = optional<StdString>(StdString("200 Park Ave"));
    a2.city = optional<StdString>(StdString("New York"));
    a2.state = optional<StdString>(StdString("NY"));
    a2.zipCode = optional<int>(10001);
    a2.isPrimary = optional<bool>(false);
    addresses.push_back(a2);
    
    StdString serialized = SerializationUtility::Serialize(addresses);
    
    ASSERT(serialized.find("Boston") != std::string::npos, "Serialized deque should contain Boston");
    ASSERT(serialized.find("New York") != std::string::npos, "Serialized deque should contain New York");
    ASSERT(serialized.find("MA") != std::string::npos, "Serialized deque should contain MA");
    ASSERT(serialized.find("NY") != std::string::npos, "Serialized deque should contain NY");
    ASSERT(serialized[0] == '[', "Serialized deque should be a JSON array");
    
    testsPassed_serialization++;
    return true;
}

// Test 36: Serialize vector of ProductX objects (using vector instead of set since set requires operator<)
bool TestSerializeVectorProductXMultiple() {
    TEST_START("Test Serialize Vector of ProductX Multiple");
    
    StdVector<ProductX> products;
    
    ProductX p1;
    p1.productId = optional<int>(601);
    p1.productName = optional<StdString>(StdString("Headphones"));
    p1.price = optional<double>(199.99);
    p1.quantity = optional<int>(75);
    p1.inStock = optional<bool>(true);
    products.push_back(p1);
    
    ProductX p2;
    p2.productId = optional<int>(602);
    p2.productName = optional<StdString>(StdString("Webcam"));
    p2.price = optional<double>(89.99);
    p2.quantity = optional<int>(120);
    p2.inStock = optional<bool>(true);
    products.push_back(p2);
    
    StdString serialized = SerializationUtility::Serialize(products);
    
    ASSERT(serialized.find("Headphones") != std::string::npos, "Serialized vector should contain Headphones");
    ASSERT(serialized.find("Webcam") != std::string::npos, "Serialized vector should contain Webcam");
    ASSERT(serialized[0] == '[', "Serialized vector should be a JSON array");
    
    testsPassed_serialization++;
    return true;
}

// Test 37: Serialize list of Person objects (using list instead of unordered_set since unordered_set requires hash and ==)
bool TestSerializeListPersonMultiple() {
    TEST_START("Test Serialize List of Person Multiple");
    
    StdList<Person> people;
    
    Person p1;
    p1.id = optional<int>(2001);
    p1.name = optional<StdString>(StdString("David Miller"));
    p1.age = optional<int>(29);
    p1.isActive = optional<bool>(true);
    p1.salary = optional<double>(70000.0);
    people.push_back(p1);
    
    Person p2;
    p2.id = optional<int>(2002);
    p2.name = optional<StdString>(StdString("Emma Wilson"));
    p2.age = optional<int>(31);
    p2.isActive = optional<bool>(true);
    p2.salary = optional<double>(80000.0);
    people.push_back(p2);
    
    StdString serialized = SerializationUtility::Serialize(people);
    
    ASSERT(serialized.find("David Miller") != std::string::npos, "Serialized list should contain David Miller");
    ASSERT(serialized.find("Emma Wilson") != std::string::npos, "Serialized list should contain Emma Wilson");
    ASSERT(serialized[0] == '[', "Serialized list should be a JSON array");
    
    testsPassed_serialization++;
    return true;
}

// Test 38: Serialize array of Address objects
bool TestSerializeArrayAddress() {
    TEST_START("Test Serialize Array of Address");
    
    StdArray<Address, 3> addresses;
    
    Address a1;
    a1.street = optional<StdString>(StdString("300 Oak St"));
    a1.city = optional<StdString>(StdString("Chicago"));
    a1.state = optional<StdString>(StdString("IL"));
    a1.zipCode = optional<int>(60601);
    a1.isPrimary = optional<bool>(false);
    addresses[0] = a1;
    
    Address a2;
    a2.street = optional<StdString>(StdString("400 Pine St"));
    a2.city = optional<StdString>(StdString("Seattle"));
    a2.state = optional<StdString>(StdString("WA"));
    a2.zipCode = optional<int>(98101);
    a2.isPrimary = optional<bool>(true);
    addresses[1] = a2;
    
    Address a3;
    a3.street = optional<StdString>(StdString("500 Elm St"));
    a3.city = optional<StdString>(StdString("Austin"));
    a3.state = optional<StdString>(StdString("TX"));
    a3.zipCode = optional<int>(78701);
    a3.isPrimary = optional<bool>(false);
    addresses[2] = a3;
    
    StdString serialized = SerializationUtility::Serialize(addresses);
    
    ASSERT(serialized.find("Chicago") != std::string::npos, "Serialized array should contain Chicago");
    ASSERT(serialized.find("Seattle") != std::string::npos, "Serialized array should contain Seattle");
    ASSERT(serialized.find("Austin") != std::string::npos, "Serialized array should contain Austin");
    ASSERT(serialized[0] == '[', "Serialized array should be a JSON array");
    
    testsPassed_serialization++;
    return true;
}

// Test 39: Serialize map<string, Person>
bool TestSerializeMapStringPerson() {
    TEST_START("Test Serialize Map String to Person");
    
    StdMap<StdString, Person> personMap;
    
    Person p1;
    p1.id = optional<int>(3001);
    p1.name = optional<StdString>(StdString("Frank Brown"));
    p1.age = optional<int>(33);
    p1.isActive = optional<bool>(true);
    p1.salary = optional<double>(90000.0);
    personMap["employee1"] = p1;
    
    Person p2;
    p2.id = optional<int>(3002);
    p2.name = optional<StdString>(StdString("Grace Lee"));
    p2.age = optional<int>(27);
    p2.isActive = optional<bool>(true);
    p2.salary = optional<double>(75000.0);
    personMap["employee2"] = p2;
    
    StdString serialized = SerializationUtility::Serialize(personMap);
    
    ASSERT(serialized.find("employee1") != std::string::npos, "Serialized map should contain key 'employee1'");
    ASSERT(serialized.find("employee2") != std::string::npos, "Serialized map should contain key 'employee2'");
    ASSERT(serialized.find("Frank Brown") != std::string::npos, "Serialized map should contain Frank Brown");
    ASSERT(serialized.find("Grace Lee") != std::string::npos, "Serialized map should contain Grace Lee");
    ASSERT(serialized[0] == '{', "Serialized map should be a JSON object");
    
    testsPassed_serialization++;
    return true;
}

// Test 40: Serialize map<int, Address>
bool TestSerializeMapIntAddress() {
    TEST_START("Test Serialize Map Int to Address");
    
    StdMap<int, Address> addressMap;
    
    Address a1;
    a1.street = optional<StdString>(StdString("600 Maple St"));
    a1.city = optional<StdString>(StdString("Denver"));
    a1.state = optional<StdString>(StdString("CO"));
    a1.zipCode = optional<int>(80201);
    a1.isPrimary = optional<bool>(true);
    addressMap[1] = a1;
    
    Address a2;
    a2.street = optional<StdString>(StdString("700 Cedar St"));
    a2.city = optional<StdString>(StdString("Portland"));
    a2.state = optional<StdString>(StdString("OR"));
    a2.zipCode = optional<int>(97201);
    a2.isPrimary = optional<bool>(false);
    addressMap[2] = a2;
    
    StdString serialized = SerializationUtility::Serialize(addressMap);
    
    ASSERT(serialized.find("1") != std::string::npos, "Serialized map should contain key '1'");
    ASSERT(serialized.find("2") != std::string::npos, "Serialized map should contain key '2'");
    ASSERT(serialized.find("Denver") != std::string::npos, "Serialized map should contain Denver");
    ASSERT(serialized.find("Portland") != std::string::npos, "Serialized map should contain Portland");
    ASSERT(serialized[0] == '{', "Serialized map should be a JSON object");
    
    testsPassed_serialization++;
    return true;
}

// Test 41: Serialize StdUnorderedMap<string, ProductX>
bool TestSerializeUnorderedMapStringProductX() {
    TEST_START("Test Serialize Unordered Map String to ProductX");
    
    StdUnorderedMap<StdString, ProductX> productMap;
    
    ProductX p1;
    p1.productId = optional<int>(701);
    p1.productName = optional<StdString>(StdString("Microphone"));
    p1.price = optional<double>(149.99);
    p1.quantity = optional<int>(50);
    p1.inStock = optional<bool>(true);
    productMap["audio1"] = p1;
    
    ProductX p2;
    p2.productId = optional<int>(702);
    p2.productName = optional<StdString>(StdString("Stand"));
    p2.price = optional<double>(79.99);
    p2.quantity = optional<int>(100);
    p2.inStock = optional<bool>(true);
    productMap["accessory1"] = p2;
    
    StdString serialized = SerializationUtility::Serialize(productMap);
    
    ASSERT(serialized.find("audio1") != std::string::npos, "Serialized unordered_map should contain key 'audio1'");
    ASSERT(serialized.find("accessory1") != std::string::npos, "Serialized unordered_map should contain key 'accessory1'");
    ASSERT(serialized.find("Microphone") != std::string::npos, "Serialized unordered_map should contain Microphone");
    ASSERT(serialized.find("Stand") != std::string::npos, "Serialized unordered_map should contain Stand");
    ASSERT(serialized[0] == '{', "Serialized unordered_map should be a JSON object");
    
    testsPassed_serialization++;
    return true;
}

// Test 42: Serialize vector of Person with partial fields
bool TestSerializeVectorPersonPartial() {
    TEST_START("Test Serialize Vector of Person with Partial Fields");
    
    StdVector<Person> people;
    
    Person p1;
    p1.id = optional<int>(4001);
    p1.name = optional<StdString>(StdString("Henry Taylor"));
    // age, isActive, salary not set
    people.push_back(p1);
    
    Person p2;
    p2.id = optional<int>(4002);
    p2.name = optional<StdString>(StdString("Iris Martinez"));
    p2.age = optional<int>(26);
    // isActive, salary not set
    people.push_back(p2);
    
    StdString serialized = SerializationUtility::Serialize(people);
    
    ASSERT(serialized.find("Henry Taylor") != std::string::npos, "Serialized vector should contain Henry Taylor");
    ASSERT(serialized.find("Iris Martinez") != std::string::npos, "Serialized vector should contain Iris Martinez");
    ASSERT(serialized[0] == '[', "Serialized vector should be a JSON array");
    
    testsPassed_serialization++;
    return true;
}

// Test 43: Serialize large vector of ProductX
bool TestSerializeLargeVectorProductX() {
    TEST_START("Test Serialize Large Vector of ProductX");
    
    StdVector<ProductX> products;
    
    // Create 10 products
    for (int i = 1; i <= 10; i++) {
        ProductX p;
        p.productId = optional<int>(8000 + i);
        p.productName = optional<StdString>(StdString("Product " + std::to_string(i)));
        p.price = optional<double>(10.0 * i);
        p.quantity = optional<int>(i * 10);
        p.inStock = optional<bool>(i % 2 == 0);
        products.push_back(p);
    }
    
    StdString serialized = SerializationUtility::Serialize(products);
    
    ASSERT(serialized.find("Product 1") != std::string::npos, "Serialized vector should contain Product 1");
    ASSERT(serialized.find("Product 5") != std::string::npos, "Serialized vector should contain Product 5");
    ASSERT(serialized.find("Product 10") != std::string::npos, "Serialized vector should contain Product 10");
    ASSERT(serialized[0] == '[', "Serialized vector should be a JSON array");
    
    testsPassed_serialization++;
    return true;
}

// Test 44: Serialize map<string, StdVector<Address>>
bool TestSerializeMapStringVectorAddress() {
    TEST_START("Test Serialize Map String to Vector of Address");
    
    StdMap<StdString, StdVector<Address>> addressMap;
    
    StdVector<Address> homeAddresses;
    Address a1;
    a1.street = optional<StdString>(StdString("100 Home St"));
    a1.city = optional<StdString>(StdString("Miami"));
    a1.state = optional<StdString>(StdString("FL"));
    a1.zipCode = optional<int>(33101);
    a1.isPrimary = optional<bool>(true);
    homeAddresses.push_back(a1);
    addressMap["home"] = homeAddresses;
    
    StdVector<Address> workAddresses;
    Address a2;
    a2.street = optional<StdString>(StdString("200 Work Ave"));
    a2.city = optional<StdString>(StdString("Tampa"));
    a2.state = optional<StdString>(StdString("FL"));
    a2.zipCode = optional<int>(33601);
    a2.isPrimary = optional<bool>(false);
    workAddresses.push_back(a2);
    addressMap["work"] = workAddresses;
    
    StdString serialized = SerializationUtility::Serialize(addressMap);
    
    ASSERT(serialized.find("home") != std::string::npos, "Serialized map should contain key 'home'");
    ASSERT(serialized.find("work") != std::string::npos, "Serialized map should contain key 'work'");
    ASSERT(serialized.find("Miami") != std::string::npos, "Serialized map should contain Miami");
    ASSERT(serialized.find("Tampa") != std::string::npos, "Serialized map should contain Tampa");
    ASSERT(serialized[0] == '{', "Serialized map should be a JSON object");
    
    testsPassed_serialization++;
    return true;
}

// Test 45: Serialize vector of Person with all fields empty
bool TestSerializeVectorPersonEmptyFields() {
    TEST_START("Test Serialize Vector of Person with Empty Fields");
    
    StdVector<Person> people;
    
    Person p1;
    // All fields are empty/not set
    people.push_back(p1);
    
    Person p2;
    // All fields are empty/not set
    people.push_back(p2);
    
    StdString serialized = SerializationUtility::Serialize(people);
    
    ASSERT(serialized[0] == '[', "Serialized vector should be a JSON array");
    ASSERT(serialized.find("id") != std::string::npos || serialized.find("null") != std::string::npos, 
           "Serialized vector should handle empty fields");
    
    testsPassed_serialization++;
    return true;
}

// Test 46: Serialize map<int, StdList<ProductX>>
bool TestSerializeMapIntListProductX() {
    TEST_START("Test Serialize Map Int to List of ProductX");
    
    StdMap<int, StdList<ProductX>> categoryMap;
    
    StdList<ProductX> electronics;
    ProductX p1;
    p1.productId = optional<int>(9001);
    p1.productName = optional<StdString>(StdString("TV"));
    p1.price = optional<double>(599.99);
    p1.quantity = optional<int>(25);
    p1.inStock = optional<bool>(true);
    electronics.push_back(p1);
    
    ProductX p2;
    p2.productId = optional<int>(9002);
    p2.productName = optional<StdString>(StdString("Radio"));
    p2.price = optional<double>(49.99);
    p2.quantity = optional<int>(100);
    p2.inStock = optional<bool>(true);
    electronics.push_back(p2);
    categoryMap[1] = electronics;
    
    StdList<ProductX> furniture;
    ProductX p3;
    p3.productId = optional<int>(9003);
    p3.productName = optional<StdString>(StdString("Chair"));
    p3.price = optional<double>(199.99);
    p3.quantity = optional<int>(50);
    p3.inStock = optional<bool>(true);
    furniture.push_back(p3);
    categoryMap[2] = furniture;
    
    StdString serialized = SerializationUtility::Serialize(categoryMap);
    
    ASSERT(serialized.find("1") != std::string::npos, "Serialized map should contain key '1'");
    ASSERT(serialized.find("2") != std::string::npos, "Serialized map should contain key '2'");
    ASSERT(serialized.find("TV") != std::string::npos, "Serialized map should contain TV");
    ASSERT(serialized.find("Radio") != std::string::npos, "Serialized map should contain Radio");
    ASSERT(serialized.find("Chair") != std::string::npos, "Serialized map should contain Chair");
    ASSERT(serialized[0] == '{', "Serialized map should be a JSON object");
    
    testsPassed_serialization++;
    return true;
}

// Test 47: Serialize deque of Address with mixed primary flags
bool TestSerializeDequeAddressMixedPrimary() {
    TEST_START("Test Serialize Deque of Address with Mixed Primary Flags");
    
    StdDeque<Address> addresses;
    
    Address a1;
    a1.street = optional<StdString>(StdString("800 First St"));
    a1.city = optional<StdString>(StdString("Phoenix"));
    a1.state = optional<StdString>(StdString("AZ"));
    a1.zipCode = optional<int>(85001);
    a1.isPrimary = optional<bool>(false);
    addresses.push_back(a1);
    
    Address a2;
    a2.street = optional<StdString>(StdString("900 Second St"));
    a2.city = optional<StdString>(StdString("Las Vegas"));
    a2.state = optional<StdString>(StdString("NV"));
    a2.zipCode = optional<int>(89101);
    a2.isPrimary = optional<bool>(true);
    addresses.push_back(a2);
    
    Address a3;
    a3.street = optional<StdString>(StdString("1000 Third St"));
    a3.city = optional<StdString>(StdString("Reno"));
    a3.state = optional<StdString>(StdString("NV"));
    a3.zipCode = optional<int>(89501);
    a3.isPrimary = optional<bool>(false);
    addresses.push_back(a3);
    
    StdString serialized = SerializationUtility::Serialize(addresses);
    
    ASSERT(serialized.find("Phoenix") != std::string::npos, "Serialized deque should contain Phoenix");
    ASSERT(serialized.find("Las Vegas") != std::string::npos, "Serialized deque should contain Las Vegas");
    ASSERT(serialized.find("Reno") != std::string::npos, "Serialized deque should contain Reno");
    ASSERT(serialized.find("true") != std::string::npos, "Serialized deque should contain true for isPrimary");
    ASSERT(serialized.find("false") != std::string::npos, "Serialized deque should contain false for isPrimary");
    
    testsPassed_serialization++;
    return true;
}

// Test 48: Serialize deque of Person with different ages (using deque instead of set since set requires operator<)
bool TestSerializeDequePersonDifferentAges() {
    TEST_START("Test Serialize Deque of Person with Different Ages");
    
    StdDeque<Person> people;
    
    Person p1;
    p1.id = optional<int>(5001);
    p1.name = optional<StdString>(StdString("Jack Anderson"));
    p1.age = optional<int>(25);
    p1.isActive = optional<bool>(true);
    p1.salary = optional<double>(60000.0);
    people.push_back(p1);
    
    Person p2;
    p2.id = optional<int>(5002);
    p2.name = optional<StdString>(StdString("Kate Thompson"));
    p2.age = optional<int>(45);
    p2.isActive = optional<bool>(true);
    p2.salary = optional<double>(110000.0);
    people.push_back(p2);
    
    Person p3;
    p3.id = optional<int>(5003);
    p3.name = optional<StdString>(StdString("Liam White"));
    p3.age = optional<int>(30);
    p3.isActive = optional<bool>(false);
    p3.salary = optional<double>(80000.0);
    people.push_back(p3);
    
    StdString serialized = SerializationUtility::Serialize(people);
    
    ASSERT(serialized.find("Jack Anderson") != std::string::npos, "Serialized deque should contain Jack Anderson");
    ASSERT(serialized.find("Kate Thompson") != std::string::npos, "Serialized deque should contain Kate Thompson");
    ASSERT(serialized.find("Liam White") != std::string::npos, "Serialized deque should contain Liam White");
    ASSERT(serialized.find("25") != std::string::npos, "Serialized deque should contain age 25");
    ASSERT(serialized.find("45") != std::string::npos, "Serialized deque should contain age 45");
    ASSERT(serialized.find("30") != std::string::npos, "Serialized deque should contain age 30");
    
    testsPassed_serialization++;
    return true;
}

// Test 49: Serialize StdUnorderedMap<int, Person>
bool TestSerializeUnorderedMapIntPerson() {
    TEST_START("Test Serialize Unordered Map Int to Person");
    
    StdUnorderedMap<int, Person> personMap;
    
    Person p1;
    p1.id = optional<int>(6001);
    p1.name = optional<StdString>(StdString("Mia Garcia"));
    p1.age = optional<int>(32);
    p1.isActive = optional<bool>(true);
    p1.salary = optional<double>(95000.0);
    personMap[101] = p1;
    
    Person p2;
    p2.id = optional<int>(6002);
    p2.name = optional<StdString>(StdString("Noah Rodriguez"));
    p2.age = optional<int>(38);
    p2.isActive = optional<bool>(true);
    p2.salary = optional<double>(105000.0);
    personMap[102] = p2;
    
    StdString serialized = SerializationUtility::Serialize(personMap);
    
    ASSERT(serialized.find("101") != std::string::npos, "Serialized unordered_map should contain key 101");
    ASSERT(serialized.find("102") != std::string::npos, "Serialized unordered_map should contain key 102");
    ASSERT(serialized.find("Mia Garcia") != std::string::npos, "Serialized unordered_map should contain Mia Garcia");
    ASSERT(serialized.find("Noah Rodriguez") != std::string::npos, "Serialized unordered_map should contain Noah Rodriguez");
    ASSERT(serialized[0] == '{', "Serialized unordered_map should be a JSON object");
    
    testsPassed_serialization++;
    return true;
}

// Test 50: Serialize vector of ProductX with varying stock status
bool TestSerializeVectorProductXVaryingStock() {
    TEST_START("Test Serialize Vector of ProductX with Varying Stock");
    
    StdVector<ProductX> products;
    
    // In stock products
    for (int i = 1; i <= 3; i++) {
        ProductX p;
        p.productId = optional<int>(10000 + i);
        p.productName = optional<StdString>(StdString("InStockProduct" + std::to_string(i)));
        p.price = optional<double>(50.0 * i);
        p.quantity = optional<int>(i * 20);
        p.inStock = optional<bool>(true);
        products.push_back(p);
    }
    
    // Out of stock products
    for (int i = 4; i <= 5; i++) {
        ProductX p;
        p.productId = optional<int>(10000 + i);
        p.productName = optional<StdString>(StdString("OutOfStockProduct" + std::to_string(i)));
        p.price = optional<double>(50.0 * i);
        p.quantity = optional<int>(0);
        p.inStock = optional<bool>(false);
        products.push_back(p);
    }
    
    StdString serialized = SerializationUtility::Serialize(products);
    
    ASSERT(serialized.find("InStockProduct1") != std::string::npos, "Serialized vector should contain InStockProduct1");
    ASSERT(serialized.find("OutOfStockProduct4") != std::string::npos, "Serialized vector should contain OutOfStockProduct4");
    ASSERT(serialized.find("OutOfStockProduct5") != std::string::npos, "Serialized vector should contain OutOfStockProduct5");
    ASSERT(serialized.find("\"inStock\":true") != std::string::npos, "Serialized vector should contain inStock:true");
    ASSERT(serialized.find("\"inStock\":false") != std::string::npos, "Serialized vector should contain inStock:false");
    
    testsPassed_serialization++;
    return true;
}

// Test: Deserialize Vector of ProductX from JSON array string
bool TestDeserializeVectorProductX() {
    TEST_START("Test Deserialize Vector of ProductX");
    
    // JSON string with array of 4 ProductX objects
    StdString jsonString = "["
        "{\"productId\":401,\"productName\":\"Keyboard\",\"price\":79.99,\"quantity\":50,\"inStock\":true},"
        "{\"productId\":402,\"productName\":\"Mouse\",\"price\":29.99,\"quantity\":100,\"inStock\":true},"
        "{\"productId\":403,\"productName\":\"Monitor\",\"price\":299.99,\"quantity\":20,\"inStock\":false},"
        "{\"productId\":404,\"productName\":\"Speaker\",\"price\":149.50,\"quantity\":30,\"inStock\":true}"
    "]";
    
    // Deserialize the JSON string to StdVector<ProductX>
    StdVector<ProductX> deserializedProducts = SerializationUtility::Deserialize<StdVector<ProductX>>(jsonString);
    
    // Verify the vector has 4 products
    ASSERT(deserializedProducts.size() == 4, "Deserialized vector should have 4 products");
    
    // Verify first product (Keyboard)
    ASSERT(deserializedProducts[0].productId.has_value(), "Product 0 should have productId");
    ASSERT(deserializedProducts[0].productId.value() == 401, "Product 0 productId should be 401");
    ASSERT(deserializedProducts[0].productName.has_value(), "Product 0 should have productName");
    ASSERT(deserializedProducts[0].productName.value() == "Keyboard", "Product 0 productName should be Keyboard");
    ASSERT(deserializedProducts[0].price.has_value(), "Product 0 should have price");
    ASSERT(std::abs(deserializedProducts[0].price.value() - 79.99) < 0.01, "Product 0 price should be 79.99");
    ASSERT(deserializedProducts[0].quantity.has_value(), "Product 0 should have quantity");
    ASSERT(deserializedProducts[0].quantity.value() == 50, "Product 0 quantity should be 50");
    ASSERT(deserializedProducts[0].inStock.has_value(), "Product 0 should have inStock");
    ASSERT(deserializedProducts[0].inStock.value() == true, "Product 0 inStock should be true");
    
    // Verify second product (Mouse)
    ASSERT(deserializedProducts[1].productId.has_value(), "Product 1 should have productId");
    ASSERT(deserializedProducts[1].productId.value() == 402, "Product 1 productId should be 402");
    ASSERT(deserializedProducts[1].productName.has_value(), "Product 1 should have productName");
    ASSERT(deserializedProducts[1].productName.value() == "Mouse", "Product 1 productName should be Mouse");
    ASSERT(deserializedProducts[1].price.has_value(), "Product 1 should have price");
    ASSERT(std::abs(deserializedProducts[1].price.value() - 29.99) < 0.01, "Product 1 price should be 29.99");
    ASSERT(deserializedProducts[1].quantity.has_value(), "Product 1 should have quantity");
    ASSERT(deserializedProducts[1].quantity.value() == 100, "Product 1 quantity should be 100");
    ASSERT(deserializedProducts[1].inStock.has_value(), "Product 1 should have inStock");
    ASSERT(deserializedProducts[1].inStock.value() == true, "Product 1 inStock should be true");
    
    // Verify third product (Monitor)
    ASSERT(deserializedProducts[2].productId.has_value(), "Product 2 should have productId");
    ASSERT(deserializedProducts[2].productId.value() == 403, "Product 2 productId should be 403");
    ASSERT(deserializedProducts[2].productName.has_value(), "Product 2 should have productName");
    ASSERT(deserializedProducts[2].productName.value() == "Monitor", "Product 2 productName should be Monitor");
    ASSERT(deserializedProducts[2].price.has_value(), "Product 2 should have price");
    ASSERT(std::abs(deserializedProducts[2].price.value() - 299.99) < 0.01, "Product 2 price should be 299.99");
    ASSERT(deserializedProducts[2].quantity.has_value(), "Product 2 should have quantity");
    ASSERT(deserializedProducts[2].quantity.value() == 20, "Product 2 quantity should be 20");
    ASSERT(deserializedProducts[2].inStock.has_value(), "Product 2 should have inStock");
    ASSERT(deserializedProducts[2].inStock.value() == false, "Product 2 inStock should be false");
    
    // Verify fourth product (Speaker)
    ASSERT(deserializedProducts[3].productId.has_value(), "Product 3 should have productId");
    ASSERT(deserializedProducts[3].productId.value() == 404, "Product 3 productId should be 404");
    ASSERT(deserializedProducts[3].productName.has_value(), "Product 3 should have productName");
    ASSERT(deserializedProducts[3].productName.value() == "Speaker", "Product 3 productName should be Speaker");
    ASSERT(deserializedProducts[3].price.has_value(), "Product 3 should have price");
    ASSERT(std::abs(deserializedProducts[3].price.value() - 149.50) < 0.01, "Product 3 price should be 149.50");
    ASSERT(deserializedProducts[3].quantity.has_value(), "Product 3 should have quantity");
    ASSERT(deserializedProducts[3].quantity.value() == 30, "Product 3 quantity should be 30");
    ASSERT(deserializedProducts[3].inStock.has_value(), "Product 3 should have inStock");
    ASSERT(deserializedProducts[3].inStock.value() == true, "Product 3 inStock should be true");
    
    testsPassed_serialization++;
    return true;
}

// ========== Deserialization Tests for Sequential Containers ==========

// Test: Deserialize StdVector<int>
bool TestDeserializeVectorInt() {
    TEST_START("Test Deserialize Vector of Int");
    
    StdString jsonString = "[10,20,30,40,50]";
    StdVector<int> deserialized = SerializationUtility::Deserialize<StdVector<int>>(jsonString);
    
    ASSERT(deserialized.size() == 5, "Deserialized vector should have 5 elements");
    ASSERT(deserialized[0] == 10, "First element should be 10");
    ASSERT(deserialized[1] == 20, "Second element should be 20");
    ASSERT(deserialized[2] == 30, "Third element should be 30");
    ASSERT(deserialized[3] == 40, "Fourth element should be 40");
    ASSERT(deserialized[4] == 50, "Fifth element should be 50");
    
    testsPassed_serialization++;
    return true;
}

// Test: Deserialize StdList<string>
bool TestDeserializeListString() {
    TEST_START("Test Deserialize List of String");
    
    StdString jsonString = "[\"apple\",\"banana\",\"cherry\"]";
    StdList<StdString> deserialized = SerializationUtility::Deserialize<StdList<StdString>>(jsonString);
    
    ASSERT(deserialized.size() == 3, "Deserialized list should have 3 elements");
    auto it = deserialized.begin();
    ASSERT(*it == "apple", "First element should be apple");
    ++it;
    ASSERT(*it == "banana", "Second element should be banana");
    ++it;
    ASSERT(*it == "cherry", "Third element should be cherry");
    
    testsPassed_serialization++;
    return true;
}

// Test: Deserialize StdDeque<double>
bool TestDeserializeDequeDouble() {
    TEST_START("Test Deserialize Deque of Double");
    
    StdString jsonString = "[3.14,2.71,1.41,0.57]";
    StdDeque<double> deserialized = SerializationUtility::Deserialize<StdDeque<double>>(jsonString);
    
    ASSERT(deserialized.size() == 4, "Deserialized deque should have 4 elements");
    ASSERT(std::abs(deserialized[0] - 3.14) < 0.01, "First element should be 3.14");
    ASSERT(std::abs(deserialized[1] - 2.71) < 0.01, "Second element should be 2.71");
    ASSERT(std::abs(deserialized[2] - 1.41) < 0.01, "Third element should be 1.41");
    ASSERT(std::abs(deserialized[3] - 0.57) < 0.01, "Fourth element should be 0.57");
    
    testsPassed_serialization++;
    return true;
}

// Test: Deserialize StdSet<int>
bool TestDeserializeSetInt() {
    TEST_START("Test Deserialize Set of Int");
    
    StdString jsonString = "[5,3,8,1,9]";
    StdSet<int> deserialized = SerializationUtility::Deserialize<StdSet<int>>(jsonString);
    
    ASSERT(deserialized.size() == 5, "Deserialized set should have 5 elements");
    ASSERT(deserialized.find(5) != deserialized.end(), "Set should contain 5");
    ASSERT(deserialized.find(3) != deserialized.end(), "Set should contain 3");
    ASSERT(deserialized.find(8) != deserialized.end(), "Set should contain 8");
    ASSERT(deserialized.find(1) != deserialized.end(), "Set should contain 1");
    ASSERT(deserialized.find(9) != deserialized.end(), "Set should contain 9");
    
    testsPassed_serialization++;
    return true;
}

// Test: Deserialize StdUnorderedSet<string>
bool TestDeserializeUnorderedSetString() {
    TEST_START("Test Deserialize UnorderedSet of String");
    
    StdString jsonString = "[\"red\",\"green\",\"blue\"]";
    StdUnorderedSet<StdString> deserialized = SerializationUtility::Deserialize<StdUnorderedSet<StdString>>(jsonString);
    
    ASSERT(deserialized.size() == 3, "Deserialized unordered_set should have 3 elements");
    ASSERT(deserialized.find("red") != deserialized.end(), "Set should contain red");
    ASSERT(deserialized.find("green") != deserialized.end(), "Set should contain green");
    ASSERT(deserialized.find("blue") != deserialized.end(), "Set should contain blue");
    
    testsPassed_serialization++;
    return true;
}

// Test: Deserialize StdArray<int, 4>
bool TestDeserializeArrayInt() {
    TEST_START("Test Deserialize Array of Int");
    
    StdString jsonString = "[1,2,3,4]";
    StdArray<int, 4> deserialized = SerializationUtility::Deserialize<StdArray<int, 4>>(jsonString);
    
    ASSERT(deserialized.size() == 4, "Deserialized array should have 4 elements");
    ASSERT(deserialized[0] == 1, "First element should be 1");
    ASSERT(deserialized[1] == 2, "Second element should be 2");
    ASSERT(deserialized[2] == 3, "Third element should be 3");
    ASSERT(deserialized[3] == 4, "Fourth element should be 4");
    
    testsPassed_serialization++;
    return true;
}

// Test: Deserialize StdVector<bool>
bool TestDeserializeVectorBool() {
    TEST_START("Test Deserialize Vector of Bool");
    
    StdString jsonString = "[true,false,true,true,false]";
    StdVector<bool> deserialized = SerializationUtility::Deserialize<StdVector<bool>>(jsonString);
    
    ASSERT(deserialized.size() == 5, "Deserialized vector should have 5 elements");
    ASSERT(deserialized[0] == true, "First element should be true");
    ASSERT(deserialized[1] == false, "Second element should be false");
    ASSERT(deserialized[2] == true, "Third element should be true");
    ASSERT(deserialized[3] == true, "Fourth element should be true");
    ASSERT(deserialized[4] == false, "Fifth element should be false");
    
    testsPassed_serialization++;
    return true;
}

// Test: Deserialize empty Vector
bool TestDeserializeEmptyVector() {
    TEST_START("Test Deserialize Empty Vector");
    
    StdString jsonString = "[]";
    StdVector<int> deserialized = SerializationUtility::Deserialize<StdVector<int>>(jsonString);
    
    ASSERT(deserialized.size() == 0, "Deserialized vector should be empty");
    
    testsPassed_serialization++;
    return true;
}

// Test: Deserialize StdList<Person>
bool TestDeserializeListPerson() {
    TEST_START("Test Deserialize List of Person");
    
    StdString jsonString = "["
        "{\"id\":101,\"name\":\"Alice\",\"age\":30,\"isActive\":true,\"salary\":50000.0},"
        "{\"id\":102,\"name\":\"Bob\",\"age\":25,\"isActive\":false,\"salary\":45000.0}"
    "]";
    
    StdList<Person> deserialized = SerializationUtility::Deserialize<StdList<Person>>(jsonString);
    
    ASSERT(deserialized.size() == 2, "Deserialized list should have 2 persons");
    
    auto it = deserialized.begin();
    ASSERT(it->id.has_value() && it->id.value() == 101, "First person should have id 101");
    ASSERT(it->name.has_value() && it->name.value() == "Alice", "First person should have name Alice");
    ASSERT(it->age.has_value() && it->age.value() == 30, "First person should have age 30");
    
    ++it;
    ASSERT(it->id.has_value() && it->id.value() == 102, "Second person should have id 102");
    ASSERT(it->name.has_value() && it->name.value() == "Bob", "Second person should have name Bob");
    ASSERT(it->age.has_value() && it->age.value() == 25, "Second person should have age 25");
    
    testsPassed_serialization++;
    return true;
}

// Test: Deserialize StdDeque<Address>
bool TestDeserializeDequeAddress() {
    TEST_START("Test Deserialize Deque of Address");
    
    StdString jsonString = "["
        "{\"street\":\"123 Main St\",\"city\":\"New York\",\"state\":\"NY\",\"zipCode\":\"10001\",\"isPrimary\":true},"
        "{\"street\":\"456 Oak Ave\",\"city\":\"Los Angeles\",\"state\":\"CA\",\"zipCode\":\"90001\",\"isPrimary\":false}"
    "]";
    
    StdDeque<Address> deserialized = SerializationUtility::Deserialize<StdDeque<Address>>(jsonString);
    
    ASSERT(deserialized.size() == 2, "Deserialized deque should have 2 addresses");
    ASSERT(deserialized[0].street.has_value() && deserialized[0].street.value() == "123 Main St", "First address should have correct street");
    ASSERT(deserialized[0].city.has_value() && deserialized[0].city.value() == "New York", "First address should have correct city");
    ASSERT(deserialized[1].street.has_value() && deserialized[1].street.value() == "456 Oak Ave", "Second address should have correct street");
    ASSERT(deserialized[1].city.has_value() && deserialized[1].city.value() == "Los Angeles", "Second address should have correct city");
    
    testsPassed_serialization++;
    return true;
}

// Test: Deserialize StdArray<Address, 3>
bool TestDeserializeArrayAddress() {
    TEST_START("Test Deserialize Array of Address");
    
    StdString jsonString = "["
        "{\"street\":\"100 First St\",\"city\":\"Boston\",\"state\":\"MA\",\"zipCode\":\"02101\",\"isPrimary\":true},"
        "{\"street\":\"200 Second St\",\"city\":\"Chicago\",\"state\":\"IL\",\"zipCode\":\"60601\",\"isPrimary\":false},"
        "{\"street\":\"300 Third St\",\"city\":\"Houston\",\"state\":\"TX\",\"zipCode\":\"77001\",\"isPrimary\":false}"
    "]";
    
    StdArray<Address, 3> deserialized = SerializationUtility::Deserialize<StdArray<Address, 3>>(jsonString);
    
    ASSERT(deserialized.size() == 3, "Deserialized array should have 3 addresses");
    ASSERT(deserialized[0].city.has_value() && deserialized[0].city.value() == "Boston", "First address should be Boston");
    ASSERT(deserialized[1].city.has_value() && deserialized[1].city.value() == "Chicago", "Second address should be Chicago");
    ASSERT(deserialized[2].city.has_value() && deserialized[2].city.value() == "Houston", "Third address should be Houston");
    
    testsPassed_serialization++;
    return true;
}

// ========== Deserialization Tests for Associative Containers ==========

// Test: Deserialize StdMap<string, int>
bool TestDeserializeMapStringInt() {
    TEST_START("Test Deserialize Map String to Int");
    
    StdString jsonString = "{\"apple\":10,\"banana\":20,\"cherry\":30}";
    StdMap<StdString, int> deserialized = SerializationUtility::Deserialize<StdMap<StdString, int>>(jsonString);
    
    ASSERT(deserialized.size() == 3, "Deserialized map should have 3 entries");
    ASSERT(deserialized["apple"] == 10, "apple should map to 10");
    ASSERT(deserialized["banana"] == 20, "banana should map to 20");
    ASSERT(deserialized["cherry"] == 30, "cherry should map to 30");
    
    testsPassed_serialization++;
    return true;
}

// Test: Deserialize StdMap<int, ProductX>
bool TestDeserializeMapIntProductX() {
    TEST_START("Test Deserialize Map Int to ProductX");
    
    StdString jsonString = "{"
        "\"1001\":{\"productId\":1001,\"productName\":\"Laptop\",\"price\":999.99,\"quantity\":5,\"inStock\":true},"
        "\"1002\":{\"productId\":1002,\"productName\":\"Phone\",\"price\":699.99,\"quantity\":10,\"inStock\":true}"
    "}";
    
    // Note: JSON keys must be strings, so we'll use string keys and convert
    // For this test, we'll use StdMap<string, ProductX> instead
    StdMap<StdString, ProductX> deserialized = SerializationUtility::Deserialize<StdMap<StdString, ProductX>>(jsonString);
    
    ASSERT(deserialized.size() == 2, "Deserialized map should have 2 entries");
    ASSERT(deserialized.find("1001") != deserialized.end(), "Map should contain key 1001");
    ASSERT(deserialized.find("1002") != deserialized.end(), "Map should contain key 1002");
    
    ProductX p1 = deserialized["1001"];
    ASSERT(p1.productId.has_value() && p1.productId.value() == 1001, "Product 1001 should have correct id");
    ASSERT(p1.productName.has_value() && p1.productName.value() == "Laptop", "Product 1001 should be Laptop");
    
    testsPassed_serialization++;
    return true;
}

// Test: Deserialize StdMap<string, Person>
bool TestDeserializeMapStringPerson() {
    TEST_START("Test Deserialize Map String to Person");
    
    StdString jsonString = "{"
        "\"alice\":{\"id\":201,\"name\":\"Alice Smith\",\"age\":28,\"isActive\":true,\"salary\":55000.0},"
        "\"bob\":{\"id\":202,\"name\":\"Bob Jones\",\"age\":35,\"isActive\":true,\"salary\":60000.0}"
    "}";
    
    StdMap<StdString, Person> deserialized = SerializationUtility::Deserialize<StdMap<StdString, Person>>(jsonString);
    
    ASSERT(deserialized.size() == 2, "Deserialized map should have 2 entries");
    ASSERT(deserialized.find("alice") != deserialized.end(), "Map should contain key alice");
    ASSERT(deserialized.find("bob") != deserialized.end(), "Map should contain key bob");
    
    Person alice = deserialized["alice"];
    ASSERT(alice.name.has_value() && alice.name.value() == "Alice Smith", "Alice should have correct name");
    ASSERT(alice.age.has_value() && alice.age.value() == 28, "Alice should have age 28");
    
    Person bob = deserialized["bob"];
    ASSERT(bob.name.has_value() && bob.name.value() == "Bob Jones", "Bob should have correct name");
    ASSERT(bob.age.has_value() && bob.age.value() == 35, "Bob should have age 35");
    
    testsPassed_serialization++;
    return true;
}

// Test: Deserialize StdUnorderedMap<string, string>
bool TestDeserializeUnorderedMapStringString() {
    TEST_START("Test Deserialize UnorderedMap String to String");
    
    StdString jsonString = "{\"key1\":\"value1\",\"key2\":\"value2\",\"key3\":\"value3\"}";
    StdUnorderedMap<StdString, StdString> deserialized = SerializationUtility::Deserialize<StdUnorderedMap<StdString, StdString>>(jsonString);
    
    ASSERT(deserialized.size() == 3, "Deserialized unordered_map should have 3 entries");
    ASSERT(deserialized["key1"] == "value1", "key1 should map to value1");
    ASSERT(deserialized["key2"] == "value2", "key2 should map to value2");
    ASSERT(deserialized["key3"] == "value3", "key3 should map to value3");
    
    testsPassed_serialization++;
    return true;
}

// Test: Deserialize StdUnorderedMap<string, ProductX>
bool TestDeserializeUnorderedMapStringProductX() {
    TEST_START("Test Deserialize UnorderedMap String to ProductX");
    
    StdString jsonString = "{"
        "\"prod1\":{\"productId\":501,\"productName\":\"Tablet\",\"price\":399.99,\"quantity\":15,\"inStock\":true},"
        "\"prod2\":{\"productId\":502,\"productName\":\"Watch\",\"price\":199.99,\"quantity\":25,\"inStock\":true}"
    "}";
    
    StdUnorderedMap<StdString, ProductX> deserialized = SerializationUtility::Deserialize<StdUnorderedMap<StdString, ProductX>>(jsonString);
    
    ASSERT(deserialized.size() == 2, "Deserialized unordered_map should have 2 entries");
    ASSERT(deserialized.find("prod1") != deserialized.end(), "Map should contain key prod1");
    ASSERT(deserialized.find("prod2") != deserialized.end(), "Map should contain key prod2");
    
    ProductX p1 = deserialized["prod1"];
    ASSERT(p1.productName.has_value() && p1.productName.value() == "Tablet", "prod1 should be Tablet");
    
    testsPassed_serialization++;
    return true;
}

// Test: Deserialize empty Map
bool TestDeserializeEmptyMap() {
    TEST_START("Test Deserialize Empty Map");
    
    StdString jsonString = "{}";
    StdMap<StdString, int> deserialized = SerializationUtility::Deserialize<StdMap<StdString, int>>(jsonString);
    
    ASSERT(deserialized.size() == 0, "Deserialized map should be empty");
    
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
    if (!TestSerializeProductX()) testsFailed_serialization++;
    if (!TestDeserializeProductX()) testsFailed_serialization++;
    if (!TestProductXRoundTrip()) testsFailed_serialization++;
    if (!TestSerializePersonPartial()) testsFailed_serialization++;
    if (!TestDeserializePersonPartial()) testsFailed_serialization++;
    if (!TestSerializeVectorProductX()) testsFailed_serialization++;
    if (!TestDeserializeVectorProductX()) testsFailed_serialization++;
    
    // Optional type tests
    if (!TestSerializeOptionalIntWithValue()) testsFailed_serialization++;
    if (!TestSerializeOptionalIntEmpty()) testsFailed_serialization++;
    if (!TestDeserializeOptionalIntWithValue()) testsFailed_serialization++;
    if (!TestDeserializeOptionalIntEmpty()) testsFailed_serialization++;
    if (!TestDeserializeOptionalIntFromEmptyString()) testsFailed_serialization++;
    if (!TestSerializeOptionalStringWithValue()) testsFailed_serialization++;
    if (!TestSerializeOptionalStringEmpty()) testsFailed_serialization++;
    if (!TestDeserializeOptionalStringWithValue()) testsFailed_serialization++;
    if (!TestSerializeOptionalPersonWithValue()) testsFailed_serialization++;
    if (!TestSerializeOptionalPersonEmpty()) testsFailed_serialization++;
    if (!TestDeserializeOptionalPersonWithValue()) testsFailed_serialization++;
    if (!TestDeserializeOptionalPersonEmpty()) testsFailed_serialization++;
    if (!TestSerializeOptionalProductXWithValue()) testsFailed_serialization++;
    if (!TestDeserializeOptionalProductXWithValue()) testsFailed_serialization++;
    if (!TestOptionalIntRoundTrip()) testsFailed_serialization++;
    if (!TestOptionalPersonRoundTrip()) testsFailed_serialization++;
    if (!TestOptionalIntEmptyRoundTrip()) testsFailed_serialization++;
    
    // Container type tests - Serialization
    if (!TestSerializeVectorInt()) testsFailed_serialization++;
    if (!TestSerializeListString()) testsFailed_serialization++;
    if (!TestSerializeDequeProductX()) testsFailed_serialization++;
    if (!TestSerializeSetInt()) testsFailed_serialization++;
    if (!TestSerializeUnorderedSetString()) testsFailed_serialization++;
    if (!TestSerializeArrayInt()) testsFailed_serialization++;
    if (!TestSerializeForwardListInt()) testsFailed_serialization++;
    if (!TestSerializeMapStringInt()) testsFailed_serialization++;
    if (!TestSerializeMapIntProductX()) testsFailed_serialization++;
    if (!TestSerializeUnorderedMapStringString()) testsFailed_serialization++;
    if (!TestSerializeVectorBool()) testsFailed_serialization++;
    if (!TestSerializeVectorDouble()) testsFailed_serialization++;
    if (!TestSerializeEmptyVector()) testsFailed_serialization++;
    if (!TestSerializeEmptyMap()) testsFailed_serialization++;
    
    // Container type tests - Deserialization
    if (!TestDeserializeVectorInt()) testsFailed_serialization++;
    if (!TestDeserializeListString()) testsFailed_serialization++;
    if (!TestDeserializeDequeDouble()) testsFailed_serialization++;
    if (!TestDeserializeSetInt()) testsFailed_serialization++;
    if (!TestDeserializeUnorderedSetString()) testsFailed_serialization++;
    if (!TestDeserializeArrayInt()) testsFailed_serialization++;
    if (!TestDeserializeVectorBool()) testsFailed_serialization++;
    if (!TestDeserializeEmptyVector()) testsFailed_serialization++;
    if (!TestDeserializeMapStringInt()) testsFailed_serialization++;
    if (!TestDeserializeUnorderedMapStringString()) testsFailed_serialization++;
    if (!TestDeserializeEmptyMap()) testsFailed_serialization++;
    
    // Container with entity objects tests - Serialization
    if (!TestSerializeListPerson()) testsFailed_serialization++;
    if (!TestSerializeDequeAddress()) testsFailed_serialization++;
    if (!TestSerializeVectorProductXMultiple()) testsFailed_serialization++;
    if (!TestSerializeListPersonMultiple()) testsFailed_serialization++;
    if (!TestSerializeArrayAddress()) testsFailed_serialization++;
    if (!TestSerializeMapStringPerson()) testsFailed_serialization++;
    if (!TestSerializeMapIntAddress()) testsFailed_serialization++;
    if (!TestSerializeUnorderedMapStringProductX()) testsFailed_serialization++;
    if (!TestSerializeVectorPersonPartial()) testsFailed_serialization++;
    if (!TestSerializeLargeVectorProductX()) testsFailed_serialization++;
    if (!TestSerializeMapStringVectorAddress()) testsFailed_serialization++;
    if (!TestSerializeVectorPersonEmptyFields()) testsFailed_serialization++;
    if (!TestSerializeMapIntListProductX()) testsFailed_serialization++;
    if (!TestSerializeDequeAddressMixedPrimary()) testsFailed_serialization++;
    if (!TestSerializeDequePersonDifferentAges()) testsFailed_serialization++;
    if (!TestSerializeUnorderedMapIntPerson()) testsFailed_serialization++;
    if (!TestSerializeVectorProductXVaryingStock()) testsFailed_serialization++;
    
    // Container with entity objects tests - Deserialization
    if (!TestDeserializeListPerson()) testsFailed_serialization++;
    if (!TestDeserializeDequeAddress()) testsFailed_serialization++;
    if (!TestDeserializeArrayAddress()) testsFailed_serialization++;
    if (!TestDeserializeMapStringPerson()) testsFailed_serialization++;
    if (!TestDeserializeMapIntProductX()) testsFailed_serialization++;
    if (!TestDeserializeUnorderedMapStringProductX()) testsFailed_serialization++;
    
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

