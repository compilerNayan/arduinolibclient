#ifndef RESPONSEENTITYCONTROLLER_H
#define RESPONSEENTITYCONTROLLER_H

#include <StandardDefines.h>
#include "IResponseEntityController.h"
#include "ResponseEntity.h"
#include "HttpStatus.h"
#include "../repository_tests/Order.h"

/* @RestController */
/* @RequestMapping("/response-entity") */
class ResponseEntityController final : public IResponseEntityController {
public:

    /* @GetMapping("/string") */
    ResponseEntity<StdString> GetStringResponse() override {
        StdString message = "Created successfully";
        return ResponseEntity<StdString>::Created(message);
    }

    /* @GetMapping("/int") */
    ResponseEntity<Int> GetIntResponse() override {
        Int value = 42;
        return ResponseEntity<Int>::Accepted(value);
    }

    /* @GetMapping("/order") */
    ResponseEntity<Order> GetOrderResponse() override {
        Order order;
        order.id = 1;
        order.orderNumber = StdString("ORD-12345");
        order.customerId = 100;
        order.totalAmount = 99.99;
        return ResponseEntity<Order>::Status(HttpStatus::NON_AUTHORITATIVE_INFORMATION, order);
    }

    /* @GetMapping("/void") */
    ResponseEntity<Void> GetVoidResponse() override {
        return ResponseEntity<Void>::NotFound();
    }
};

#endif // RESPONSEENTITYCONTROLLER_H

