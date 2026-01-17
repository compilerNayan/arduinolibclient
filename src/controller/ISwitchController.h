#ifndef ISWITCHCONTROLLER_H
#define ISWITCHCONTROLLER_H

#include <StandardDefines.h>

// Forward declarations
template<typename T>
class ResponseEntity;

class SwitchDto;

DefineStandardPointers(ISwitchController)
class ISwitchController {
    Public Virtual ~ISwitchController() = default;

    /**
     * @brief Turn on a switch by ID
     * @param id The switch ID
     * @return ResponseEntity<SwitchDto> with the updated switch state, or 404 if not found
     */
    Public Virtual ResponseEntity<SwitchDto> TurnOnSwitch(Int id) = 0;

    /**
     * @brief Turn off a switch by ID
     * @param id The switch ID
     * @return ResponseEntity<SwitchDto> with the updated switch state, or 404 if not found
     */
    Public Virtual ResponseEntity<SwitchDto> TurnOffSwitch(Int id) = 0;

    /**
     * @brief Toggle a switch by ID
     * @param id The switch ID
     * @return ResponseEntity<SwitchDto> with the updated switch state, or 404 if not found
     */
    Public Virtual ResponseEntity<SwitchDto> ToggleSwitch(Int id) = 0;

    /**
     * @brief Get switch state by ID
     * @param id The switch ID
     * @return ResponseEntity<SwitchDto> with the switch state, or 404 if not found
     */
    Public Virtual ResponseEntity<SwitchDto> GetSwitchStateById(Int id) = 0;

    /**
     * @brief Get all switch states
     * @return ResponseEntity<Vector<SwitchDto>> with all switch states
     */
    Public Virtual ResponseEntity<Vector<SwitchDto>> GetAllSwitchState() = 0;
};

#include "SwitchController.h"
#endif // ISWITCHCONTROLLER_H

