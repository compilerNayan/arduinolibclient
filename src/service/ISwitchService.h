#ifndef ISWITCHSERVICE_H
#define ISWITCHSERVICE_H

#include <StandardDefines.h>
#include "../SwitchState.h"
#include "../controller/SwitchDto.h"

DefineStandardPointers(ISwitchService)
class ISwitchService {
    Public Virtual ~ISwitchService() = default;

    /**
     * @brief Turn on a switch by ID
     * @param id The switch ID
     * @return The final switch state after turning on, or empty optional if not found
     */
    Public Virtual optional<SwitchState> TurnOnSwitch(Int id) = 0;

    /**
     * @brief Turn off a switch by ID
     * @param id The switch ID
     * @return The final switch state after turning off, or empty optional if not found
     */
    Public Virtual optional<SwitchState> TurnOffSwitch(Int id) = 0;

    /**
     * @brief Toggle a switch by ID
     * @param id The switch ID
     * @return The final switch state after toggling, or empty optional if not found
     */
    Public Virtual optional<SwitchState> ToggleSwitch(Int id) = 0;

    /**
     * @brief Get switch state by ID
     * @param id The switch ID
     * @return The switch state, or empty optional if not found
     */
    Public Virtual optional<SwitchState> GetSwitchStateById(Int id) = 0;

    /**
     * @brief Get all switch states
     * @return Vector of SwitchDto objects containing id and state for each switch
     */
    Public Virtual Vector<SwitchDto> GetAllSwitchState() = 0;
};

#endif // ISWITCHSERVICE_H

