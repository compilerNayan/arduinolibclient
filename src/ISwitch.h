#ifndef ISWITCH_H
#define ISWITCH_H

#include <StandardDefines.h>
#include "SwitchState.h"

DefineStandardPointers(ISwitch)
class ISwitch {
    Public Virtual ~ISwitch() = default;

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

#endif // ISWITCH_H

