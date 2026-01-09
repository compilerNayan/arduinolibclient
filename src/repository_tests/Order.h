#ifndef ORDER_H
#define ORDER_H

#include <StandardDefines.h>

/* @Entity */
class Order {

    /// @Id
    /// @NotNull
    Public optional<int> id;

    Public optional<StdString> orderNumber;

    Public optional<int> customerId;

    Public optional<double> totalAmount;

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
            // Serialize optional field: orderNumber
            if (orderNumber.has_value()) {
                doc["orderNumber"] = orderNumber.value().c_str();
            } else {
                doc["orderNumber"] = nullptr;
            }
            // Serialize optional field: customerId
            if (customerId.has_value()) {
                doc["customerId"] = customerId.value();
            } else {
                doc["customerId"] = nullptr;
            }
            // Serialize optional field: totalAmount
            if (totalAmount.has_value()) {
                doc["totalAmount"] = totalAmount.value();
            } else {
                doc["totalAmount"] = nullptr;
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
        Public Static Order Deserialize(CStdString& data) {
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
            Order obj;

            // Assign values from JSON if present (only optional fields)
            // Deserialize optional field: id
            if (!doc["id"].isNull()) {
                obj.id = doc["id"].as<int>();
            }
            // Deserialize optional field: orderNumber
            if (!doc["orderNumber"].isNull()) {
                obj.orderNumber = StdString(doc["orderNumber"].as<const char*>());
            }
            // Deserialize optional field: customerId
            if (!doc["customerId"].isNull()) {
                obj.customerId = doc["customerId"].as<int>();
            }
            // Deserialize optional field: totalAmount
            if (!doc["totalAmount"].isNull()) {
                obj.totalAmount = doc["totalAmount"].as<double>();
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
            return "Order";
        }
};

#endif

