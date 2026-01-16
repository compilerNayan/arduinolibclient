#ifndef IRELAYCONTROLLER_H
#define IRELAYCONTROLLER_H

#include <StandardDefines.h>

DefineStandardPointers(IRelayController)
class IRelayController {
    Public Virtual ~IRelayController() = default;

    /**
     * @brief Set a pin to HIGH state (turn relay ON)
     * @param pin The GPIO pin number to control
     */
    Public Virtual Void SetHigh(Int pin) = 0;

    /**
     * @brief Set a pin to LOW state (turn relay OFF)
     * @param pin The GPIO pin number to control
     */
    Public Virtual Void SetLow(Int pin) = 0;

    /**
     * @brief Write a state to a pin (HIGH or LOW)
     * @param pin The GPIO pin number to control
     * @param isHigh true for HIGH, false for LOW
     */
    Public Virtual Void Write(Int pin, Bool isHigh) = 0;

    /**
     * @brief Read the current state of a pin
     * @param pin The GPIO pin number to read
     * @return true if pin is HIGH, false if LOW
     */
    Public Virtual Bool Read(Int pin) = 0;
};

#endif // IRELAYCONTROLLER_H

