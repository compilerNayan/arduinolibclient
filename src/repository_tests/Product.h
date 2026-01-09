#ifndef PRODUCT_H
#define PRODUCT_H

#include <StandardDefines.h>

/* @Entity */
class Product {

    /// @Id
    /// @NotNull
    Public optional<int> id;

    Public optional<StdString> name;

    Public optional<double> price;

    Public optional<StdString> category;

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
            // Serialize optional field: name
            if (name.has_value()) {
                doc["name"] = name.value().c_str();
            } else {
                doc["name"] = nullptr;
            }
            // Serialize optional field: price
            if (price.has_value()) {
                doc["price"] = price.value();
            } else {
                doc["price"] = nullptr;
            }
            // Serialize optional field: category
            if (category.has_value()) {
                doc["category"] = category.value().c_str();
            } else {
                doc["category"] = nullptr;
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
        Public Static Product Deserialize(CStdString& data) {
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
            Product obj;

            // Assign values from JSON if present (only optional fields)
            // Deserialize optional field: id
            if (!doc["id"].isNull()) {
                obj.id = doc["id"].as<int>();
            }
            // Deserialize optional field: name
            if (!doc["name"].isNull()) {
                obj.name = StdString(doc["name"].as<const char*>());
            }
            // Deserialize optional field: price
            if (!doc["price"].isNull()) {
                obj.price = doc["price"].as<double>();
            }
            // Deserialize optional field: category
            if (!doc["category"].isNull()) {
                obj.category = StdString(doc["category"].as<const char*>());
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
            return "Product";
        }
};

#endif

