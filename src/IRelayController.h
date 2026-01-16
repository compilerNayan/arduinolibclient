#ifndef IRELAYCONTROLLER_H
#define IRELAYCONTROLLER_H

#include <StandardDefines.h>
#include "SwitchState.h"

DefineStandardPointers(IRelayController)
class IRelayController {
    Public Virtual ~IRelayController() = default;

    /**
     * @brief Turn on the relay switch
     * @param pin The GPIO pin number to control
     */
    Public Virtual Void TurnOn(Int pin) = 0;

    /**
     * @brief Turn off the relay switch
     * @param pin The GPIO pin number to control
     */
    Public Virtual Void TurnOff(Int pin) = 0;

    /**
     * @brief Get the current state of the relay switch
     * @param pin The GPIO pin number to read
     * @return SwitchState::On if relay is on, SwitchState::Off if off
     */
    Public Virtual SwitchState GetState(Int pin) = 0;
};

#endif // IRELAYCONTROLLER_H

