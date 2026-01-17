#ifndef ISWITCHDEVICE_H
#define ISWITCHDEVICE_H

#include <StandardDefines.h>
#include "SwitchState.h"

DefineStandardPointers(ISwitchDevice)
class ISwitchDevice {
    Public Virtual ~ISwitchDevice() = default;

    /**
     * @brief Get the switch ID
     * @return The switch ID
     */
    Public Virtual Int GetId() const = 0;

    /**
     * @brief Turn on the switch
     */
    Public Virtual Void TurnOn() = 0;

    /**
     * @brief Turn off the switch
     */
    Public Virtual Void TurnOff() = 0;

    /**
     * @brief Toggle the switch state
     */
    Public Virtual Void Toggle() = 0;

    /**
     * @brief Get the current actual state of the switch
     * @return SwitchState::On if both virtual and physical states match, SwitchState::Off otherwise
     */
    Public Virtual SwitchState GetState() = 0;

    /**
     * @brief Refresh the relay state based on current actual state
     * Updates relay state if it doesn't match the current actual state
     */
    Public Virtual Void Refresh() = 0;
};

#endif // ISWITCHDEVICE_H

