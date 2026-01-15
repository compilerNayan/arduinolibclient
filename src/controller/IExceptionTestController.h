#ifndef IEXCEPTIONTESTCONTROLLER_H
#define IEXCEPTIONTESTCONTROLLER_H

#include <StandardDefines.h>

// Forward declarations
template<typename T>
class ResponseEntity;

DefineStandardPointers(IExceptionTestController)
class IExceptionTestController {
public:
    virtual ~IExceptionTestController() = default;

    /**
     * @brief Throws std::runtime_error with a message
     */
    virtual ResponseEntity<StdString> ThrowRuntimeException() = 0;

    /**
     * @brief Throws std::logic_error with a message
     */
    virtual ResponseEntity<StdString> ThrowLogicException() = 0;

    /**
     * @brief Throws a const char* (not derived from std::exception)
     */
    virtual ResponseEntity<StdString> ThrowStringException() = 0;

    /**
     * @brief Throws an int (not derived from std::exception)
     */
    virtual ResponseEntity<StdString> ThrowIntException() = 0;

    /**
     * @brief Throws a custom exception type
     */
    virtual ResponseEntity<StdString> ThrowCustomException() = 0;
};

#include "ExceptionTestController.h"
#endif // IEXCEPTIONTESTCONTROLLER_H

