#ifndef ADDRESS_H
#define ADDRESS_H

#include <StandardDefines.h>
#include <ArduinoJson.h>

/* @Serializable */
class Address {
    Public optional<StdString> street;
    Public optional<StdString> city;
    Public optional<StdString> state;
    Public optional<int> zipCode;
    Public optional<bool> isPrimary;
};

#endif // ADDRESS_H

