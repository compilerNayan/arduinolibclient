#ifndef PERSON_H
#define PERSON_H

#include <StandardDefines.h>
#include <ArduinoJson.h>

/// @Serializable
class Person {
    Public optional<int> id;
    Public optional<StdString> name;
    Public optional<int> age;
    Public optional<bool> isActive;
    Public optional<double> salary;
};

#endif // PERSON_H

