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
#include "../TestUtils.h"
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

// Test 19: Serialize vector of ProductX and compare with expected JSON
bool TestSerializeVectorProductX() {
    TEST_START("Test Serialize Vector of ProductX");
    
    // Create a vector with 3 ProductX objects
    vector<ProductX> originalProducts;
    
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
    
    vector<int> numbers = {10, 20, 30, 40, 50};
    StdString serialized = SerializationUtility::Serialize(numbers);
    
    StdString expectedJson = "[10,20,30,40,50]";
    ASSERT(serialized == expectedJson, "Serialized vector<int> should match expected JSON");
    
    testsPassed_serialization++;
    return true;
}

// Test 21: Serialize list of strings
bool TestSerializeListString() {
    TEST_START("Test Serialize List of String");
    
    list<StdString> strings;
    strings.push_back("apple");
    strings.push_back("banana");
    strings.push_back("cherry");
    
    StdString serialized = SerializationUtility::Serialize(strings);
    
    StdString expectedJson = "[\"apple\",\"banana\",\"cherry\"]";
    ASSERT(serialized == expectedJson, "Serialized list<string> should match expected JSON");
    
    testsPassed_serialization++;
    return true;
}

// Test 22: Serialize deque of ProductX
bool TestSerializeDequeProductX() {
    TEST_START("Test Serialize Deque of ProductX");
    
    deque<ProductX> products;
    
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
    ASSERT(serialized == expectedJson, "Serialized deque<ProductX> should match expected JSON");
    
    testsPassed_serialization++;
    return true;
}

// Test 23: Serialize set of int
bool TestSerializeSetInt() {
    TEST_START("Test Serialize Set of Int");
    
    set<int> numbers = {5, 3, 8, 1, 9};
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
    
    unordered_set<StdString> strings = {"red", "green", "blue"};
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
    
    std::array<int, 4> numbers = {1, 2, 3, 4};
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
    ASSERT(serialized == expectedJson, "Serialized forward_list<int> should match expected JSON");
    
    testsPassed_serialization++;
    return true;
}

// Test 27: Serialize map<string, int>
bool TestSerializeMapStringInt() {
    TEST_START("Test Serialize Map String to Int");
    
    std_map<StdString, int> scores;
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
    
    std_map<int, ProductX> productMap;
    
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

// Test 29: Serialize unordered_map<string, string>
bool TestSerializeUnorderedMapStringString() {
    TEST_START("Test Serialize Unordered Map String to String");
    
    unordered_map<StdString, StdString> config;
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
    
    vector<bool> flags = {true, false, true, true, false};
    StdString serialized = SerializationUtility::Serialize(flags);
    
    StdString expectedJson = "[true,false,true,true,false]";
    ASSERT(serialized == expectedJson, "Serialized vector<bool> should match expected JSON");
    
    testsPassed_serialization++;
    return true;
}

// Test 31: Serialize vector of double
bool TestSerializeVectorDouble() {
    TEST_START("Test Serialize Vector of Double");
    
    vector<double> values = {3.14, 2.71, 1.41, 0.57};
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
    
    vector<int> empty;
    StdString serialized = SerializationUtility::Serialize(empty);
    
    StdString expectedJson = "[]";
    ASSERT(serialized == expectedJson, "Serialized empty vector should be empty array");
    
    testsPassed_serialization++;
    return true;
}

// Test 33: Serialize empty map
bool TestSerializeEmptyMap() {
    TEST_START("Test Serialize Empty Map");
    
    std_map<StdString, int> emptyMap;
    StdString serialized = SerializationUtility::Serialize(emptyMap);
    
    StdString expectedJson = "{}";
    ASSERT(serialized == expectedJson, "Serialized empty map should be empty object");
    
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
    
    // Container type tests
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

