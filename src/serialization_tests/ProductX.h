#ifndef PRODUCT_SERIALIZATION_H
#define PRODUCT_SERIALIZATION_H

#include <StandardDefines.h>
#include <ArduinoJson.h>

/// @Serializable
class ProductX {
    Public optional<int> productId;
    Public optional<StdString> productName;
    Public optional<double> price;
    Public optional<int> quantity;
    Public optional<bool> inStock;
};

#endif // PRODUCT_SERIALIZATION_H

