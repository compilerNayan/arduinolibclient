#ifndef IRESPONSEENTITYCONTROLLER_H
#define IRESPONSEENTITYCONTROLLER_H

#include <StandardDefines.h>

// Forward declarations
template<typename T>
class ResponseEntity;

class Order;

DefineStandardPointers(IResponseEntityController)
class IResponseEntityController {
public:
    virtual ~IResponseEntityController() = default;

    /**
     * @brief Returns ResponseEntity<StdString> with status 201 (CREATED)
     */
    virtual ResponseEntity<StdString> GetStringResponse() = 0;

    /**
     * @brief Returns ResponseEntity<Int> with status 202 (ACCEPTED)
     */
    virtual ResponseEntity<Int> GetIntResponse() = 0;

    /**
     * @brief Returns ResponseEntity<Order> with status 203 (NON_AUTHORITATIVE_INFORMATION)
     */
    virtual ResponseEntity<Order> GetOrderResponse() = 0;

    /**
     * @brief Returns ResponseEntity<Void> with status 404 (NOT_FOUND)
     */
    virtual ResponseEntity<Void> GetVoidResponse() = 0;
};

#include "ResponseEntityController.h"
#endif // IRESPONSEENTITYCONTROLLER_H

