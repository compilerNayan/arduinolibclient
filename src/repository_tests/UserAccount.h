#ifndef USERACCOUNT_H
#define USERACCOUNT_H

#include <StandardDefines.h>

/* @Entity */
class UserAccount {

    /// @Id
    /// @NotNull
    Public optional<StdString> username;

    Public optional<StdString> password;

    Public optional<StdString> name;

        // Serialization method
        Public StdString Serialize() const {
            // Create JSON document
            JsonDocument doc;

            // Serialize optional field: username
            if (username.has_value()) {
                doc["username"] = username.value().c_str();
            } else {
                doc["username"] = nullptr;
            }
            // Serialize optional field: password
            if (password.has_value()) {
                doc["password"] = password.value().c_str();
            } else {
                doc["password"] = nullptr;
            }
            // Serialize optional field: name
            if (name.has_value()) {
                doc["name"] = name.value().c_str();
            } else {
                doc["name"] = nullptr;
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
        Public Static UserAccount Deserialize(CStdString& data) {
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
            UserAccount obj;

            // Assign values from JSON if present (only optional fields)
            // Deserialize optional field: username
            if (!doc["username"].isNull()) {
                obj.username = StdString(doc["username"].as<const char*>());
            }
            // Deserialize optional field: password
            if (!doc["password"].isNull()) {
                obj.password = StdString(doc["password"].as<const char*>());
            }
            // Deserialize optional field: name
            if (!doc["name"].isNull()) {
                obj.name = StdString(doc["name"].as<const char*>());
            }

            return obj;
        }

        // Primary key methods
        inline optional<StdString> GetPrimaryKey() {
            return username;
        }

        inline Static StdString GetPrimaryKeyName() {
            return "username";
        }

        inline Static StdString GetTableName() {
            return "UserAccount";
        }
};

#endif

