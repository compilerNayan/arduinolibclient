#ifndef CUSTOM_EXCEPTION_H
#define CUSTOM_EXCEPTION_H

#include <StandardDefines.h>

// Custom exception class (not derived from std::exception)
class CustomException {
public:
    StdString message;
    CustomException(const StdString& msg) : message(msg) {}
};

#endif // CUSTOM_EXCEPTION_H

