#ifndef EXCEPTIONTESTCONTROLLER_H
#define EXCEPTIONTESTCONTROLLER_H

#include <StandardDefines.h>
#include "IExceptionTestController.h"
#include "ResponseEntity.h"
#include "HttpStatus.h"
#include "CustomException.h"
#include <stdexcept>
#include <exception>

/* @RestController */
/* @RequestMapping("/exception-test") */
class ExceptionTestController final : public IExceptionTestController {
public:

    /* @GetMapping("/runtime-error") */
    ResponseEntity<StdString> ThrowRuntimeException() override {
        throw std::runtime_error("This is a runtime error with a detailed message");
    }

    /* @GetMapping("/logic-error") */
    ResponseEntity<StdString> ThrowLogicException() override {
        throw std::logic_error("This is a logic error: invalid operation attempted");
    }

    /* @GetMapping("/string-exception") */
    ResponseEntity<StdString> ThrowStringException() override {
        throw "This is a const char* exception (not std::exception)";
    }

    /* @GetMapping("/int-exception") */
    ResponseEntity<StdString> ThrowIntException() override {
        throw 42;  // Throwing an int
    }

    /* @GetMapping("/custom-exception") */
    ResponseEntity<StdString> ThrowCustomException() override {
        throw CustomException("This is a custom exception type");
    }
};

#endif // EXCEPTIONTESTCONTROLLER_H

