#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <StandardDefines.h>

/* @Entity */
class Customer {

    /// @Id
    /// @NotNull
    Public optional<int> id;

    Public optional<StdString> email;

    Public optional<StdString> firstName;

    Public optional<StdString> lastName;

    Public optional<StdString> phone;

        // Serialization method
        Public StdString Serialize() const {
            // Create JSON document
            JsonDocument doc;

            // Serialize optional field: id
            if (id.has_value()) {
                doc["id"] = id.value();
            } else {
                doc["id"] = nullptr;
            }
            // Serialize optional field: email
            if (email.has_value()) {
                doc["email"] = email.value().c_str();
            } else {
                doc["email"] = nullptr;
            }
            // Serialize optional field: firstName
            if (firstName.has_value()) {
                doc["firstName"] = firstName.value().c_str();
            } else {
                doc["firstName"] = nullptr;
            }
            // Serialize optional field: lastName
            if (lastName.has_value()) {
                doc["lastName"] = lastName.value().c_str();
            } else {
                doc["lastName"] = nullptr;
            }
            // Serialize optional field: phone
            if (phone.has_value()) {
                doc["phone"] = phone.value().c_str();
            } else {
                doc["phone"] = nullptr;
            }

            // Serialize to string
            StdString output;
            serializeJson(doc, output);

            return StdString(output.c_str());
        }

            // Validation method for all validation macros
            #pragma GCC diagnostic push
            #pragma GCC diagnostic ignored "-Wunused-parameter"
            Public template<typename DocType>
            Static StdString ValidateFields(DocType& doc) {
            StdString validationErrors;

            // No validation macros defined for this class

            return validationErrors;
        }
            #pragma GCC diagnostic pop

        // Deserialization method
        Public Static Customer Deserialize(CStdString& data) {
            // Create JSON document
            JsonDocument doc;

            // Deserialize JSON string
            DeserializationError error = deserializeJson(doc, data.c_str());

            if (error) {
                StdString errorMsg = "JSON parse error: ";
                errorMsg += error.c_str();
                throw std::runtime_error(errorMsg.c_str());
            }

            // Validate all fields with validation macros
            StdString validationErrors = ValidateFields(doc);
            if (!validationErrors.empty()) {
                throw std::runtime_error(validationErrors.c_str());
            }

            // Create object with default constructor
            Customer obj;

            // Assign values from JSON if present (only optional fields)
            // Deserialize optional field: id
            if (!doc["id"].isNull()) {
                obj.id = doc["id"].as<int>();
            }
            // Deserialize optional field: email
            if (!doc["email"].isNull()) {
                obj.email = StdString(doc["email"].as<const char*>());
            }
            // Deserialize optional field: firstName
            if (!doc["firstName"].isNull()) {
                obj.firstName = StdString(doc["firstName"].as<const char*>());
            }
            // Deserialize optional field: lastName
            if (!doc["lastName"].isNull()) {
                obj.lastName = StdString(doc["lastName"].as<const char*>());
            }
            // Deserialize optional field: phone
            if (!doc["phone"].isNull()) {
                obj.phone = StdString(doc["phone"].as<const char*>());
            }

            return obj;
        }

        // Primary key methods
        inline optional<int> GetPrimaryKey() {
            return id;
        }

        inline Static StdString GetPrimaryKeyName() {
            return "id";
        }

        inline Static StdString GetTableName() {
            return "Customer";
        }
};

#endif

